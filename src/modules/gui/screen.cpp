#include <iostream>
#include <glm/glm.hpp>
#include <glib-unix.h>

#include <modules/gui/screen.hpp>
#include <shaders/shader.hpp>
#include <oscilloscope.hpp>
#include <utils/capturers/puertoSerie.hpp>
#include <utils/capturers/signalGenerator.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  STATIC FUNCTIONS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
static void updateDropPort(Screen *userData, bool add, char *text){
    GtkDropDown *dropPort = GTK_DROP_DOWN(userData->dropPort);
    GListModel *model = gtk_drop_down_get_model(dropPort);

    if(model == nullptr){
        if(add){
            const char *auxArr[] = {text, nullptr};
            GtkStringList *newList = gtk_string_list_new(static_cast<const char* const*>(auxArr));
            gtk_drop_down_set_model(dropPort, G_LIST_MODEL(newList));
            g_object_unref(newList);
        }
        return;
    }

    unsigned int n = g_list_model_get_n_items(model);
    int foundIndex = -1;

    for(unsigned int i = 0; i < n; i++){
        GObject *item = reinterpret_cast<GObject*>(g_list_model_get_item(model, i));
        if(GTK_IS_STRING_OBJECT(item)){
            const char *s = gtk_string_object_get_string(GTK_STRING_OBJECT(item));
            if(g_strcmp0(s, text) == 0){
                foundIndex = static_cast<int>(i);
                g_object_unref(item);//eee
            }
        }
        g_object_unref(item);
    }

    if(add){
        if(foundIndex != -1){
            g_object_unref(model);
            return;
        }

        gchar **arr = g_new0(gchar*, n + 2); /* n existentes + 1 nuevo + NULL */
        for(guint i = 0; i < n; ++i){
            GObject *item = reinterpret_cast<GObject*>(g_list_model_get_item(model, i)); /* nueva ref */
            if(GTK_IS_STRING_OBJECT(item)){
                const char *s = gtk_string_object_get_string(GTK_STRING_OBJECT(item));
                arr[i] = g_strdup(s);
            }else{
                arr[i] = g_strdup(""); /* fallback */
            }
            g_object_unref(item);
        }
        arr[n] = g_strdup(text);
        arr[n+1] = nullptr;

        GtkStringList *new_list = gtk_string_list_new((const char * const *)arr);
        gtk_drop_down_set_model(dropPort, G_LIST_MODEL(new_list));
        g_object_unref(new_list);

        /* liberar arr y strings duplicadas */
        for(guint i = 0; i < n + 1; ++i)
            g_free(arr[i]);
        g_free(arr);
    }else{
        if(foundIndex < 0){
            g_object_unref(model);
            return;
        }

        if(n <= 1){
            gtk_drop_down_set_model(dropPort, nullptr);
            g_object_unref(model);
            return;
        }

        gchar **arr = g_new0(gchar*, n); /* n-1 elementos + NULL => tamaño n */
        guint j = 0;
        for (guint i = 0; i < n; ++i) {
            if ((int)i == foundIndex)
                continue;

            GObject *item = reinterpret_cast<GObject*>(g_list_model_get_item(model, i)); /* nueva ref */
            if (GTK_IS_STRING_OBJECT(item)) {
                const char *s = gtk_string_object_get_string(GTK_STRING_OBJECT(item));
                arr[j++] = g_strdup(s);
            } else {
                arr[j++] = g_strdup("");
            }
            g_object_unref(item);
        }
        arr[j] = NULL;

        GtkStringList *new_list = gtk_string_list_new((const char * const *)arr);
        gtk_drop_down_set_model(dropPort, G_LIST_MODEL(new_list));
        g_object_unref(new_list);

        for (guint k = 0; k < j; ++k)
            g_free(arr[k]);
        g_free(arr);
    }
    g_object_unref(model);
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  INSPECTION PORTS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
static gboolean udev_monitor_callback([[maybe_unused]]gint fd, [[maybe_unused]]GIOCondition condition, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);

    struct udev_device *dev = udev_monitor_receive_device(screen->monitor);
    if(!dev){
        return TRUE;
    }

    const char *action = udev_device_get_action(dev);
    const char *devnode = udev_device_get_devnode(dev);

    if(action && devnode){
        if(strcmp(action, "add") == 0){
            gchar *copy = g_strdup(devnode);
            updateDropPort(screen, true, copy);
            g_free(copy);
        }else if(strcmp(action, "remove") == 0){
            gchar *copy = g_strdup(devnode);
            updateDropPort(screen, false, copy);
            g_free(copy);
        }
    }
    
    udev_device_unref(dev);

    return TRUE;
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      CALLBACKS CANVA VOLTAGE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void realizeVoltage(GtkGLArea *area, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "Failed to link contex areaSignal : realize\n", -1);
        std::cerr << "Failed to link contex areaSignal : realize" << std::endl;
        return;
    }

    ProgramShader shader("./src/shaders/codeVertexES.vs", "./src/shaders/codeFragmentES.fs");
    screen->idShaderVolt = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    screen->gridVoltage.createGrid();
    screen->createVAO(osc->voltage1_);
    screen->createVAO(osc->voltage2_);
    screen->createVAO(osc->voltage3_);
    screen->createVAO(osc->voltage4_);
}

void unrealizeVoltage(GtkGLArea *area, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "Failed to link contex areaSignal : unrealize\n", -1);
        std::cerr << "Failed to link contex areaSignal : unrealize" << std::endl; 
    }

    screen->gridVoltage.deleteGrid();
    screen->deleteVAO(osc->voltage1_);
    screen->deleteVAO(osc->voltage2_);
    screen->deleteVAO(osc->voltage3_);
    screen->deleteVAO(osc->voltage4_);

    glDeleteProgram(screen->idShaderVolt);
}

gboolean renderVoltage(GtkGLArea *area, [[maybe_unused]]GdkGLContext *context, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "Failed to link contex areaSignal : render\n", -1);
        std::cerr << "Failed to link contex areaSignal : render" << std::endl;
        return FALSE;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(screen->idShaderVolt);
    // Verificar si el shader está en uso
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

    osc->voltage1_.applyOffset(screen->offset1_, screen->voltDiv_);
    osc->voltage2_.applyOffset(screen->offset2_, screen->voltDiv_);
    osc->voltage3_.applyOffset(screen->offset3_, screen->voltDiv_);
    osc->voltage4_.applyOffset(screen->offset4_, screen->voltDiv_);

    if(static_cast<unsigned int>(currentProgram) == (screen->idShaderVolt) && (screen->idShaderVolt) != 0){
        //aqui dibuja
        screen->gridVoltage.draw();
        if(screen->stateSignal1_){
            screen->drawVAO(osc->voltage1_);
        }
        if(screen->stateSignal2_){
            screen->drawVAO(osc->voltage2_);
        }
        if(screen->stateSignal3_){
            screen->drawVAO(osc->voltage3_);
        }
        if(screen->stateSignal4_){
            screen->drawVAO(osc->voltage4_);
        }
    }

    // Verificar errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "OpenGL Error\n", -1);
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    glFlush();

    return TRUE;
}

void resizeVoltage([[maybe_unused]]GtkGLArea *area, [[maybe_unused]]gpointer userData){
    //glViewport(0, 0, width, height);
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      CALLBACKS CANVA SPECTRUM
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
static void realizeSpectrum(GtkGLArea *area, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);

    if(!gtk_gl_area_get_context(area)){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "No OpenGL context available\n", -1);
        std::cerr << "No OpenGL context available\n";
        return;
    }

    if(gtk_gl_area_get_error(area) != NULL){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "Failed to link contex areaSpectrum : realize\n", -1);
        std::cerr << "Failed to link contex areaSpectrum : realize" << std::endl;
        return;
    }

    ProgramShader shader("./src/shaders/codeVertexES.vs", "./src/shaders/codeFragmentES.fs");
    screen->idShaderSpec = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    screen->gridSpectrum.createGrid();

    //Aqui crea los espectros
    screen->createVAO(osc->voltage1_.spectrumSignal);
    screen->createVAO(osc->voltage2_.spectrumSignal);
    screen->createVAO(osc->voltage3_.spectrumSignal);
    screen->createVAO(osc->voltage4_.spectrumSignal);
}

static void unrealizeSpectrum(GtkGLArea *area, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "Failed to link contex areaSpectrum : unrealize\n", -1);
        std::cerr << "Failed to link contex areaSpectrum : unrealize" << std::endl; 
    }

    screen->gridSpectrum.deleteGrid();
    //Aqui Elimina los spectros
    screen->deleteVAO(osc->voltage1_.spectrumSignal);
    screen->deleteVAO(osc->voltage2_.spectrumSignal);
    screen->deleteVAO(osc->voltage3_.spectrumSignal);
    screen->deleteVAO(osc->voltage4_.spectrumSignal);

    glDeleteProgram(screen->idShaderSpec);
}

static gboolean renderSpectrum(GtkGLArea *area, [[maybe_unused]]GdkGLContext *context, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "Failed to link contex areaSpectrum : render\n", -1);
        std::cerr << "Failed to link contex areaSpectrum : render" << std::endl;
        return FALSE;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //aqui ajusta los voltajes

    glUseProgram(screen->idShaderSpec);
    // Verificar si el shader está en uso
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if(osc->stateStartStop_.load(std::memory_order_acquire)){
        osc->voltage1_.spectrumSignal.updateVertex();
        osc->voltage2_.spectrumSignal.updateVertex();
        osc->voltage3_.spectrumSignal.updateVertex();
        osc->voltage4_.spectrumSignal.updateVertex();
    }
    if(static_cast<unsigned int>(currentProgram) == (screen->idShaderSpec) && (screen->idShaderSpec) != 0){
        screen->gridSpectrum.draw();
        //aqui dibuja
        screen->gridVoltage.draw();
        if(screen->stateSignal1_){
            screen->drawVAO(osc->voltage1_.spectrumSignal);
        }
        if(screen->stateSignal2_){
            screen->drawVAO(osc->voltage2_.spectrumSignal);
        }
        if(screen->stateSignal3_){
            screen->drawVAO(osc->voltage3_.spectrumSignal);
        }
        if(screen->stateSignal4_){
            screen->drawVAO(osc->voltage4_.spectrumSignal);
        }
    }

    // Verificar errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert(screen->bufferConsole, &end, "OpenGL Error\n", -1);
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    glFlush();

    return TRUE;
}

static void resize(){
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      CALLBACKS CONTROL PANEL
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void funcStartStop(GtkWidget *widget, [[maybe_unused]]gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();

    osc->stateStartStop_.store(!(osc->stateStartStop_), std::memory_order_release);
    if(osc->stateStartStop_.load(std::memory_order_acquire)){
        gtk_button_set_label(GTK_BUTTON(widget), "Stop");
        gtk_widget_remove_css_class(widget, "led-off");
        gtk_widget_add_css_class(widget, "led-on");
    }else{
        gtk_button_set_label(GTK_BUTTON(widget), "Start");
        gtk_widget_remove_css_class(widget, "led-on");
        gtk_widget_add_css_class(widget, "led-off");
    }
}

void funcVoltDiv(GtkDropDown *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    switch(gtk_drop_down_get_selected(widget)){
        case 0:
            screen->voltDiv_ = 0.1f;
            break;
        case 1:
            screen->voltDiv_ = 0.2f;
            break;
        case 2:
            screen->voltDiv_ = 0.3f;
            break;
        case 3:
            screen->voltDiv_ = 0.4f;
            break;
        case 4:
            screen->voltDiv_ = 0.5f;
            break;
        case 5:
            screen->voltDiv_ = 1.0f;
            break;
        case 6:
            screen->voltDiv_ = 2.0f;
            break;
        case 7:
            screen->voltDiv_ = 3.0f;
            break;
        case 8:
            screen->voltDiv_ = 4.0f;
            break;
        case 9:
            screen->voltDiv_ = 5.0f;
            break;
        default:
            screen->voltDiv_ = 1.0f;
    }
}

void funcCheckSig1(GtkCheckButton *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->stateSignal1_ = gtk_check_button_get_active(widget);
}

void funcCheckSig2(GtkCheckButton *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->stateSignal2_ = gtk_check_button_get_active(widget);
}

void funcCheckSig3(GtkCheckButton *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->stateSignal3_ = gtk_check_button_get_active(widget);
}

void funcCheckSig4(GtkCheckButton *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->stateSignal4_ = gtk_check_button_get_active(widget);
}

void funcOffset1(GtkSpinButton *spinButton, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->offset1_ = gtk_spin_button_get_value(spinButton);
    screen->draw_signals();
}

void funcOffset2(GtkSpinButton *spinButton, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->offset2_ = gtk_spin_button_get_value(spinButton);
    screen->draw_signals();
}

void funcOffset3(GtkSpinButton *spinButton, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->offset3_ = gtk_spin_button_get_value(spinButton);
    screen->draw_signals();
}

void funcOffset4(GtkSpinButton *spinButton, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->offset4_ = gtk_spin_button_get_value(spinButton);
    screen->draw_signals();
}

void funcSpinButtonFreq(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();
    osc->signalCapturer_.setTimeDiv(gtk_spin_button_get_value(spinButton));
}

void funcCheckTestSignal(GtkCheckButton *widget, gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();
    Screen *screen = reinterpret_cast<Screen*>(userData);

    if(gtk_check_button_get_active(widget)){
        osc->signalCapturer_.selectCapturer(std::make_unique<SignalGenerator>());
        gtk_button_set_label(GTK_BUTTON(screen->buttonPort), "Open");
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
        gtk_text_buffer_insert_with_tags_by_name(screen->bufferConsole, &end, "Puerto serial cerrado\n", -1, "close", NULL);
    }else{
        osc->signalCapturer_.selectCapturer(std::make_unique<ComSerial>());
    }
}

void funcDropPort(GtkDropDown *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    GtkStringObject *item = reinterpret_cast<GtkStringObject*>(gtk_drop_down_get_selected_item(widget));
    const char *text = gtk_string_object_get_string(item);
    if(text != nullptr){
        screen->routePort = text;
    }
}

void funcButtonPort(GtkWidget *widget, gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();
    Screen *screen = static_cast<Screen*>(userData);

    if(osc->signalCapturer_.capturer->getId() == SERIAL_PORT_ID){
        if(osc->signalCapturer_.capturer->getFlagSerial()){
            osc->signalCapturer_.capturer->closePort();
            if(!(osc->signalCapturer_.capturer->getFlagSerial())){
                gtk_button_set_label(GTK_BUTTON(widget), "Open");
                GtkTextIter end;
                gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
                gtk_text_buffer_insert_with_tags_by_name(screen->bufferConsole, &end, "Puerto serial cerrado\n", -1, "close", NULL);
            }
        }else{
            osc->signalCapturer_.capturer->openPort(screen->routePort.c_str());
            if(osc->signalCapturer_.capturer->getFlagSerial()){
                gtk_button_set_label(GTK_BUTTON(widget), "Close");
                GtkTextIter end;
                gtk_text_buffer_get_end_iter(screen->bufferConsole, &end);
                gtk_text_buffer_insert_with_tags_by_name(screen->bufferConsole, &end, "Puerto serial abierto\n", -1, "open", NULL);
            }
        }
    }
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      WINDOW CALLBACKS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
static void destroyWindow([[maybe_unused]]GtkWidget *widget, [[maybe_unused]]gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();
    osc->stateStartStop_.store(false, std::memory_order_release);
    osc->stateOnOff_.store(false, std::memory_order_release);
}

static void activate(GtkApplication* app, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *boxPanels = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        GtkWidget *boxView = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
        gtk_widget_set_hexpand(boxView, true);
        gtk_widget_set_vexpand(boxView, true);
            screen->glAreaVoltage = gtk_gl_area_new();
            gtk_widget_set_size_request(screen->glAreaVoltage, -1, 700);

            screen->glAreaSpectrum = gtk_gl_area_new();
            gtk_widget_set_size_request(screen->glAreaSpectrum, -1, 100);

            GtkTextTagTable *tagTableConsole = gtk_text_tag_table_new();
            screen->bufferConsole = gtk_text_buffer_new(tagTableConsole);
            GtkWidget *appConsole = gtk_text_view_new_with_buffer(screen->bufferConsole);
            gtk_text_view_set_editable(GTK_TEXT_VIEW(appConsole), false);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appConsole), false);
            gtk_text_view_set_monospace(GTK_TEXT_VIEW(appConsole), true);
            GtkWidget *windowScrool = gtk_scrolled_window_new();
            gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(windowScrool), appConsole);
            gtk_widget_set_size_request(windowScrool, -1, 100);

            GtkTextTag *tag_error = gtk_text_tag_new("error");
            g_object_set(tag_error, "foreground", "red", NULL);
            gtk_text_tag_table_add(tagTableConsole, tag_error);

            GtkTextTag *tag_open = gtk_text_tag_new("open");
            g_object_set(tag_open, "foreground", "green", NULL);
            gtk_text_tag_table_add(tagTableConsole, tag_open);

            GtkTextTag *tag_close = gtk_text_tag_new("close");
            g_object_set(tag_close, "foreground", "purple", NULL);
            gtk_text_tag_table_add(tagTableConsole, tag_close);

        GtkWidget *boxControl = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
        gtk_widget_set_vexpand(boxControl, true);
            GtkCssProvider *provider = gtk_css_provider_new();
            GFile *fileStyles = g_file_new_for_path("./src/styles/styleButtonStartStop.css"); 
            gtk_css_provider_load_from_file(provider, fileStyles);
            g_object_unref(fileStyles);
            gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            GtkWidget *buttonStartStop = gtk_button_new_with_label("Start");
            gtk_widget_add_css_class(buttonStartStop, "led-off");

            GtkWidget *separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
            gtk_widget_set_vexpand(separator1, true);

            const char *optionsVolDiv[] = {
                "0.1v/div",
                "0.2v/div",
                "0.3v/div",
                "0.4v/div",
                "0.5v/div",
                "1v/div",
                "2v/div",
                "3v/div",
                "4v/div",
                "5v/div",
                nullptr
            };
            GtkStringList *listVoltDiv = gtk_string_list_new(optionsVolDiv);
            GtkWidget *dropVoltDiv = gtk_drop_down_new(G_LIST_MODEL(listVoltDiv), nullptr);
            gtk_drop_down_set_selected(GTK_DROP_DOWN(dropVoltDiv), 5);
            GtkWidget *gridSignals = gtk_grid_new();
            GtkWidget *checkSignal1 = gtk_check_button_new_with_label("signal1");
            GtkWidget *checkSignal2 = gtk_check_button_new_with_label("signal2");
            GtkWidget *checkSignal3 = gtk_check_button_new_with_label("signal3");
            GtkWidget *checkSignal4 = gtk_check_button_new_with_label("signal4");
            GtkAdjustment *adjustment1 = gtk_adjustment_new(0.0, -3.3, 3.0, 0.1, 0.0, 0.0);
            GtkAdjustment *adjustment2 = gtk_adjustment_new(0.0, -3.3, 3.0, 0.1, 0.0, 0.0);
            GtkAdjustment *adjustment3 = gtk_adjustment_new(0.0, -3.3, 3.0, 0.1, 0.0, 0.0);
            GtkAdjustment *adjustment4 = gtk_adjustment_new(0.0, -3.3, 3.0, 0.1, 0.0, 0.0);
            GtkWidget *spinOffset1 = gtk_spin_button_new(adjustment1, 0.1, 2);
            GtkWidget *spinOffset2 = gtk_spin_button_new(adjustment2, 0.1, 2);
            GtkWidget *spinOffset3 = gtk_spin_button_new(adjustment3, 0.1, 2);
            GtkWidget *spinOffset4 = gtk_spin_button_new(adjustment4, 0.1, 2);
                gtk_grid_attach(GTK_GRID(gridSignals), checkSignal1, 0, 0, 1, 1);
                gtk_grid_attach(GTK_GRID(gridSignals), spinOffset1, 1, 0, 1, 1);
                gtk_grid_attach(GTK_GRID(gridSignals), checkSignal2, 0, 1, 1, 1);
                gtk_grid_attach(GTK_GRID(gridSignals), spinOffset2, 1, 1, 1, 1);
                gtk_grid_attach(GTK_GRID(gridSignals), checkSignal3, 0, 2, 1, 1);
                gtk_grid_attach(GTK_GRID(gridSignals), spinOffset3, 1, 2, 1, 1);
                gtk_grid_attach(GTK_GRID(gridSignals), checkSignal4, 0, 3, 1, 1);
                gtk_grid_attach(GTK_GRID(gridSignals), spinOffset4, 1, 3, 1, 1);

            GtkWidget *separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
            gtk_widget_set_vexpand(separator2, true);

            GtkWidget *boxFreq = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
            GtkAdjustment *adjustmentFreq = gtk_adjustment_new(1.0, 0.001, 100.0, 0.001, 1.0, 0.0);
            GtkWidget *spinFreq = gtk_spin_button_new(adjustmentFreq, 1.0, 3);
            GtkWidget *labelFreq = gtk_label_new("ms/div");
            GtkWidget *checkTestSignal = gtk_check_button_new_with_label("Test signal");

            GtkWidget *separator3 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
            gtk_widget_set_vexpand(separator3, true);

            GtkWidget *labelPort = gtk_label_new("Port");
            GtkWidget *boxPort = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
            screen->dropPort = gtk_drop_down_new(nullptr, nullptr);
            screen->buttonPort = gtk_button_new_with_label("Open");

    gtk_window_set_child(GTK_WINDOW(window), boxPanels);
    gtk_box_append(GTK_BOX(boxPanels), boxView);
    gtk_box_append(GTK_BOX(boxView), screen->glAreaVoltage);
    gtk_box_append(GTK_BOX(boxView), screen->glAreaSpectrum);
    gtk_box_append(GTK_BOX(boxView), windowScrool);
    gtk_box_append(GTK_BOX(boxPanels), boxControl);
    gtk_box_append(GTK_BOX(boxControl), buttonStartStop);
    gtk_box_append(GTK_BOX(boxControl), separator1);
    gtk_box_append(GTK_BOX(boxControl), dropVoltDiv);
    gtk_box_append(GTK_BOX(boxControl), gridSignals);
    gtk_box_append(GTK_BOX(boxControl), separator2);
    gtk_box_append(GTK_BOX(boxControl), boxFreq);
    gtk_box_append(GTK_BOX(boxFreq), spinFreq);
    gtk_box_append(GTK_BOX(boxFreq), labelFreq);
    gtk_box_append(GTK_BOX(boxControl), checkTestSignal);
    gtk_box_append(GTK_BOX(boxControl), separator3);
    gtk_box_append(GTK_BOX(boxControl), labelPort);
    gtk_box_append(GTK_BOX(boxControl), boxPort);
    gtk_box_append(GTK_BOX(boxPort), screen->dropPort);
    gtk_box_append(GTK_BOX(boxPort), screen->buttonPort);

    g_signal_connect(screen->glAreaVoltage, "realize", G_CALLBACK(realizeVoltage), userData);
    g_signal_connect(screen->glAreaVoltage, "unrealize", G_CALLBACK(unrealizeVoltage), userData);
    g_signal_connect(screen->glAreaVoltage, "render", G_CALLBACK(renderVoltage), userData);
    g_signal_connect(screen->glAreaVoltage, "resize", G_CALLBACK(resizeVoltage), userData);
    g_signal_connect(screen->glAreaSpectrum, "realize", G_CALLBACK(realizeSpectrum), userData);
    g_signal_connect(screen->glAreaSpectrum, "unrealize", G_CALLBACK(unrealizeSpectrum), userData);
    g_signal_connect(screen->glAreaSpectrum, "resize", G_CALLBACK(resize), userData);
    g_signal_connect(screen->glAreaSpectrum, "render", G_CALLBACK(renderSpectrum), userData);
    g_signal_connect(buttonStartStop, "clicked", G_CALLBACK(funcStartStop), userData);
    g_signal_connect(dropVoltDiv, "notify::selected", G_CALLBACK(funcVoltDiv), userData);
    g_signal_connect(checkSignal1, "toggled", G_CALLBACK(funcCheckSig1), userData);
    g_signal_connect(checkSignal2, "toggled", G_CALLBACK(funcCheckSig2), userData);
    g_signal_connect(checkSignal3, "toggled", G_CALLBACK(funcCheckSig3), userData);
    g_signal_connect(checkSignal4, "toggled", G_CALLBACK(funcCheckSig4), userData);
    g_signal_connect(spinOffset1, "value_changed", G_CALLBACK(funcOffset1), userData);
    g_signal_connect(spinOffset2, "value_changed", G_CALLBACK(funcOffset2), userData);
    g_signal_connect(spinOffset3, "value_changed", G_CALLBACK(funcOffset3), userData);
    g_signal_connect(spinOffset4, "value_changed", G_CALLBACK(funcOffset4), userData);
    g_signal_connect(spinFreq, "value_changed", G_CALLBACK(funcSpinButtonFreq), userData);
    g_signal_connect(checkTestSignal, "toggled", G_CALLBACK(funcCheckTestSignal), userData);
    g_signal_connect(screen->dropPort, "notify::selected", G_CALLBACK(funcDropPort), userData);
    g_signal_connect(screen->buttonPort, "clicked", G_CALLBACK(funcButtonPort), userData);
    g_signal_connect(window, "destroy", G_CALLBACK(destroyWindow), userData);

    GdkDisplay *display = gdk_display_get_default();
    GListModel *monitors = gdk_display_get_monitors(display);
    GdkMonitor *monitor = GDK_MONITOR(g_list_model_get_item(monitors, 0));
    GdkRectangle monitorGeometry;
    gdk_monitor_get_geometry(monitor, &monitorGeometry);

    gtk_window_set_title(GTK_WINDOW(window), "Oscilloscope");
    gtk_window_set_default_size(GTK_WINDOW(window), monitorGeometry.width, monitorGeometry.height);
    gtk_window_present(GTK_WINDOW(window));

    int udev_fd = udev_monitor_get_fd(screen->monitor);
    if(udev_fd >= 0){
        g_unix_fd_add(udev_fd, G_IO_IN, udev_monitor_callback, screen);
    }
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Screen::Screen()
    : appGtk(gtk_application_new("com.oscilloscope", G_APPLICATION_DEFAULT_FLAGS)){
    routePort.reserve(50);
    routePort.clear();
    createContextUdev();
    g_signal_connect(appGtk, "activate", G_CALLBACK(activate), this);
}

Screen::~Screen(){
    deleteContextUdev();
    if(appGtk){
        g_object_unref(appGtk);
    }
}

int Screen::show(int &argc, char** argv){
    return g_application_run(G_APPLICATION(appGtk), argc, argv);
}

void Screen::draw_signals() const{
    gtk_gl_area_queue_render(GTK_GL_AREA(glAreaVoltage));
    gtk_gl_area_queue_render(GTK_GL_AREA(glAreaSpectrum));
}

int Screen::createContextUdev(){
    //crear contexto udev
	udev = udev_new();
	if(!(udev)){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole, &end);
        gtk_text_buffer_insert(bufferConsole, &end, "No se pudo crear el contexto udev\n", -1);
		std::cerr << "No se pudo crear el contexto udev\n";
		return EXIT_FAILURE;
	}

	//Crear monitor para recibir eventos desde udev a través de netlink
	monitor = udev_monitor_new_from_netlink(udev, "udev");
	if(!(monitor)){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole, &end);
        gtk_text_buffer_insert(bufferConsole, &end, "No se pudo crear el monitor de udev\n", -1);
		std::cerr << "No se pudo crear el monitor de udev\n";
		udev_unref(udev);
		return EXIT_FAILURE;
	}

	//Filtrar eventos para el subsistema "tty"
	if(udev_monitor_filter_add_match_subsystem_devtype(monitor, "tty", nullptr) < 0){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole, &end);
        gtk_text_buffer_insert(bufferConsole, &end, "Error al aplicar el filtro para el subsistema tty\n", -1);
		std::cerr << "Error al aplicar el filtro para el subsistema tty\n";
		udev_monitor_unref(monitor);
		udev_unref(udev);
		return EXIT_FAILURE;
	}

	//Habilitar la recepcion de eventos
	if(udev_monitor_enable_receiving(monitor) < 0){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole, &end);
        gtk_text_buffer_insert(bufferConsole, &end, "Error al habilitar la recepcion de eventos\n", -1);
		std::cerr << "Error al habilitar la recepcion de eventos\n";
		udev_monitor_unref(monitor);
		udev_unref(udev);
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}

int Screen::deleteContextUdev(){
    if(monitor){
        udev_monitor_unref(monitor);
    }
    if(udev){
        udev_unref(udev);
    }

    return EXIT_SUCCESS;
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
bool Screen::createVAO(SignalObject &signalObject){
    glGenVertexArrays(1, &signalObject.vao);
    glGenBuffers(2, signalObject.vbo);

    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 2*signalObject.length*sizeof(float), signalObject.vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 3*signalObject.length*sizeof(float), signalObject.colorVertex, GL_STATIC_DRAW);

    glBindVertexArray(signalObject.vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole, &end);
        gtk_text_buffer_insert(bufferConsole, &end, "OpenGL Error in createVAO\n", -1);
        std::cerr << "OpenGL Error in createVAO: " << error << std::endl;
        return 1;
    }

    return 0;
}

bool Screen:: deleteVAO(SignalObject &signalObject){
    glDeleteVertexArrays(1, &signalObject.vao);
    glDeleteBuffers(2, signalObject.vbo);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole, &end);
        gtk_text_buffer_insert(bufferConsole, &end, "OpenGL Error in deleteVAO\n", -1);
        std::cerr << "OpenGL Error in deleteVAO: " << error << std::endl;
        return 1;
    }

    return 0;
}

bool Screen:: drawVAO(SignalObject &signalObject) const{
    glBindVertexArray(signalObject.vao);
    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 2*signalObject.length*sizeof(float), signalObject.vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glLineWidth(2);
    glBindVertexArray(signalObject.vao);
    glDrawArrays(GL_LINE_STRIP, 0, signalObject.length);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole, &end);
        gtk_text_buffer_insert(bufferConsole, &end, "OpenGL Error in drawVAO\n", -1);
        std::cerr << "OpenGL Error in drawVAO: " << error << std::endl;
        return 1;
    }

    return 0;
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      METHODS GRID_VOLTAGE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void GridVoltage::createGrid(){
    //Genera el VAO y el VBO
    glGenVertexArrays(1, &(this->vao));
    glGenBuffers(2, vbo);

    //Enlaza el VAO para configurar los atributos
    glBindVertexArray(vao);

    //Enlaza el VBO y copia los datos
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 56*sizeof(float), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 84*sizeof(float), color, GL_STATIC_DRAW);

    //Configurar atributos
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    //Desenlazar el VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Desenlazar el VAO
    glBindVertexArray(0);
}

void GridVoltage::deleteGrid(){
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);
}

void GridVoltage::draw(){
    glLineWidth(1);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, sizeof(vertex)/(2*sizeof(float)));
    glBindVertexArray(0);
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//      METHODS GRID_SPECTRUM
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void GridSpectrum::createGrid(){
    //Genera el VAO y el VBO
    glGenVertexArrays(1, &(vao));
    glGenBuffers(2, vbo);

    //Enlaza el VBO y copia los datos
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 32*sizeof(float), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 48*sizeof(float), color, GL_STATIC_DRAW);

    //Enlaza el VAO para configurar los atributos
    glBindVertexArray(vao);

    //Configurar atributos
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    //Desenlazar el VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Desenlazar el VAO
    glBindVertexArray(0);
}

void GridSpectrum::deleteGrid(){
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &(vao));
}

void GridSpectrum::draw(){
    glLineWidth(1);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, sizeof(vertex)/(2*sizeof(float)));
    glBindVertexArray(0);
}

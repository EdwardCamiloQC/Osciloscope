#include <iostream>
#include <glm/glm.hpp>

#include <sys/gui/screen.hpp>
#include <sys/shaders/shader.hpp>
#include <sys/oscilloscope.hpp>
#include <sys/peripherals/puertoSerie.hpp>
#include <sys/peripherals/signalGenerator.hpp>

//----------
//      CALLBACKS CANVA VOLTAGE
//----------
void realizeVoltage(GtkGLArea *area, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : realize" << std::endl;
        return;
    }

    ProgramShader shader("./src/sys/shaders/code.vs", "./src/sys/shaders/code.fs");
    screen->idShaderVolt = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    screen->gridVoltage.createGrid();
    screen->createVAO(osc->voltage1);
    screen->createVAO(osc->voltage2);
    screen->createVAO(osc->voltage3);
    screen->createVAO(osc->voltage4);
}

void unrealizeVoltage(GtkGLArea *area, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : unrealize" << std::endl; 
    }

    screen->gridVoltage.deleteGrid();
    screen->deleteVAO(osc->voltage1);
    screen->deleteVAO(osc->voltage2);
    screen->deleteVAO(osc->voltage3);
    screen->deleteVAO(osc->voltage4);

    glDeleteProgram(screen->idShaderVolt);
}

gboolean renderVoltage(GtkGLArea *area, GdkGLContext *context, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    gdk_gl_context_make_current(context);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : render" << std::endl;
        return FALSE;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(screen->idShaderVolt);
    // Verificar si el shader está en uso
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    if(screen->stateStartStop_){
        osc->voltage1.applyOffset(screen->offset1_, screen->voltDiv_);
        osc->voltage2.applyOffset(screen->offset2_, screen->voltDiv_);
        osc->voltage3.applyOffset(screen->offset3_, screen->voltDiv_);
        osc->voltage4.applyOffset(screen->offset4_, screen->voltDiv_);
    }
    if(static_cast<unsigned int>(currentProgram) == (screen->idShaderVolt) && (screen->idShaderVolt) != 0){
        //aqui dibuja
        screen->gridVoltage.draw();
        if(screen->stateSignal1_){
            screen->drawVAO(osc->voltage1);
        }
        if(screen->stateSignal2_){
            screen->drawVAO(osc->voltage2);
        }
        if(screen->stateSignal3_){
            screen->drawVAO(osc->voltage3);
        }
        if(screen->stateSignal4_){
            screen->drawVAO(osc->voltage4);
        }
    }

    // Verificar errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    glFlush();

    gtk_gl_area_attach_buffers(area);
    gtk_gl_area_queue_render(area);
    gtk_widget_queue_draw((GtkWidget*)area);
    return TRUE;
}

void resizeVoltage(GtkGLArea *area, gpointer userData){
    //
}

//----------
//      CALLBACKS CANVA SPECTRUM
//----------
static void realizeSpectrum(GtkGLArea *area, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : realize" << std::endl;
        return;
    }

    ProgramShader shader("./src/sys/shaders/code.vs", "./src/sys/shaders/code.fs");
    screen->idShaderSpec = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    screen->gridSpectrum.createGrid();

    //Aqui crea los espectros
    screen->createVAO(osc->voltage1.spectrumSignal);
    screen->createVAO(osc->voltage2.spectrumSignal);
    screen->createVAO(osc->voltage3.spectrumSignal);
    screen->createVAO(osc->voltage4.spectrumSignal);
}

static void unrealizeSpectrum(GtkGLArea *area, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : unrealize" << std::endl; 
    }

    screen->gridSpectrum.deleteGrid();
    //Aqui Elimina los spectros
    screen->deleteVAO(osc->voltage1.spectrumSignal);
    screen->deleteVAO(osc->voltage2.spectrumSignal);
    screen->deleteVAO(osc->voltage3.spectrumSignal);
    screen->deleteVAO(osc->voltage4.spectrumSignal);

    glDeleteProgram(screen->idShaderSpec);
}

static gboolean renderSpectrum(GtkGLArea *area, GdkGLContext *context, gpointer userData){
    Screen* screen = static_cast<Screen*>(userData);
    Oscilloscope *osc = Oscilloscope::getInstance();

    gtk_gl_area_make_current(area);
    gdk_gl_context_make_current(context);
    if(gtk_gl_area_get_error(area) != NULL){
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
    if(screen->stateStartStop_){
        osc->voltage1.spectrumSignal.updateVertex();
        osc->voltage2.spectrumSignal.updateVertex();
        osc->voltage3.spectrumSignal.updateVertex();
        osc->voltage4.spectrumSignal.updateVertex();
    }
    if(static_cast<unsigned int>(currentProgram) == (screen->idShaderSpec) && (screen->idShaderSpec) != 0){
        screen->gridSpectrum.draw();
        //aqui dibuja
        screen->gridVoltage.draw();
        if(screen->stateSignal1_){
            screen->drawVAO(osc->voltage1.spectrumSignal);
        }
        if(screen->stateSignal2_){
            screen->drawVAO(osc->voltage2.spectrumSignal);
        }
        if(screen->stateSignal3_){
            screen->drawVAO(osc->voltage3.spectrumSignal);
        }
        if(screen->stateSignal4_){
            screen->drawVAO(osc->voltage4.spectrumSignal);
        }
    }

    // Verificar errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    glFlush();

    gtk_gl_area_attach_buffers(area);
    gtk_gl_area_queue_render(area);
    gtk_widget_queue_draw((GtkWidget*)area);
    return TRUE;
}

static void resize(){
}

//----------
//      CALLBACKS CONTROL PANEL
//----------
void funcStartStop(GtkWidget *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    //GtkWidget *led = GTK_WIDGET(userData);
    //GtkStyleContext *context = gtk_widget_get_style_context(led);

    screen->stateStartStop_ = !(screen->stateStartStop_);
    if(screen->stateStartStop_){
        gtk_button_set_label(GTK_BUTTON(widget), "Stop");
        //gtk_style_context_remove_class(context, "led-on");
        //gtk_style_context_add_class(context, "led-off");
    }else{
        gtk_button_set_label(GTK_BUTTON(widget), "Start");
        //gtk_style_context_remove_class(context, "led-off");
        //gtk_style_context_add_class(context, "led-on");
    }
}

void funcVoltDiv(GtkComboBox *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    switch(gtk_combo_box_get_active(widget)){
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

void funcCheckSig1(GtkWidget *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->stateSignal1_ = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void funcCheckSig2(GtkWidget *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->stateSignal2_ = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void funcCheckSig3(GtkWidget *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->stateSignal3_ = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void funcCheckSig4(GtkWidget *widget, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->stateSignal4_ = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
}

void funcOffset1(GtkSpinButton *spinButton, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->offset1_ = gtk_spin_button_get_value(spinButton);
}

void funcOffset2(GtkSpinButton *spinButton, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->offset2_ = gtk_spin_button_get_value(spinButton);
}

void funcOffset3(GtkSpinButton *spinButton, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->offset3_ = gtk_spin_button_get_value(spinButton);
}

void funcOffset4(GtkSpinButton *spinButton, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->offset4_ = gtk_spin_button_get_value(spinButton);
}

void funcComboBoxFreq(GtkWidget *widget, gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();
    switch(gtk_combo_box_get_active(GTK_COMBO_BOX(widget))){
        case 0:
            osc->signalCapturer.setMultiplierFrequency(1);
            break;
        case 1:
            osc->signalCapturer.setMultiplierFrequency(1000);
            break;
        case 2:
            osc->signalCapturer.setMultiplierFrequency(1000000);
            break;
        default:
            osc->signalCapturer.setMultiplierFrequency(1);
    }
}

void funcSpinButtonFreq(GtkSpinButton *spinButton, gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();
    osc->signalCapturer.setSampleFrequency(gtk_spin_button_get_value_as_int(spinButton));
}

void funcCheckTestSignal(GtkWidget *widget, gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))){
        osc->signalCapturer.selectCapturer(std::make_unique<SignalGenerator>());
    }else{
        osc->signalCapturer.selectCapturer(std::make_unique<ComSerial>());
    }
}

void funcComboPort(GtkComboBoxText *widget, gpointer userData){
    //
}

void funcButtonPort(GtkWidget *widget, gpointer userData){
    //
}

//----------
//      WINDOW CALLBACKS
//----------
static void destroyWindow(GtkWidget *widget, gpointer userData){
    Oscilloscope *osc = Oscilloscope::getInstance();
    osc->stateOnOff_ = false;
}

static void activate(GtkApplication* app, gpointer userData){
    Screen *screen = static_cast<Screen*>(userData);
    screen->window = gtk_application_window_new(app);
    screen->boxPanels = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        screen->boxView = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
            screen->glAreaVoltage = gtk_gl_area_new();
            screen->glAreaSpectrum = gtk_gl_area_new();
            gtk_widget_set_size_request(screen->glAreaSpectrum, -1, 100);

        screen->boxControl = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
            screen->provider = gtk_css_provider_new();
            GError *error = nullptr;
            if(!gtk_css_provider_load_from_path(screen->provider, "./src/styles/styleButtonStartStop.css", &error)){
                g_warning("Error cargando CSS: %s", error->message);
                g_error_free(error);
            }
            gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(screen->provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
            //g_object_unref(provider);

            screen->led = gtk_drawing_area_new();
            gtk_widget_set_size_request(screen->led, 50, 50);
            screen->context = gtk_widget_get_style_context(screen->led);
            gtk_style_context_add_class(screen->context, "led-off");
            screen->buttonStartStop = gtk_button_new_with_label("Start");

            screen->separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

            screen->labelVoltDiv = gtk_label_new("Volt/Div");
            screen->comboVoltDiv = gtk_combo_box_text_new();
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "1", "0.1v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "2", "0.2v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "3", "0.3v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "4", "0.4v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "5", "0.5v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "6", "1v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "7", "2v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "8", "3v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "9", "4v/div");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboVoltDiv), "10", "5v/div");
                gtk_combo_box_set_active(GTK_COMBO_BOX(screen->comboVoltDiv), 5);
            screen->gridSignals = gtk_grid_new();
            screen->checkSignal1 = gtk_check_button_new_with_label("signal1");
            screen->checkSignal2 = gtk_check_button_new_with_label("signal2");
            screen->checkSignal3 = gtk_check_button_new_with_label("signal3");
            screen->checkSignal4 = gtk_check_button_new_with_label("signal4");
            screen->adjustment1 = gtk_adjustment_new(0.0, -3.3, 3.0, 0.1, 0.0, 0.0);
            screen->adjustment2 = gtk_adjustment_new(0.0, -3.3, 3.0, 0.1, 0.0, 0.0);
            screen->adjustment3 = gtk_adjustment_new(0.0, -3.3, 3.0, 0.1, 0.0, 0.0);
            screen->adjustment4 = gtk_adjustment_new(0.0, -3.3, 3.0, 0.1, 0.0, 0.0);
            screen->spinOffset1 = gtk_spin_button_new(screen->adjustment1, 0.1, 2);
            screen->spinOffset2 = gtk_spin_button_new(screen->adjustment2, 0.1, 2);
            screen->spinOffset3 = gtk_spin_button_new(screen->adjustment3, 0.1, 2);
            screen->spinOffset4 = gtk_spin_button_new(screen->adjustment4, 0.1, 2);
                gtk_grid_attach(GTK_GRID(screen->gridSignals), screen->checkSignal1, 0, 0, 1, 1);
                gtk_grid_attach(GTK_GRID(screen->gridSignals), screen->spinOffset1, 1, 0, 1, 1);
                gtk_grid_attach(GTK_GRID(screen->gridSignals), screen->checkSignal2, 0, 1, 1, 1);
                gtk_grid_attach(GTK_GRID(screen->gridSignals), screen->spinOffset2, 1, 1, 1, 1);
                gtk_grid_attach(GTK_GRID(screen->gridSignals), screen->checkSignal3, 0, 2, 1, 1);
                gtk_grid_attach(GTK_GRID(screen->gridSignals), screen->spinOffset3, 1, 2, 1, 1);
                gtk_grid_attach(GTK_GRID(screen->gridSignals), screen->checkSignal4, 0, 3, 1, 1);
                gtk_grid_attach(GTK_GRID(screen->gridSignals), screen->spinOffset4, 1, 3, 1, 1);

            screen->separator2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

            screen->labelFreq = gtk_label_new("Freq");
            screen->boxFreq = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
            screen->adjustmentFreq = gtk_adjustment_new(50.0, 1.0, 100.0, 1.0, 5.0, 0.0);
            screen->spinFreq = gtk_spin_button_new(screen->adjustmentFreq, 1.0, 0);
            screen->comboFreq = gtk_combo_box_text_new();
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboFreq), "1", "Hz");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboFreq), "2", "Khz");
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(screen->comboFreq), "3", "Mhz");
                gtk_combo_box_set_active(GTK_COMBO_BOX(screen->comboFreq), 0);

            screen->separator3 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

            screen->labelTestSignal = gtk_label_new("Test Signal");
            screen->checkTestSignal = gtk_check_button_new_with_label("Test signal");

            screen->labelPort = gtk_label_new("Port");
            screen->boxPort = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
            screen->dropPort = gtk_combo_box_text_new();
            screen->buttonPort = gtk_button_new_with_label("Open");

    gtk_container_add(GTK_CONTAINER(screen->window), screen->boxPanels);
    gtk_box_pack_start(GTK_BOX(screen->boxPanels), screen->boxView, true, true, 0);
    gtk_box_pack_start(GTK_BOX(screen->boxView), screen->glAreaVoltage, true, true, 0);
    gtk_box_pack_start(GTK_BOX(screen->boxView), screen->glAreaSpectrum, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxPanels), screen->boxControl, false, false, 1);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->buttonStartStop, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->separator1, true, true, 20);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->labelVoltDiv, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->comboVoltDiv, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->gridSignals, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->separator2, true, true, 20);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->labelFreq, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->boxFreq, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxFreq), screen->spinFreq, true, false, 1);
    gtk_box_pack_start(GTK_BOX(screen->boxFreq), screen->comboFreq, true, false, 1);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->separator3, true, true, 20);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->labelTestSignal, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->checkTestSignal, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->labelPort, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxControl), screen->boxPort, false, false, 5);
    gtk_box_pack_start(GTK_BOX(screen->boxPort), screen->dropPort, true, true, 1);
    gtk_box_pack_start(GTK_BOX(screen->boxPort), screen->buttonPort, false, true, 1);

    g_signal_connect(screen->glAreaVoltage, "realize", G_CALLBACK(realizeVoltage), userData);
    g_signal_connect(screen->glAreaVoltage, "unrealize", G_CALLBACK(unrealizeVoltage), userData);
    g_signal_connect(screen->glAreaVoltage, "render", G_CALLBACK(renderVoltage), userData);
    g_signal_connect(screen->glAreaVoltage, "resize", G_CALLBACK(resizeVoltage), userData);
    g_signal_connect(screen->glAreaSpectrum, "realize", G_CALLBACK(realizeSpectrum), userData);
    g_signal_connect(screen->glAreaSpectrum, "unrealize", G_CALLBACK(unrealizeSpectrum), userData);
    g_signal_connect(screen->glAreaSpectrum, "resize", G_CALLBACK(resize), userData);
    g_signal_connect(screen->glAreaSpectrum, "render", G_CALLBACK(renderSpectrum), userData);
    g_signal_connect(screen->buttonStartStop, "clicked", G_CALLBACK(funcStartStop), userData);
    g_signal_connect(screen->comboVoltDiv, "changed", G_CALLBACK(funcVoltDiv), userData);
    g_signal_connect(screen->checkSignal1, "clicked", G_CALLBACK(funcCheckSig1), userData);
    g_signal_connect(screen->checkSignal2, "clicked", G_CALLBACK(funcCheckSig2), userData);
    g_signal_connect(screen->checkSignal3, "clicked", G_CALLBACK(funcCheckSig3), userData);
    g_signal_connect(screen->checkSignal4, "clicked", G_CALLBACK(funcCheckSig4), userData);
    g_signal_connect(screen->spinOffset1, "value_changed", G_CALLBACK(funcOffset1), userData);
    g_signal_connect(screen->spinOffset2, "value_changed", G_CALLBACK(funcOffset2), userData);
    g_signal_connect(screen->spinOffset3, "value_changed", G_CALLBACK(funcOffset3), userData);
    g_signal_connect(screen->spinOffset4, "value_changed", G_CALLBACK(funcOffset4), userData);
    g_signal_connect(screen->comboFreq, "changed", G_CALLBACK(funcComboBoxFreq), userData);
    g_signal_connect(screen->spinFreq, "value_changed", G_CALLBACK(funcSpinButtonFreq), userData);
    g_signal_connect(screen->checkTestSignal, "clicked", G_CALLBACK(funcCheckTestSignal), userData);
    g_signal_connect(screen->dropPort, "changed", G_CALLBACK(funcComboPort), nullptr);
    g_signal_connect(screen->buttonPort, "clicked", G_CALLBACK(funcButtonPort), nullptr);
    g_signal_connect(screen->window, "destroy", G_CALLBACK(destroyWindow), nullptr);

    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *monitor = gdk_display_get_monitor(display, 0);
    GdkRectangle monitorGeometry;
    gdk_monitor_get_geometry(monitor, &monitorGeometry);

    gtk_window_set_title(GTK_WINDOW(screen->window), "Oscilloscope");
    gtk_window_set_default_size(GTK_WINDOW(screen->window), monitorGeometry.width, 700);
    gtk_window_present(GTK_WINDOW(screen->window));
    gtk_widget_show_all(screen->window);

    //fla
    //g_thread_new("TTY Devices", inspectionTtyDevices, userData);
}

//----------
//      PUBLIC METHODS
//----------
Screen::Screen()
    : appGtk(gtk_application_new("com.oscilloscope", G_APPLICATION_DEFAULT_FLAGS)){
    g_signal_connect(appGtk, "activate", G_CALLBACK(activate), this);
}

Screen::~Screen(){
    if(appGtk){
        g_object_unref(appGtk);
    }
}

int Screen::show(int &argc, char** argv){
    return g_application_run(G_APPLICATION(appGtk), argc, argv);
}

//----------
//      PRIVATE METHODS
//----------
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
        std::cerr << "OpenGL Error in drawVAO: " << error << std::endl;
        return 1;
    }

    return 0;
}

//----------
//      METHODS GRID_VOLTAGE
//----------
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

//----------
//      METHODS GRID_SPECTRUM
//----------
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

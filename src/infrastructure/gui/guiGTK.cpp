#include <iostream>

#include <infrastructure/gui/guiGTK.hpp>
#include <application/oscilloscope.hpp>
#include <infrastructure/shaderCompiler/shader.hpp>
#include <entities/VAOs/vaoObject.hpp>

using namespace GUI_GTK;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DEFINES
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
#define WIDTH_WIDGET 150
#define WIDTH_LENGHT 30

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
GuiGtk::GuiGtk(){
    appGtkPtr_ = gtk_application_new("com.ed.oscilloscope", G_APPLICATION_DEFAULT_FLAGS);
}

GuiGtk::~GuiGtk(){
    if(appGtkPtr_)
        g_object_unref(appGtkPtr_);
}

int GuiGtk::init(int &argc, char **&argv){
    g_signal_connect(appGtkPtr_, "activate", G_CALLBACK(construct_window_callback), this);
    return g_application_run(G_APPLICATION(appGtkPtr_), argc, argv);
}

void GuiGtk::update_drop_port(const bool add, const char *text){
    GtkDropDown *dropPort = GTK_DROP_DOWN(dropPortPtr_);
    GListModel *model = gtk_drop_down_get_model(dropPort);

    if(model == nullptr){
        if(add){
            gtk_string_list_append(deviceListPortPtr_, text);
            //gtk_drop_down_set_model(dropPort, G_LIST_MODEL(newList));
            //g_object_unref(newList);//eee
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
                if(!add){
                    gtk_string_list_remove(deviceListPortPtr_, i);
                }
            }
        }
        g_object_unref(item);
    }

    if(foundIndex == -1){
        if(add){
            gtk_string_list_append(deviceListPortPtr_, text);
        }
    }
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void GuiGtk::construct_window_callback(GtkApplication* appPt, gpointer userData){
    auto *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    GtkCssProvider *providerCssWindowPt = gtk_css_provider_new();
    GtkCssProvider *providerCssGlAreasPt = gtk_css_provider_new();
    GtkCssProvider *providerCssConsolePt = gtk_css_provider_new();
    GtkCssProvider *providerCssButtonsPt = gtk_css_provider_new();
    GtkCssProvider *providerCssChecksPt = gtk_css_provider_new();
    GtkCssProvider *providerCssLabelsPt = gtk_css_provider_new();
    GtkCssProvider *providerCssSpinButtonsPt = gtk_css_provider_new();
    GtkCssProvider *providerCssDropsPt = gtk_css_provider_new();

    GFile *fileCssPt = g_file_new_for_path("./resources/styles/window.css"); 
    gtk_css_provider_load_from_file(providerCssWindowPt, fileCssPt);
    g_object_unref(fileCssPt);

    fileCssPt = g_file_new_for_path("./resources/styles/glArea.css"); 
    gtk_css_provider_load_from_file(providerCssGlAreasPt, fileCssPt);
    g_object_unref(fileCssPt);

    fileCssPt = g_file_new_for_path("./resources/styles/console.css"); 
    gtk_css_provider_load_from_file(providerCssConsolePt, fileCssPt);
    g_object_unref(fileCssPt);

    fileCssPt = g_file_new_for_path("./resources/styles/buttons.css"); 
    gtk_css_provider_load_from_file(providerCssButtonsPt, fileCssPt);
    g_object_unref(fileCssPt);

    fileCssPt = g_file_new_for_path("./resources/styles/checkButtons.css"); 
    gtk_css_provider_load_from_file(providerCssChecksPt, fileCssPt);
    g_object_unref(fileCssPt);

    fileCssPt = g_file_new_for_path("./resources/styles/labels.css"); 
    gtk_css_provider_load_from_file(providerCssLabelsPt, fileCssPt);
    g_object_unref(fileCssPt);

    fileCssPt = g_file_new_for_path("./resources/styles/spinButtons.css"); 
    gtk_css_provider_load_from_file(providerCssSpinButtonsPt, fileCssPt);
    g_object_unref(fileCssPt);

    fileCssPt = g_file_new_for_path("./resources/styles/drops.css"); 
    gtk_css_provider_load_from_file(providerCssDropsPt, fileCssPt);
    g_object_unref(fileCssPt);

    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(providerCssWindowPt), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(providerCssGlAreasPt), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(providerCssConsolePt), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(providerCssButtonsPt), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(providerCssChecksPt), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(providerCssLabelsPt), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(providerCssSpinButtonsPt), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(providerCssDropsPt), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *windowPt = gtk_application_window_new(appPt);
    gtk_widget_add_css_class(windowPt, "window");
    gtk_window_set_title(GTK_WINDOW(windowPt), "Oscilloscope");
    GdkDisplay *displayPt = gdk_display_get_default();
    GListModel *monitorsPt = gdk_display_get_monitors(displayPt);
    GdkMonitor *monitorPt = GDK_MONITOR(g_list_model_get_item(monitorsPt, 0));
    GdkRectangle monitorGeometry;
    gdk_monitor_get_geometry(monitorPt, &monitorGeometry);
    int width = monitorGeometry.width;
    int height = monitorGeometry.height;
    gtk_window_set_default_size(GTK_WINDOW(windowPt), width, height);

    int widthCol1 = WIDTH_WIDGET;
    int widthCol0 = width - 100 - WIDTH_WIDGET * 2;

        GtkWidget *gridGeneralPt = gtk_grid_new();
        gtk_grid_set_row_spacing(GTK_GRID(gridGeneralPt), 10);
        gtk_grid_set_column_spacing(GTK_GRID(gridGeneralPt), 5);
        gtk_widget_set_halign(gridGeneralPt, GTK_ALIGN_CENTER);
        gtk_widget_set_valign(gridGeneralPt, GTK_ALIGN_CENTER);

            glAreaVoltagePtr_ = gtk_gl_area_new();
            gtk_widget_set_size_request(glAreaVoltagePtr_, widthCol0, static_cast<int>(height*5/8));
            gtk_widget_add_css_class(glAreaVoltagePtr_, "gl-area1");

            glAreaSpectrumPtr_ = gtk_gl_area_new();
            gtk_widget_set_size_request(glAreaSpectrumPtr_, widthCol0, static_cast<int>(height/8));
            gtk_widget_add_css_class(glAreaSpectrumPtr_, "gl-area2");

            GtkTextTagTable *tagTableConsolePt = gtk_text_tag_table_new();
            bufferConsolePtr_ = gtk_text_buffer_new(tagTableConsolePt);
            GtkWidget *appConsolePt = gtk_text_view_new_with_buffer(bufferConsolePtr_);
            gtk_text_view_set_editable(GTK_TEXT_VIEW(appConsolePt), false);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appConsolePt), false);
            gtk_text_view_set_monospace(GTK_TEXT_VIEW(appConsolePt), true);
            GtkWidget *windowScroolPt = gtk_scrolled_window_new();
            gtk_widget_set_size_request(windowScroolPt, widthCol1, static_cast<int>(height/6));
            gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(windowScroolPt), appConsolePt);
            gtk_widget_add_css_class(windowScroolPt, "consoleTerminal");

            GtkTextTag *tagErrorPt = gtk_text_tag_new("error");
            g_object_set(tagErrorPt, "foreground", "red", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagErrorPt);

            GtkTextTag *tagOpenPt = gtk_text_tag_new("open");
            g_object_set(tagOpenPt, "foreground", "green", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagOpenPt);

            GtkTextTag *tagClosePt = gtk_text_tag_new("close");
            g_object_set(tagClosePt, "foreground", "blue", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagClosePt);

            GtkTextTag *tagNormalPt = gtk_text_tag_new("normal");
            g_object_set(tagNormalPt, "foreground", "black", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagNormalPt);

            GtkWidget *buttonStartStopPt = gtk_button_new_with_label("Start");
            gtk_widget_set_size_request(buttonStartStopPt, widthCol1, 20);
            gtk_widget_add_css_class(buttonStartStopPt, "led-off");

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
            GtkStringList *listVoltDivPt = gtk_string_list_new(optionsVolDiv);
            GtkWidget *dropVoltDivPt = gtk_drop_down_new(G_LIST_MODEL(listVoltDivPt), nullptr);
            gtk_widget_set_size_request(dropVoltDivPt, widthCol1, 20);
            gtk_drop_down_set_selected(GTK_DROP_DOWN(dropVoltDivPt), 5);
            gtk_widget_add_css_class(dropVoltDivPt, "drops");

            GtkWidget *gridSignalsPt = gtk_grid_new();
            gtk_grid_set_row_spacing(GTK_GRID(gridSignalsPt), 10);
            gtk_grid_set_column_spacing(GTK_GRID(gridSignalsPt), 5);

                GtkWidget *checkSignal1Pt = gtk_check_button_new_with_label("signal1");
                gtk_widget_set_size_request(checkSignal1Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(checkSignal1Pt, "check");

                GtkWidget *checkSignal2Pt = gtk_check_button_new_with_label("signal2");
                gtk_widget_set_size_request(checkSignal2Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(checkSignal2Pt, "check");

                GtkWidget *checkSignal3Pt = gtk_check_button_new_with_label("signal3");
                gtk_widget_set_size_request(checkSignal3Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(checkSignal3Pt, "check");

                GtkWidget *checkSignal4Pt = gtk_check_button_new_with_label("signal4");
                gtk_widget_set_size_request(checkSignal4Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(checkSignal4Pt, "check");

                GtkAdjustment *adjustment1Pt = gtk_adjustment_new(0.0, -10.0, 10.0, 0.1, 0.0, 0.0);
                GtkAdjustment *adjustment2Pt = gtk_adjustment_new(0.0, -10.0, 10.0, 0.1, 0.0, 0.0);
                GtkAdjustment *adjustment3Pt = gtk_adjustment_new(0.0, -10.0, 10.0, 0.1, 0.0, 0.0);
                GtkAdjustment *adjustment4Pt = gtk_adjustment_new(0.0, -10.0, 10.0, 0.1, 0.0, 0.0);

                GtkWidget *spinOffset1Pt = gtk_spin_button_new(adjustment1Pt, 0.1, 2);
                gtk_widget_set_size_request(spinOffset1Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinOffset1Pt, "changeValues");

                GtkWidget *spinOffset2Pt = gtk_spin_button_new(adjustment2Pt, 0.1, 2);
                gtk_widget_set_size_request(spinOffset2Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinOffset2Pt, "changeValues");

                GtkWidget *spinOffset3Pt = gtk_spin_button_new(adjustment3Pt, 0.1, 2);
                gtk_widget_set_size_request(spinOffset3Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinOffset3Pt, "changeValues");

                GtkWidget *spinOffset4Pt = gtk_spin_button_new(adjustment4Pt, 0.1, 2);
                gtk_widget_set_size_request(spinOffset4Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinOffset4Pt, "changeValues");

                GtkAdjustment *adjustmentFreqPt = gtk_adjustment_new(1.0, 0.001, 100.0, 0.001, 1.0, 0.0);
                GtkWidget *spinFreqPt = gtk_spin_button_new(adjustmentFreqPt, 1.0, 3);
                gtk_widget_set_size_request(spinFreqPt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinFreqPt, "changeValues");

                GtkWidget *labelFreqPt = gtk_label_new("ms/div");
                gtk_widget_set_size_request(labelFreqPt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(labelFreqPt, "indicators");

            gtk_grid_attach(GTK_GRID(gridSignalsPt), checkSignal1Pt,    0, 0, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), spinOffset1Pt,     1, 0, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), checkSignal2Pt,    0, 1, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), spinOffset2Pt,     1, 1, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), checkSignal3Pt,    0, 2, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), spinOffset3Pt,     1, 2, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), checkSignal4Pt,    0, 3, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), spinOffset4Pt,     1, 3, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), spinFreqPt,        0, 4, 1, 1);
            gtk_grid_attach(GTK_GRID(gridSignalsPt), labelFreqPt,       1, 4, 1, 1);

            GtkWidget *labelPortPt = gtk_label_new("SERIAL PORT");
            gtk_widget_set_size_request(labelPortPt, widthCol1, WIDTH_LENGHT);
            gtk_widget_add_css_class(labelPortPt, "titles");

            GtkWidget *boxPortPt = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

                deviceListPortPtr_ = gtk_string_list_new(nullptr);
                dropPortPtr_ = gtk_drop_down_new(G_LIST_MODEL(deviceListPortPtr_), nullptr);
                gtk_widget_set_size_request(dropPortPtr_, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(dropPortPtr_, "dropdown");

                buttonPortPtr_ = gtk_button_new_with_label("Open");
                gtk_widget_set_size_request(buttonPortPtr_, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(buttonPortPtr_, "port-closed");

            gtk_box_append(GTK_BOX(boxPortPt), dropPortPtr_);
            gtk_box_append(GTK_BOX(boxPortPt), buttonPortPtr_);

        gtk_grid_attach(GTK_GRID(gridGeneralPt), glAreaVoltagePtr_,  0, 0, 1, 4);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), glAreaSpectrumPtr_, 0, 4, 1, 2);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), buttonStartStopPt, 1, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), dropVoltDivPt,     1, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), gridSignalsPt,     1, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), windowScroolPt,    1, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), labelPortPt,       1, 4, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), boxPortPt,         1, 5, 1, 1);

    gtk_window_set_child(GTK_WINDOW(windowPt), gridGeneralPt);

    GtkGesture *clickPt = gtk_gesture_click_new();
    gtk_widget_add_controller(glAreaSpectrumPtr_, GTK_EVENT_CONTROLLER(clickPt));

    g_signal_connect(glAreaVoltagePtr_,  "realize",          G_CALLBACK(realize_voltage_callback),    userData);
    g_signal_connect(glAreaVoltagePtr_,  "unrealize",        G_CALLBACK(unrealize_voltage_callback),  userData);
    g_signal_connect(glAreaVoltagePtr_,  "render",           G_CALLBACK(render_voltage_callback),     userData);
    g_signal_connect(glAreaVoltagePtr_,  "resize",           G_CALLBACK(resize_voltage_callback),     userData);
    g_signal_connect(glAreaSpectrumPtr_, "realize",          G_CALLBACK(realize_spectrum_callback),   userData);
    g_signal_connect(glAreaSpectrumPtr_, "unrealize",        G_CALLBACK(unrealize_spectrum_callback), userData);
    g_signal_connect(glAreaSpectrumPtr_, "resize",           G_CALLBACK(resize_spectrum_callback),    userData);
    g_signal_connect(glAreaSpectrumPtr_, "render",           G_CALLBACK(render_spectrum_callback),    userData);
    g_signal_connect(buttonStartStopPt,  "clicked",          G_CALLBACK(startStop_callback),          userData);
    g_signal_connect(dropVoltDivPt,      "notify::selected", G_CALLBACK(voltDiv_callback),            userData);
    g_signal_connect(checkSignal1Pt,     "toggled",          G_CALLBACK(check_signal1_callback),      userData);
    g_signal_connect(checkSignal2Pt,     "toggled",          G_CALLBACK(check_signal2_callback),      userData);
    g_signal_connect(checkSignal3Pt,     "toggled",          G_CALLBACK(check_signal3_callback),      userData);
    g_signal_connect(checkSignal4Pt,     "toggled",          G_CALLBACK(check_signal4_callback),      userData);
    g_signal_connect(spinOffset1Pt,      "value_changed",    G_CALLBACK(offset_signal1_callback),     userData);
    g_signal_connect(spinOffset2Pt,      "value_changed",    G_CALLBACK(offset_signal2_callback),     userData);
    g_signal_connect(spinOffset3Pt,      "value_changed",    G_CALLBACK(offset_signal3_callback),     userData);
    g_signal_connect(spinOffset4Pt,      "value_changed",    G_CALLBACK(offset_signal4_callback),     userData);
    g_signal_connect(spinFreqPt,         "value_changed",    G_CALLBACK(spin_button_freq_callback),   userData);
    g_signal_connect(dropPortPtr_,       "notify::selected", G_CALLBACK(drop_port_callback),          userData);
    g_signal_connect(buttonPortPtr_,     "clicked",          G_CALLBACK(button_port_callback),        userData);
    g_signal_connect(windowPt,           "destroy",          G_CALLBACK(destroy_window_callback),     userData);
    g_signal_connect(clickPt,            "pressed",          G_CALLBACK(click_callback),              userData);

    gtk_window_present(GTK_WINDOW(windowPt));

    osc->stateOnOff_.store(true, std::memory_order_release);

    osc->signalCapturer_.init(userData);

    osc->devInspector_.init();

    drawTimeOutID_ = g_timeout_add(16, render, userData);
}

void GuiGtk::destroy_window_callback([[maybe_unused]]GtkWidget *widget, [[maybe_unused]]gpointer userData){
    auto *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    osc->stateOnOff_.store(false, std::memory_order_release);

    osc->devInspector_.stop();

    if(drawTimeOutID_){
        g_source_remove(drawTimeOutID_);
        drawTimeOutID_ = 0;
    }
}

void GuiGtk::realize_voltage_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    auto *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : realize" << std::endl;
        return;
    }

    ProgramShader shader("./resources/shaders/codeVertexES.vs", "./resources/shaders/codeFragmentES.fs");
    idShaderVolt_ = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    gridVoltage_.createGrid();
    VAOS_OBJECTS::VaoObject::createVAO(osc->voltage1_);
    VAOS_OBJECTS::VaoObject::createVAO(osc->voltage2_);
    VAOS_OBJECTS::VaoObject::createVAO(osc->voltage3_);
    VAOS_OBJECTS::VaoObject::createVAO(osc->voltage4_);
}

void GuiGtk::unrealize_voltage_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    auto *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : unrealize" << std::endl; 
    }

    gridVoltage_.deleteGrid();
    VAOS_OBJECTS::VaoObject::deleteVAO(osc->voltage1_);
    VAOS_OBJECTS::VaoObject::deleteVAO(osc->voltage2_);
    VAOS_OBJECTS::VaoObject::deleteVAO(osc->voltage3_);
    VAOS_OBJECTS::VaoObject::deleteVAO(osc->voltage4_);

    glDeleteProgram(idShaderVolt_);
}

gboolean GuiGtk::render_voltage_callback(GtkGLArea *area, [[maybe_unused]]GdkGLContext *context, [[maybe_unused]]gpointer userData){
    auto *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : render" << std::endl;
        return FALSE;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(idShaderVolt_ != 0){
        glUseProgram(idShaderVolt_);
        //aqui dibuja
        gridVoltage_.draw();

        //if(stateStartStop_){
            {
                std::lock_guard<std::mutex> lock(osc->mutex_);

                if(stateSignal1_){
                    osc->voltage1_.apply_offset(offset1_, voltDiv_, stateStartStop_);
                    VAOS_OBJECTS::VaoObject::drawVAO(osc->voltage1_);
                }
                if(stateSignal2_){
                    osc->voltage2_.apply_offset(offset2_, voltDiv_, stateStartStop_);
                    VAOS_OBJECTS::VaoObject::drawVAO(osc->voltage2_);
                }
                if(stateSignal3_){
                    osc->voltage3_.apply_offset(offset3_, voltDiv_, stateStartStop_);
                    VAOS_OBJECTS::VaoObject::drawVAO(osc->voltage3_);
                }
                if(stateSignal4_){
                    osc->voltage4_.apply_offset(offset4_, voltDiv_, stateStartStop_);
                    VAOS_OBJECTS::VaoObject::drawVAO(osc->voltage4_);
                }
            }
        //}
    }

    // Verificar errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    return TRUE;
}

void GuiGtk::resize_voltage_callback([[maybe_unused]]GtkGLArea *area, [[maybe_unused]]gpointer userData){
    //glViewport(0, 0, width, height);
}

void GuiGtk::realize_spectrum_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    OSCILLOSCOPE::Oscilloscope *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    gtk_gl_area_make_current(area);

    if(!gtk_gl_area_get_context(area)){
        std::cerr << "No OpenGL context available\n";
        return;
    }

    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : realize" << std::endl;
        return;
    }

    ProgramShader shader("./resources/shaders/codeVertexES.vs", "./resources/shaders/codeFragmentES.fs");
    idShaderSpec_ = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    gridSpectrum_.createGrid();

    //Aqui crea los espectros
    VAOS_OBJECTS::VaoObject::createVAO(osc->voltage1_.spectrumSignal_);
    VAOS_OBJECTS::VaoObject::createVAO(osc->voltage2_.spectrumSignal_);
    VAOS_OBJECTS::VaoObject::createVAO(osc->voltage3_.spectrumSignal_);
    VAOS_OBJECTS::VaoObject::createVAO(osc->voltage4_.spectrumSignal_);
}

void GuiGtk::unrealize_spectrum_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    OSCILLOSCOPE::Oscilloscope *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : unrealize" << std::endl; 
    }

    gridSpectrum_.deleteGrid();
    //Aqui Elimina los spectros
    VAOS_OBJECTS::VaoObject::deleteVAO(osc->voltage1_.spectrumSignal_);
    VAOS_OBJECTS::VaoObject::deleteVAO(osc->voltage2_.spectrumSignal_);
    VAOS_OBJECTS::VaoObject::deleteVAO(osc->voltage3_.spectrumSignal_);
    VAOS_OBJECTS::VaoObject::deleteVAO(osc->voltage4_.spectrumSignal_);

    glDeleteProgram(idShaderSpec_);
}

gboolean GuiGtk::render_spectrum_callback(GtkGLArea *area, [[maybe_unused]]GdkGLContext *context, [[maybe_unused]]gpointer userData){
    OSCILLOSCOPE::Oscilloscope *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : render" << std::endl;
        return false;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(idShaderSpec_ != 0){
        glUseProgram(idShaderSpec_);

        gridSpectrum_.draw();

        {
            std::lock_guard<std::mutex> lock(osc->mutex_);

            if(stateSignal1_){
                osc->voltage1_.calculate_spectrum();
                osc->voltage1_.spectrumSignal_.update_vertex();
                VAOS_OBJECTS::VaoObject::drawVAO(osc->voltage1_.spectrumSignal_);
            }
            if(stateSignal2_){
                osc->voltage2_.calculate_spectrum();
                osc->voltage2_.spectrumSignal_.update_vertex();
                VAOS_OBJECTS::VaoObject::drawVAO(osc->voltage2_.spectrumSignal_);
            }
            if(stateSignal3_){
                osc->voltage3_.calculate_spectrum();
                osc->voltage3_.spectrumSignal_.update_vertex();
                VAOS_OBJECTS::VaoObject::drawVAO(osc->voltage3_.spectrumSignal_);
            }
            if(stateSignal4_){
                osc->voltage4_.calculate_spectrum();
                osc->voltage4_.spectrumSignal_.update_vertex();
                VAOS_OBJECTS::VaoObject::drawVAO(osc->voltage4_.spectrumSignal_);
            }
        }
    }

    // Verificar errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    return true;
}

void GuiGtk::resize_spectrum_callback(){
    //
}

void GuiGtk::startStop_callback(GtkWidget *widget, [[maybe_unused]]gpointer userData){
    stateStartStop_ = !stateStartStop_;
    if(stateStartStop_){
        gtk_button_set_label(GTK_BUTTON(widget), "Stop");
        gtk_widget_remove_css_class(widget, "led-off");
        gtk_widget_add_css_class(widget, "led-on");
    }else{
        gtk_button_set_label(GTK_BUTTON(widget), "Start");
        gtk_widget_remove_css_class(widget, "led-on");
        gtk_widget_add_css_class(widget, "led-off");
    }
}

void GuiGtk::voltDiv_callback(GtkDropDown *widget, [[maybe_unused]]gpointer userData){
    switch(gtk_drop_down_get_selected(widget)){
        case 0:
            voltDiv_ = 0.1f;
            break;
        case 1:
            voltDiv_ = 0.2f;
            break;
        case 2:
            voltDiv_ = 0.3f;
            break;
        case 3:
            voltDiv_ = 0.4f;
            break;
        case 4:
            voltDiv_ = 0.5f;
            break;
        case 5:
            voltDiv_ = 1.0f;
            break;
        case 6:
            voltDiv_ = 2.0f;
            break;
        case 7:
            voltDiv_ = 3.0f;
            break;
        case 8:
            voltDiv_ = 4.0f;
            break;
        case 9:
            voltDiv_ = 5.0f;
            break;
        default:
            voltDiv_ = 1.0f;
    }
}

void GuiGtk::check_signal1_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    stateSignal1_ = gtk_check_button_get_active(widget);
}

void GuiGtk::check_signal2_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    stateSignal2_ = gtk_check_button_get_active(widget);
}

void GuiGtk::check_signal3_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    stateSignal3_ = gtk_check_button_get_active(widget);
}

void GuiGtk::check_signal4_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    stateSignal4_ = gtk_check_button_get_active(widget);
}

void GuiGtk::offset_signal1_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    offset1_ = gtk_spin_button_get_value(spinButton);
}

void GuiGtk::offset_signal2_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    offset2_ = gtk_spin_button_get_value(spinButton);
}

void GuiGtk::offset_signal3_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    offset3_ = gtk_spin_button_get_value(spinButton);
}

void GuiGtk::offset_signal4_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    offset4_ = gtk_spin_button_get_value(spinButton);
}

void GuiGtk::spin_button_freq_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    timeDiv_.store(gtk_spin_button_get_value(spinButton), std::memory_order_release);
}

void GuiGtk::drop_port_callback(GtkDropDown *widget, [[maybe_unused]]gpointer userData){
    GObject *obj = reinterpret_cast<GObject*>(gtk_drop_down_get_selected_item(widget));

    if(!obj)
        return;

    const char *text = gtk_string_object_get_string(GTK_STRING_OBJECT(obj));

    if(text){
        std::cout << text << std::endl;
        routePort_ = text;
    }
}

void GuiGtk::button_port_callback(GtkWidget *widget, [[maybe_unused]]gpointer userData){
    OSCILLOSCOPE::Oscilloscope *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    if(osc->signalCapturer_.capturer_->get_flag_serial()){
        osc->signalCapturer_.capturer_->close_port();

        if(!(osc->signalCapturer_.capturer_->get_flag_serial())){
            gtk_button_set_label(GTK_BUTTON(widget), "Open");
            GtkTextIter end;
            gtk_text_buffer_get_end_iter(bufferConsolePtr_, &end);
            gtk_text_buffer_insert_with_tags_by_name(bufferConsolePtr_, &end, "Puerto serial cerrado\n", -1, "close", NULL);
        }
    }else{
        osc->signalCapturer_.capturer_->open_port(routePort_.c_str());

        if(osc->signalCapturer_.capturer_->get_flag_serial()){
            gtk_button_set_label(GTK_BUTTON(widget), "Close");
            GtkTextIter end;
            gtk_text_buffer_get_end_iter(bufferConsolePtr_, &end);
            gtk_text_buffer_insert_with_tags_by_name(bufferConsolePtr_, &end, "Puerto serial abierto\n", -1, "open", NULL);
        }
    }
}

void GuiGtk::click_callback([[maybe_unused]]GtkGestureClick *gesture, [[maybe_unused]]int nPress, double x, [[maybe_unused]]double y, [[maybe_unused]]gpointer userData){
    OSCILLOSCOPE::Oscilloscope *osc = OSCILLOSCOPE::Oscilloscope::get_instance();

    int width = gtk_widget_get_width(glAreaSpectrumPtr_);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Frecuencia: %.1fHz\n", x*osc->LENGTH_VOLT_SIGNAL*1000/(timeDiv_.load(std::memory_order_acquire)*width*8*2*3.141516));

    GtkTextIter end;
    gtk_text_buffer_get_end_iter(bufferConsolePtr_, &end);
    gtk_text_buffer_insert_with_tags_by_name(bufferConsolePtr_, &end, buffer, -1, "normal", NULL);
}

gboolean GuiGtk::render([[maybe_unused]]gpointer userData){
    if(!appGtkPtr_){
        return G_SOURCE_REMOVE;
    }

    if(GTK_IS_GL_AREA(glAreaVoltagePtr_)){
        gtk_gl_area_queue_render(GTK_GL_AREA(glAreaVoltagePtr_));
    }

    if(GTK_IS_GL_AREA(glAreaSpectrumPtr_)){
        gtk_gl_area_queue_render(GTK_GL_AREA(glAreaSpectrumPtr_));
    }

    return G_SOURCE_CONTINUE;
}

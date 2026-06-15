#include <iostream>
#include "frameworksAndDrivers/gui/guiGTK.hpp"
#include "application/ISignalCapturer.hpp"
#include "application/IDevInspector.hpp"
#include "application/IDocGenerator.hpp"
#include "domain/signals/voltageSignal.hpp"
#include "frameworksAndDrivers/shaderCompiler/shader.hpp"
#include "domain/VAOs/vaoObject.hpp"

using namespace DRV_FRAMW;

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
GuiGtk& GuiGtk::get_instance(){
    static GuiGtk instance;
    return instance;
}

void GuiGtk::associate_signal_capturer(APP::ISignalCapturer* sigCapPtr){
    signalCapturerPtr_ = sigCapPtr;
}

void GuiGtk::associate_dev_inspector(APP::IDevInspector* devInspPtr){
    devInspectorPtr_ = devInspPtr;
}

void GuiGtk::associate_doc_generator(APP::IDocGenerator* docGenPtr){
    docGeneratorPtr_ = docGenPtr;
}

void GuiGtk::associate_voltages(DOMN::VoltageSignal* voltsPtr){
    voltagesPtr_ = voltsPtr;
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

void GuiGtk::display_message(const char* msg, int type) const{
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(bufferConsolePtr_, &end);
    const char* typeMsg = nullptr;

    switch(type){
        case 0:
            typeMsg = "in";
            break;
        case 1:
            typeMsg = "normal";
            break;
        case 2:
            typeMsg = "error";
            break;
        case 3:
            typeMsg = "warning";
            break;
        case 4:
            typeMsg = "open";
            break;
        case 5:
            typeMsg = "close";
            break;
        default:
            typeMsg = "normal";
    }
    gtk_text_buffer_insert_with_tags_by_name(bufferConsolePtr_, &end, msg, -1, typeMsg, NULL);
}

void GuiGtk::display_message_static(const char* msg, int type){
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(bufferConsolePtr_, &end);
    const char* typeMsg = nullptr;

    switch(type){
        case 0:
            typeMsg = "in";
            break;
        case 1:
            typeMsg = "normal";
            break;
        case 2:
            typeMsg = "error";
            break;
        case 3:
            typeMsg = "warning";
            break;
        case 4:
            typeMsg = "open";
            break;
        case 5:
            typeMsg = "close";
            break;
        default:
            typeMsg = "normal";
    }
    gtk_text_buffer_insert_with_tags_by_name(bufferConsolePtr_, &end, msg, -1, typeMsg, NULL);
}

void GuiGtk::update_label_button_port(bool open){
    if(open){
        gtk_button_set_label(GTK_BUTTON(buttonPortPtr_), "Close");
        gtk_widget_remove_css_class(buttonPortPtr_, "port-closed");
        gtk_widget_add_css_class(buttonPortPtr_, "port-opened");
    }else{
        gtk_button_set_label(GTK_BUTTON(buttonPortPtr_), "Open");
        gtk_widget_remove_css_class(buttonPortPtr_, "port-opened");
        gtk_widget_add_css_class(buttonPortPtr_, "port-closed");
    }
}

void GuiGtk::set_displacement_time([[maybe_unused]]long sec, long nsec){
    displacementTimeNs_ = nsec;
}
//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
GuiGtk::GuiGtk(){
    M_ = timeDiv_ * fs_ / 125.0f;
    appGtkPtr_ = gtk_application_new("com.ed.oscilloscope", G_APPLICATION_DEFAULT_FLAGS);
}

GuiGtk::~GuiGtk(){
    if(appGtkPtr_)
        g_object_unref(appGtkPtr_);
}

void GuiGtk::construct_window_callback(GtkApplication* appPt, gpointer userData){
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

            GtkTextTag *tagNormalPt = gtk_text_tag_new("normal");
            g_object_set(tagNormalPt, "foreground", "black", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagNormalPt);

            GtkTextTag *tagErrorPt = gtk_text_tag_new("error");
            g_object_set(tagErrorPt, "foreground", "red", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagErrorPt);

            GtkTextTag *tagWarningPt = gtk_text_tag_new("warning");
            g_object_set(tagWarningPt, "foreground", "yellow", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagWarningPt);

            GtkTextTag *tagOpenPt = gtk_text_tag_new("open");
            g_object_set(tagOpenPt, "foreground", "green", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagOpenPt);

            GtkTextTag *tagClosePt = gtk_text_tag_new("close");
            g_object_set(tagClosePt, "foreground", "blue", NULL);
            gtk_text_tag_table_add(tagTableConsolePt, tagClosePt);

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
                gtk_widget_add_css_class(spinOffset1Pt, "spinRed");

                GtkWidget *spinOffset2Pt = gtk_spin_button_new(adjustment2Pt, 0.1, 2);
                gtk_widget_set_size_request(spinOffset2Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinOffset2Pt, "spinOrange");

                GtkWidget *spinOffset3Pt = gtk_spin_button_new(adjustment3Pt, 0.1, 2);
                gtk_widget_set_size_request(spinOffset3Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinOffset3Pt, "spinGreen");

                GtkWidget *spinOffset4Pt = gtk_spin_button_new(adjustment4Pt, 0.1, 2);
                gtk_widget_set_size_request(spinOffset4Pt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinOffset4Pt, "spinBlue");

                GtkAdjustment *adjustmentFreqPt = gtk_adjustment_new(1.0, 0.01, 100.0, 0.01, 1.0, 0.0);
                GtkWidget *spinFreqPt = gtk_spin_button_new(adjustmentFreqPt, 1.0, 2);
                gtk_widget_set_size_request(spinFreqPt, WIDTH_WIDGET, WIDTH_LENGHT);
                gtk_widget_add_css_class(spinFreqPt, "spinTimeVal");

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

            GtkWidget* buttonDoc = gtk_button_new_with_label("Generate CSV");
            gtk_widget_add_css_class(buttonDoc, "doc");

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

        gtk_grid_attach(GTK_GRID(gridGeneralPt), glAreaVoltagePtr_,  0, 0, 1, 5);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), glAreaSpectrumPtr_, 0, 5, 1, 2);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), buttonStartStopPt,  1, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), dropVoltDivPt,      1, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), gridSignalsPt,      1, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), windowScroolPt,     1, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), buttonDoc,          1, 4, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), labelPortPt,        1, 5, 1, 1);
        gtk_grid_attach(GTK_GRID(gridGeneralPt), boxPortPt,          1, 6, 1, 1);

    gtk_window_set_child(GTK_WINDOW(windowPt), gridGeneralPt);

    GtkGesture *clickVoltagePt = gtk_gesture_click_new();
    gtk_widget_add_controller(glAreaVoltagePtr_, GTK_EVENT_CONTROLLER(clickVoltagePt));

    GtkGesture *clickSpectrumPt = gtk_gesture_click_new();
    gtk_widget_add_controller(glAreaSpectrumPtr_, GTK_EVENT_CONTROLLER(clickSpectrumPt));

    g_signal_connect(glAreaVoltagePtr_,  "realize",          G_CALLBACK(realize_voltage_callback),     userData);
    g_signal_connect(glAreaVoltagePtr_,  "unrealize",        G_CALLBACK(unrealize_voltage_callback),   userData);
    g_signal_connect(glAreaVoltagePtr_,  "render",           G_CALLBACK(render_voltage_callback),      userData);
    g_signal_connect(glAreaVoltagePtr_,  "resize",           G_CALLBACK(resize_voltage_callback),      userData);
    g_signal_connect(glAreaSpectrumPtr_, "realize",          G_CALLBACK(realize_spectrum_callback),    userData);
    g_signal_connect(glAreaSpectrumPtr_, "unrealize",        G_CALLBACK(unrealize_spectrum_callback),  userData);
    g_signal_connect(glAreaSpectrumPtr_, "resize",           G_CALLBACK(resize_spectrum_callback),     userData);
    g_signal_connect(glAreaSpectrumPtr_, "render",           G_CALLBACK(render_spectrum_callback),     userData);
    g_signal_connect(buttonStartStopPt,  "clicked",          G_CALLBACK(startStop_callback),           userData);
    g_signal_connect(dropVoltDivPt,      "notify::selected", G_CALLBACK(voltDiv_callback),             userData);
    g_signal_connect(checkSignal1Pt,     "toggled",          G_CALLBACK(check_signal_callback),        GUINT_TO_POINTER(0));
    g_signal_connect(checkSignal2Pt,     "toggled",          G_CALLBACK(check_signal_callback),        GUINT_TO_POINTER(1));
    g_signal_connect(checkSignal3Pt,     "toggled",          G_CALLBACK(check_signal_callback),        GUINT_TO_POINTER(2));
    g_signal_connect(checkSignal4Pt,     "toggled",          G_CALLBACK(check_signal_callback),        GUINT_TO_POINTER(3));
    g_signal_connect(spinOffset1Pt,      "value_changed",    G_CALLBACK(offset_signal_callback),       GUINT_TO_POINTER(0));
    g_signal_connect(spinOffset2Pt,      "value_changed",    G_CALLBACK(offset_signal_callback),       GUINT_TO_POINTER(1));
    g_signal_connect(spinOffset3Pt,      "value_changed",    G_CALLBACK(offset_signal_callback),       GUINT_TO_POINTER(2));
    g_signal_connect(spinOffset4Pt,      "value_changed",    G_CALLBACK(offset_signal_callback),       GUINT_TO_POINTER(3));
    g_signal_connect(spinFreqPt,         "value_changed",    G_CALLBACK(spin_button_freq_callback),    userData);
    g_signal_connect(buttonDoc,          "clicked",          G_CALLBACK(button_doc_callback),          userData);
    g_signal_connect(dropPortPtr_,       "notify::selected", G_CALLBACK(drop_port_callback),           userData);
    g_signal_connect(buttonPortPtr_,     "clicked",          G_CALLBACK(button_port_callback),         userData);
    g_signal_connect(windowPt,           "destroy",          G_CALLBACK(destroy_window_callback),      userData);
    g_signal_connect(clickVoltagePt,     "pressed",          G_CALLBACK(click_voltage_area_callback),  userData);
    g_signal_connect(clickSpectrumPt,    "pressed",          G_CALLBACK(click_spectrum_area_callback), userData);

    gtk_window_present(GTK_WINDOW(windowPt));

    if(signalCapturerPtr_ == nullptr){
        std::cerr << "No hay una asociación con un capturador de señales al momento de iniciar GTK.\n";
    }else{
        signalCapturerPtr_->start_reading();
    }

    if(devInspectorPtr_ == nullptr){
        std::cerr << "No hay una asociación con un inspector de dispositivos al momento de iniciar GTK.\n";
    }else{
        devInspectorPtr_->init();
    }

    if(docGeneratorPtr_ == nullptr){
        std::cerr << "No hay una asociación con un generador de documentos al momento de iniciar GTK.\n";
    }

    if(voltagesPtr_ == nullptr){
        std::cerr << "No hay una asociación con voltages al momento de iniciar GTK.\n";
    }

    drawTimeOutID_ = g_timeout_add(16, render, userData);
}

void GuiGtk::destroy_window_callback([[maybe_unused]]GtkWidget *widget, [[maybe_unused]]gpointer userData){
    if(signalCapturerPtr_)
        signalCapturerPtr_->stop_reading();

    if(devInspectorPtr_)
        devInspectorPtr_->stop();

    if(drawTimeOutID_){
        g_source_remove(drawTimeOutID_);
        drawTimeOutID_ = 0;
    }
}

void GuiGtk::realize_voltage_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : realize\n";
        return;
    }

    ProgramShader shader("./resources/shaders/codeVertexES.vs", "./resources/shaders/codeFragmentES.fs");
    idShaderVolt_ = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    create_VAO(gridVoltage_, false);
    if(voltagesPtr_){
        create_VAO(voltagesPtr_[0], true);
        create_VAO(voltagesPtr_[1], true);
        create_VAO(voltagesPtr_[2], true);
        create_VAO(voltagesPtr_[3], true);
    }
}

void GuiGtk::unrealize_voltage_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : unrealize\n"; 
    }

    destroy_VAO(gridVoltage_);
    if(voltagesPtr_){
        destroy_VAO(voltagesPtr_[0]);
        destroy_VAO(voltagesPtr_[1]);
        destroy_VAO(voltagesPtr_[2]);
        destroy_VAO(voltagesPtr_[3]);
    }

    glDeleteProgram(idShaderVolt_);
}

gboolean GuiGtk::render_voltage_callback(GtkGLArea *area, [[maybe_unused]]GdkGLContext *context, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : render\n";
        return FALSE;
    }

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(idShaderVolt_ != 0){
        glUseProgram(idShaderVolt_);
        //aqui dibuja
        draw_VAO(gridVoltage_, gridVoltage_.get_numOfPoints(), 1);

        if(voltagesPtr_){
            for(unsigned int i = 0; i < 4; i++){
                if(stateSignal_[i]){
                    {
                        std::lock_guard<std::mutex> lock(signalCapturerPtr_->get_mutex());

                        voltagesPtr_[i].apply_changes(offsets_[i], voltDiv_, M_, stateStartStop_);
                        update_VAO(voltagesPtr_[i], M_);
                        draw_VAO(voltagesPtr_[i], M_, 2);
                    }
                }
            }
        }
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
    gtk_gl_area_make_current(area);

    if(!gtk_gl_area_get_context(area)){
        std::cerr << "No OpenGL context available\n";
        return;
    }

    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : realize\n";
        return;
    }

    ProgramShader shader("./resources/shaders/codeVertexES.vs", "./resources/shaders/codeFragmentES.fs");
    idShaderSpec_ = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    create_VAO(gridSpectrum_, false);

    //Aqui crea los espectros
    if(voltagesPtr_){
        create_VAO(voltagesPtr_[0].spectrumSignal_, true);
        create_VAO(voltagesPtr_[1].spectrumSignal_, true);
        create_VAO(voltagesPtr_[2].spectrumSignal_, true);
        create_VAO(voltagesPtr_[3].spectrumSignal_, true);
    }
}

void GuiGtk::unrealize_spectrum_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : unrealize\n"; 
    }

    destroy_VAO(gridSpectrum_);
    //Aqui Elimina los spectros
    if(voltagesPtr_){
        destroy_VAO(voltagesPtr_[0].spectrumSignal_);
        destroy_VAO(voltagesPtr_[1].spectrumSignal_);
        destroy_VAO(voltagesPtr_[2].spectrumSignal_);
        destroy_VAO(voltagesPtr_[3].spectrumSignal_);
    }

    glDeleteProgram(idShaderSpec_);
}

gboolean GuiGtk::render_spectrum_callback(GtkGLArea *area, [[maybe_unused]]GdkGLContext *context, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : render\n";
        return false;
    }

    glClearColor(0.22f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(idShaderSpec_ != 0){
        glUseProgram(idShaderSpec_);

        draw_VAO(gridSpectrum_, gridSpectrum_.get_numOfPoints(), 1);

        if(voltagesPtr_){
            for(unsigned int i = 0; i < 4; i++){
                if(stateSignal_[i]){
                    {
                        std::lock_guard<std::mutex> lock(signalCapturerPtr_->get_mutex());

                        voltagesPtr_[i].calculate_spectrum(voltagesPtr_[i].get_numOfPoints());
                        voltagesPtr_[i].spectrumSignal_.update_vertex(voltagesPtr_[0].get_numOfPoints());
                        update_VAO(voltagesPtr_[i].spectrumSignal_, voltagesPtr_[0].get_numOfPoints());
                        draw_VAO(voltagesPtr_[i].spectrumSignal_, voltagesPtr_[0].get_numOfPoints(), 2);
                    }
                }
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

void GuiGtk::check_signal_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    auto nSignal = GPOINTER_TO_UINT(userData);

    if(nSignal < 4)
        stateSignal_[nSignal] = gtk_check_button_get_active(widget);
}

void GuiGtk::offset_signal_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    auto nSignal = GPOINTER_TO_UINT(userData);

    if(nSignal < 4)
        offsets_[nSignal] = gtk_spin_button_get_value(spinButton);
}

void GuiGtk::spin_button_freq_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    timeDiv_ = gtk_spin_button_get_value(spinButton);
    M_ = static_cast<unsigned long>(timeDiv_ * fs_ / 125); //#[] * tdiv * fs
}

void GuiGtk::button_doc_callback([[maybe_unused]]GtkButton *button, [[maybe_unused]]gpointer userData){
    if(docGeneratorPtr_)
        docGeneratorPtr_->generate_doc("./", APP::TYPE_DOC::CSV, fs_);
}

void GuiGtk::drop_port_callback(GtkDropDown *widget, [[maybe_unused]]gpointer userData){
    GObject *obj = reinterpret_cast<GObject*>(gtk_drop_down_get_selected_item(widget));

    if(!obj)
        return;

    const char *text = gtk_string_object_get_string(GTK_STRING_OBJECT(obj));

    if(text){
        routePort_ = text;
    }
}

void GuiGtk::button_port_callback([[maybe_unused]]GtkWidget *widget, [[maybe_unused]]gpointer userData){
    if(signalCapturerPtr_)
        signalCapturerPtr_->open_close_port(routePort_.c_str());
}

void GuiGtk::click_voltage_area_callback([[maybe_unused]]GtkGestureClick *gesture, [[maybe_unused]]int nPress, [[maybe_unused]]double x, double y, [[maybe_unused]]gpointer userData){
    int height = gtk_widget_get_height(glAreaVoltagePtr_);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "    %.3fV\n", (height/2-y)*8*voltDiv_/height);

    display_message_static(buffer, 1);
}

void GuiGtk::click_spectrum_area_callback([[maybe_unused]]GtkGestureClick *gesture, [[maybe_unused]]int nPress, double x, [[maybe_unused]]double y, [[maybe_unused]]gpointer userData){
    int width = gtk_widget_get_width(glAreaSpectrumPtr_);

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "    %.1fHz\n", (x * fs_)/width);

    display_message_static(buffer, 1);
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

void GuiGtk::create_VAO(DOMN::VaoObject& vaoObject, bool staticDynamic){
    if(vaoObject.get_vao()) //VAO ya existe
        return;
    
    glGenVertexArrays(1, &vaoObject.get_vao());

    if(vaoObject.get_elements_pt() == nullptr){
        glGenBuffers(2, &vaoObject.get_vertex_ind());
    }else{
        glGenBuffers(3, &vaoObject.get_vertex_ind());
    }

    glBindVertexArray(vaoObject.get_vao());

    glBindBuffer(GL_ARRAY_BUFFER, vaoObject.get_vertex_ind());
    if(staticDynamic){
        glBufferData(GL_ARRAY_BUFFER, 2*vaoObject.get_numOfPoints()*sizeof(float), vaoObject.get_vertices_pt(), GL_DYNAMIC_DRAW);
    }else{
        glBufferData(GL_ARRAY_BUFFER, 2*vaoObject.get_numOfPoints()*sizeof(float), vaoObject.get_vertices_pt(), GL_STATIC_DRAW);
    }
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vaoObject.get_color_ind());
    glBufferData(GL_ARRAY_BUFFER, 3*vaoObject.get_numOfPoints()*sizeof(float), vaoObject.get_color_pt(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    if(vaoObject.get_elements_pt() != nullptr){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vaoObject.get_elements_ind());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, vaoObject.get_numOfInd()*sizeof(unsigned int), vaoObject.get_elements_pt(), GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "Error al crear VAO: " << error << std::endl;
    }
}

void GuiGtk::destroy_VAO(DOMN::VaoObject& vaoObject){
    if(vaoObject.get_vao()){
        glDeleteVertexArrays(1, &vaoObject.get_vao());
        vaoObject.get_vao() = 0;
    }

    if(vaoObject.get_elements_pt() != nullptr){
        glDeleteBuffers(3, &vaoObject.get_vertex_ind());
        vaoObject.get_vertex_ind() = 0;
        vaoObject.get_color_ind() = 0;
        vaoObject.get_elements_ind() = 0;
    }else{
        glDeleteBuffers(2, &vaoObject.get_vertex_ind());
        vaoObject.get_vertex_ind() = 0;
        vaoObject.get_color_ind() = 0;
    }

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "Error al destruir VAO: " << error << std::endl;
    }
}

void GuiGtk::draw_VAO(DOMN::VaoObject& vaoObject, unsigned long numPoints, unsigned int lineWidth){
    if(!vaoObject.get_vao()) //No hay VAO
        return;
    
    if(numPoints > vaoObject.get_numOfPoints())
        numPoints = vaoObject.get_numOfPoints();

    glLineWidth(lineWidth);

    glBindVertexArray(vaoObject.get_vao());

    if(vaoObject.get_elements_pt() == nullptr){
        if(lineWidth == 1)
            glDrawArrays(GL_LINES, 0, numPoints);
        if(lineWidth == 2)
            glDrawArrays(GL_LINE_STRIP, 0, numPoints);
    }else{
        glDrawElements(GL_LINES, vaoObject.get_numOfInd(), GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "Error al dibujar VAO: " << error << std::endl;
    }
}

void GuiGtk::update_VAO(DOMN::VaoObject& vaoObject, unsigned long numPoints){
    if(!vaoObject.get_vao()) //No hay VAO
        return;
    
    if(numPoints > vaoObject.get_numOfPoints())
        numPoints = vaoObject.get_numOfPoints(); 

    glBindVertexArray(vaoObject.get_vao());

    glBindBuffer(GL_ARRAY_BUFFER, vaoObject.get_vertex_ind());
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2*numPoints*sizeof(float), vaoObject.get_vertices_pt());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "Error al actualizar VAO: " << error << std::endl;
    }
}

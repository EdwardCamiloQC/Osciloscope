#include <iostream>
#include <glm/glm.hpp>
#include <glib-unix.h>

#include <oscilloscope.hpp>
#include <shaders/shader.hpp>
#include <utils/capturers/puertoSerie.hpp>
#include <utils/capturers/signalGenerator.hpp>

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// METHODS GRID_VOLTAGE
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
// METHODS GRID_SPECTRUM
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
void GridSpectrum::createGrid(){
    //Genera el VAO y el VBO
    glGenVertexArrays(1, &(vao));
    glGenBuffers(2, vbo);

    //Enlaza el VAO para configurar los atributos
    glBindVertexArray(vao);
    //Enlaza el VBO y copia los datos
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 32*sizeof(float), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, 48*sizeof(float), color, GL_STATIC_DRAW);

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

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// STATICS ATTRIBUTES
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope*        Oscilloscope::instance_pt_ {nullptr};
GtkApplication*      Oscilloscope::appGtk_pt_ {nullptr};
GtkWidget*           Oscilloscope::glAreaVoltage_pt_ {nullptr};
GtkWidget*           Oscilloscope::glAreaSpectrum_pt_ {nullptr};
GtkWidget*           Oscilloscope::dropPort_pt_ {nullptr};
GtkWidget*           Oscilloscope::buttonPort_pt_ {nullptr};
GtkTextBuffer*       Oscilloscope::bufferConsole_pt_ {nullptr};
GtkStringList*       Oscilloscope::deviceListPort_pt_ {nullptr};
struct udev*         Oscilloscope::udev_pt_ {nullptr};
struct udev_monitor* Oscilloscope::monitor_pt_ {nullptr};
SignalCapturer       Oscilloscope::signalCapturer_;
VoltageSignal        Oscilloscope::voltage1_{Oscilloscope::LENGTH_VOLT_SIGNAL, SIGNAL_COLOR::RED};
VoltageSignal        Oscilloscope::voltage2_{Oscilloscope::LENGTH_VOLT_SIGNAL, SIGNAL_COLOR::ORANGE};
VoltageSignal        Oscilloscope::voltage3_{Oscilloscope::LENGTH_VOLT_SIGNAL, SIGNAL_COLOR::BLUE};
VoltageSignal        Oscilloscope::voltage4_{Oscilloscope::LENGTH_VOLT_SIGNAL, SIGNAL_COLOR::GREEN};
std::string          Oscilloscope::routePort_;
GridVoltage          Oscilloscope::gridVoltage_;
GridSpectrum         Oscilloscope::gridSpectrum_;
GLuint               Oscilloscope::idShaderVolt_    {0};
GLuint               Oscilloscope::idShaderSpec_    {0};
bool                 Oscilloscope::stateStartStop_  {false};
bool                 Oscilloscope::stateSignal1_    {false};
bool                 Oscilloscope::stateSignal2_    {false};
bool                 Oscilloscope::stateSignal3_    {false};
bool                 Oscilloscope::stateSignal4_    {false};
float                Oscilloscope::voltDiv_         {1.0f};
float                Oscilloscope::offset1_         {0.0f};
float                Oscilloscope::offset2_         {0.0f};
float                Oscilloscope::offset3_         {0.0f};
float                Oscilloscope::offset4_         {0.0f};
double               Oscilloscope::timeDiv_         {1.0};
guint                Oscilloscope::udevSourceID_    {0};
guint                Oscilloscope::drawTimeOutID_   {0};
unsigned int         Oscilloscope::shiftNvalues_    {Oscilloscope::M_VALUES_TO_SHIFT};

std::mutex Oscilloscope::mutex_;

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PUBLIC METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope::operator bool()const{
    return appGtk_pt_ != nullptr;
}

Oscilloscope* Oscilloscope::get_instance(){
    std::lock_guard<std::mutex> lock(mutex_);
    if(instance_pt_ == nullptr){
        instance_pt_ = new Oscilloscope();
        std::atexit(destroy_instance);
    }
    return instance_pt_;
}

void Oscilloscope::destroy_instance(){
    std::lock_guard<std::mutex> lock(mutex_);
    if(instance_pt_ != nullptr){
        delete instance_pt_;
        instance_pt_ = nullptr;
    }
}

int Oscilloscope::run(int &argc, char**& argv){
    return g_application_run(G_APPLICATION(appGtk_pt_), argc, argv);
}

//==================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PRIVATE METHODS
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//==================================================
Oscilloscope::Oscilloscope(){
    appGtk_pt_ = gtk_application_new("com.oscilloscope", G_APPLICATION_DEFAULT_FLAGS);
    signalCapturer_.select_capturer(std::make_unique<ComSerial>());
    routePort_.reserve(50);
    routePort_.clear();
    createContextUdev();
    g_signal_connect(appGtk_pt_, "activate", G_CALLBACK(construct_window_callback), this);
}

Oscilloscope::~Oscilloscope(){
    deleteContextUdev();
    if(appGtk_pt_){
        g_object_unref(appGtk_pt_);
    }
}

void Oscilloscope::construct_window_callback(GtkApplication* app, gpointer userData){
    GtkWidget *window = gtk_application_window_new(app);

    GtkWidget *boxPanels = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        GtkWidget *boxView = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
        gtk_widget_set_hexpand(boxView, true);
        gtk_widget_set_vexpand(boxView, true);
        gtk_box_set_spacing(GTK_BOX(boxView), 5);
            glAreaVoltage_pt_ = gtk_gl_area_new();
            gtk_widget_set_size_request(glAreaVoltage_pt_, -1, 700);

            glAreaSpectrum_pt_ = gtk_gl_area_new();
            gtk_widget_set_size_request(glAreaSpectrum_pt_, -1, 100);

            GtkTextTagTable *tagTableConsole = gtk_text_tag_table_new();
            bufferConsole_pt_ = gtk_text_buffer_new(tagTableConsole);
            GtkWidget *appConsole = gtk_text_view_new_with_buffer(bufferConsole_pt_);
            gtk_text_view_set_editable(GTK_TEXT_VIEW(appConsole), false);
            gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(appConsole), false);
            gtk_text_view_set_monospace(GTK_TEXT_VIEW(appConsole), true);
            GtkWidget *windowScrool = gtk_scrolled_window_new();
            gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(windowScrool), appConsole);
            gtk_widget_set_size_request(windowScrool, -1, 150);

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
        gtk_box_set_spacing(GTK_BOX(boxControl), 40);
            GtkCssProvider *provider = gtk_css_provider_new();
            GFile *fileStyles = g_file_new_for_path("./src/styles/styleButtonStartStop.css"); 
            gtk_css_provider_load_from_file(provider, fileStyles);
            g_object_unref(fileStyles);
            gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            GtkWidget *buttonStartStop = gtk_button_new_with_label("Start");
            gtk_widget_add_css_class(buttonStartStop, "led-off");

            GtkWidget *separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

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

            GtkWidget *boxFreq = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
            GtkAdjustment *adjustmentFreq = gtk_adjustment_new(1.0, 0.001, 100.0, 0.001, 1.0, 0.0);
            GtkWidget *spinFreq = gtk_spin_button_new(adjustmentFreq, 1.0, 3);
            GtkWidget *labelFreq = gtk_label_new("ms/div");
            GtkWidget *checkTestSignal = gtk_check_button_new_with_label("Test signal");

            GtkWidget *separator3 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

            GtkWidget *labelPort = gtk_label_new("Port");
            GtkWidget *boxPort = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
            deviceListPort_pt_ = gtk_string_list_new(nullptr);
            dropPort_pt_ = gtk_drop_down_new(G_LIST_MODEL(deviceListPort_pt_), nullptr);
            buttonPort_pt_ = gtk_button_new_with_label("Open");

    gtk_window_set_child(GTK_WINDOW(window), boxPanels);
    gtk_box_append(GTK_BOX(boxPanels),  boxView);
    gtk_box_append(GTK_BOX(boxView),    glAreaVoltage_pt_);
    gtk_box_append(GTK_BOX(boxView),    glAreaSpectrum_pt_);
    gtk_box_append(GTK_BOX(boxView),    windowScrool);
    gtk_box_append(GTK_BOX(boxPanels),  boxControl);
    gtk_box_append(GTK_BOX(boxControl), buttonStartStop);
    gtk_box_append(GTK_BOX(boxControl), separator1);
    gtk_box_append(GTK_BOX(boxControl), dropVoltDiv);
    gtk_box_append(GTK_BOX(boxControl), gridSignals);
    gtk_box_append(GTK_BOX(boxControl), separator2);
    gtk_box_append(GTK_BOX(boxControl), boxFreq);
    gtk_box_append(GTK_BOX(boxFreq),    spinFreq);
    gtk_box_append(GTK_BOX(boxFreq),    labelFreq);
    gtk_box_append(GTK_BOX(boxControl), checkTestSignal);
    gtk_box_append(GTK_BOX(boxControl), separator3);
    gtk_box_append(GTK_BOX(boxControl), labelPort);
    gtk_box_append(GTK_BOX(boxControl), boxPort);
    gtk_box_append(GTK_BOX(boxPort),    dropPort_pt_);
    gtk_box_append(GTK_BOX(boxPort),    buttonPort_pt_);

    g_signal_connect(glAreaVoltage_pt_,  "realize",          G_CALLBACK(realize_voltage_callback),    userData);
    g_signal_connect(glAreaVoltage_pt_,  "unrealize",        G_CALLBACK(unrealize_voltage_callback),  userData);
    g_signal_connect(glAreaVoltage_pt_,  "render",           G_CALLBACK(render_voltage_callback),     userData);
    g_signal_connect(glAreaVoltage_pt_,  "resize",           G_CALLBACK(resize_voltage_callback),     userData);
    g_signal_connect(glAreaSpectrum_pt_, "realize",          G_CALLBACK(realize_spectrum_callback),   userData);
    g_signal_connect(glAreaSpectrum_pt_, "unrealize",        G_CALLBACK(unrealize_spectrum_callback), userData);
    g_signal_connect(glAreaSpectrum_pt_, "resize",           G_CALLBACK(resize_spectrum_callback),    userData);
    g_signal_connect(glAreaSpectrum_pt_, "render",           G_CALLBACK(render_spectrum_callback),    userData);
    g_signal_connect(buttonStartStop,    "clicked",          G_CALLBACK(startStop_callback),          userData);
    g_signal_connect(dropVoltDiv,        "notify::selected", G_CALLBACK(voltDiv_callback),            userData);
    g_signal_connect(checkSignal1,       "toggled",          G_CALLBACK(check_signal1_callback),      userData);
    g_signal_connect(checkSignal2,       "toggled",          G_CALLBACK(check_signal2_callback),      userData);
    g_signal_connect(checkSignal3,       "toggled",          G_CALLBACK(check_signal3_callback),      userData);
    g_signal_connect(checkSignal4,       "toggled",          G_CALLBACK(check_signal4_callback),      userData);
    g_signal_connect(spinOffset1,        "value_changed",    G_CALLBACK(offset_signal1_callback),     userData);
    g_signal_connect(spinOffset2,        "value_changed",    G_CALLBACK(offset_signal2_callback),     userData);
    g_signal_connect(spinOffset3,        "value_changed",    G_CALLBACK(offset_signal3_callback),     userData);
    g_signal_connect(spinOffset4,        "value_changed",    G_CALLBACK(offset_signal4_callback),     userData);
    g_signal_connect(spinFreq,           "value_changed",    G_CALLBACK(spin_button_freq_callback),   userData);
    g_signal_connect(checkTestSignal,    "toggled",          G_CALLBACK(check_test_signal_callback),  userData);
    g_signal_connect(dropPort_pt_,       "notify::selected", G_CALLBACK(drop_port_callback),          userData);
    g_signal_connect(buttonPort_pt_,     "clicked",          G_CALLBACK(button_port_callback),        userData);
    g_signal_connect(window,             "destroy",          G_CALLBACK(destroy_window_callback),     userData);

    GdkDisplay *display = gdk_display_get_default();
    GListModel *monitors = gdk_display_get_monitors(display);
    GdkMonitor *monitor = GDK_MONITOR(g_list_model_get_item(monitors, 0));
    GdkRectangle monitorGeometry;
    gdk_monitor_get_geometry(monitor, &monitorGeometry);

    gtk_window_set_title(GTK_WINDOW(window), "Oscilloscope");
    gtk_window_set_default_size(GTK_WINDOW(window), monitorGeometry.width, monitorGeometry.height);
    gtk_window_present(GTK_WINDOW(window));

    int udev_fd = udev_monitor_get_fd(monitor_pt_);
    if(udev_fd >= 0){
        udevSourceID_ = g_unix_fd_add(udev_fd, G_IO_IN, udev_monitor_inspection, nullptr);
    }

    stateStartStop_ = true;

    drawTimeOutID_ = g_timeout_add(static_cast<guint>(8*10*shiftNvalues_/LENGTH_VOLT_SIGNAL), catch_voltages, userData);
}

void Oscilloscope::destroy_window_callback([[maybe_unused]]GtkWidget *widget, [[maybe_unused]]gpointer userData){
    stateStartStop_ = false;
    if(drawTimeOutID_){
        g_source_remove(drawTimeOutID_);
        drawTimeOutID_ = 0;
    }
    if(udevSourceID_){
        g_source_remove(udevSourceID_);
        udevSourceID_ = 0;
    }
}

void Oscilloscope::realize_voltage_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : realize" << std::endl;
        return;
    }

    ProgramShader shader("./src/shaders/codeVertexES.vs", "./src/shaders/codeFragmentES.fs");
    idShaderVolt_ = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    gridVoltage_.createGrid();
    createVAO(voltage1_);
    createVAO(voltage2_);
    createVAO(voltage3_);
    createVAO(voltage4_);
}

void Oscilloscope::unrealize_voltage_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : unrealize" << std::endl; 
    }

    gridVoltage_.deleteGrid();
    deleteVAO(voltage1_);
    deleteVAO(voltage2_);
    deleteVAO(voltage3_);
    deleteVAO(voltage4_);

    glDeleteProgram(idShaderVolt_);
}

gboolean Oscilloscope::render_voltage_callback(GtkGLArea *area, [[maybe_unused]]GdkGLContext *context, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSignal : render" << std::endl;
        return FALSE;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    voltage1_.apply_offset(offset1_, voltDiv_);
    voltage2_.apply_offset(offset2_, voltDiv_);
    voltage3_.apply_offset(offset3_, voltDiv_);
    voltage4_.apply_offset(offset4_, voltDiv_);

    if(idShaderVolt_ != 0){
        glUseProgram(idShaderVolt_);
        //aqui dibuja
        gridVoltage_.draw();
        if(stateSignal1_){
            drawVAO(voltage1_);
        }
        if(stateSignal2_){
            drawVAO(voltage2_);
        }
        if(stateSignal3_){
            drawVAO(voltage3_);
        }
        if(stateSignal4_){
            drawVAO(voltage4_);
        }
    }

    // Verificar errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    return TRUE;
}

void Oscilloscope::resize_voltage_callback([[maybe_unused]]GtkGLArea *area, [[maybe_unused]]gpointer userData){
    //glViewport(0, 0, width, height);
}

void Oscilloscope::realize_spectrum_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);

    if(!gtk_gl_area_get_context(area)){
        std::cerr << "No OpenGL context available\n";
        return;
    }

    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : realize" << std::endl;
        return;
    }

    ProgramShader shader("./src/shaders/codeVertexES.vs", "./src/shaders/codeFragmentES.fs");
    idShaderSpec_ = shader.shaderProgramId;

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    gridSpectrum_.createGrid();

    //Aqui crea los espectros
    createVAO(voltage1_.spectrumSignal_);
    createVAO(voltage2_.spectrumSignal_);
    createVAO(voltage3_.spectrumSignal_);
    createVAO(voltage4_.spectrumSignal_);
}

void Oscilloscope::unrealize_spectrum_callback(GtkGLArea *area, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : unrealize" << std::endl; 
    }

    gridSpectrum_.deleteGrid();
    //Aqui Elimina los spectros
    deleteVAO(voltage1_.spectrumSignal_);
    deleteVAO(voltage2_.spectrumSignal_);
    deleteVAO(voltage3_.spectrumSignal_);
    deleteVAO(voltage4_.spectrumSignal_);

    glDeleteProgram(idShaderSpec_);
}

gboolean Oscilloscope::render_spectrum_callback(GtkGLArea *area, [[maybe_unused]]GdkGLContext *context, [[maybe_unused]]gpointer userData){
    gtk_gl_area_make_current(area);
    if(gtk_gl_area_get_error(area) != NULL){
        std::cerr << "Failed to link contex areaSpectrum : render" << std::endl;
        return false;
    }

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //aqui ajusta los voltajes

    if(stateStartStop_){
        voltage1_.spectrumSignal_.updateVertex();
        voltage2_.spectrumSignal_.updateVertex();
        voltage3_.spectrumSignal_.updateVertex();
        voltage4_.spectrumSignal_.updateVertex();
    }
    if(idShaderSpec_ != 0){
        glUseProgram(idShaderSpec_);

        gridSpectrum_.draw();
        //aqui dibuja
        if(stateSignal1_){
            drawVAO(voltage1_.spectrumSignal_);
        }
        if(stateSignal2_){
            drawVAO(voltage2_.spectrumSignal_);
        }
        if(stateSignal3_){
            drawVAO(voltage3_.spectrumSignal_);
        }
        if(stateSignal4_){
            drawVAO(voltage4_.spectrumSignal_);
        }
    }

    // Verificar errores de OpenGL
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }

    return true;
}

void Oscilloscope::resize_spectrum_callback(){
    //
}

void Oscilloscope::startStop_callback(GtkWidget *widget, [[maybe_unused]]gpointer userData){
    stateStartStop_ = !stateStartStop_;
    if(stateStartStop_){
        gtk_button_set_label(GTK_BUTTON(widget), "Stop");
        gtk_widget_remove_css_class(widget, "led-off");
        gtk_widget_add_css_class(widget, "led-on");
    }else{
        gtk_button_set_label(GTK_BUTTON(widget), "Start");
        gtk_widget_remove_css_class(widget, "led-on");
        gtk_widget_add_css_class(widget, "led-off");
        voltage1_.pivot_ping_pong_voltages();
        voltage2_.pivot_ping_pong_voltages();
        voltage3_.pivot_ping_pong_voltages();
        voltage4_.pivot_ping_pong_voltages();
    }
}

void Oscilloscope::voltDiv_callback(GtkDropDown *widget, [[maybe_unused]]gpointer userData){
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

void Oscilloscope::check_signal1_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    stateSignal1_ = gtk_check_button_get_active(widget);
}

void Oscilloscope::check_signal2_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    stateSignal2_ = gtk_check_button_get_active(widget);
}

void Oscilloscope::check_signal3_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    stateSignal3_ = gtk_check_button_get_active(widget);
}

void Oscilloscope::check_signal4_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    stateSignal4_ = gtk_check_button_get_active(widget);
}

void Oscilloscope::offset_signal1_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    offset1_ = gtk_spin_button_get_value(spinButton);
}

void Oscilloscope::offset_signal2_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    offset2_ = gtk_spin_button_get_value(spinButton);
}

void Oscilloscope::offset_signal3_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    offset3_ = gtk_spin_button_get_value(spinButton);
}

void Oscilloscope::offset_signal4_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    offset4_ = gtk_spin_button_get_value(spinButton);
}

void Oscilloscope::spin_button_freq_callback(GtkSpinButton *spinButton, [[maybe_unused]]gpointer userData){
    timeDiv_ = gtk_spin_button_get_value(spinButton);
}

void Oscilloscope::check_test_signal_callback(GtkCheckButton *widget, [[maybe_unused]]gpointer userData){
    if(gtk_check_button_get_active(widget)){
        signalCapturer_.select_capturer(std::make_unique<SignalGenerator>());
        gtk_button_set_label(GTK_BUTTON(buttonPort_pt_), "Open");
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole_pt_, &end);
        gtk_text_buffer_insert_with_tags_by_name(bufferConsole_pt_, &end, "Puerto serial cerrado\n", -1, "close", NULL);
    }else{
        signalCapturer_.select_capturer(std::make_unique<ComSerial>());
    }
}

void Oscilloscope::drop_port_callback(GtkDropDown *widget, [[maybe_unused]]gpointer userData){
    GObject *obj = reinterpret_cast<GObject*>(gtk_drop_down_get_selected_item(widget));

    if(!obj)
        return;

    const char *text = gtk_string_object_get_string(GTK_STRING_OBJECT(obj));

    if(text){
        std::cout << text << std::endl;
        routePort_ = text;
    }
}

void Oscilloscope::button_port_callback(GtkWidget *widget, [[maybe_unused]]gpointer userData){
    if(signalCapturer_.capturer->getId() == SERIAL_PORT_ID){
        if(signalCapturer_.capturer->getFlagSerial()){
            signalCapturer_.capturer->closePort();
            if(!(signalCapturer_.capturer->getFlagSerial())){
                gtk_button_set_label(GTK_BUTTON(widget), "Open");
                GtkTextIter end;
                gtk_text_buffer_get_end_iter(bufferConsole_pt_, &end);
                gtk_text_buffer_insert_with_tags_by_name(bufferConsole_pt_, &end, "Puerto serial cerrado\n", -1, "close", NULL);
            }
        }else{
            signalCapturer_.capturer->openPort(routePort_.c_str());
            if(signalCapturer_.capturer->getFlagSerial()){
                gtk_button_set_label(GTK_BUTTON(widget), "Close");
                GtkTextIter end;
                gtk_text_buffer_get_end_iter(bufferConsole_pt_, &end);
                gtk_text_buffer_insert_with_tags_by_name(bufferConsole_pt_, &end, "Puerto serial abierto\n", -1, "open", NULL);
            }
        }
    }
}

gboolean Oscilloscope::udev_monitor_inspection([[maybe_unused]]gint fd, [[maybe_unused]]GIOCondition condition, [[maybe_unused]]gpointer userData){
    struct udev_device *dev = udev_monitor_receive_device(monitor_pt_);
    if(!dev){
        return TRUE;
    }

    const char *action = udev_device_get_action(dev);
    const char *devnode = udev_device_get_devnode(dev);

    if(action && devnode){
        if(strcmp(action, "add") == 0){
            gchar *copy = g_strdup(devnode);
            updateDropPort(true, copy);
            g_free(copy);
        }else if(strcmp(action, "remove") == 0){
            gchar *copy = g_strdup(devnode);
            updateDropPort(false, copy);
            g_free(copy);
        }
    }
    
    udev_device_unref(dev);

    return TRUE;
}

gboolean Oscilloscope::catch_voltages(gpointer userData){
    gboolean state = 0;
    //shift the voltages
    voltage1_.shift_voltage(shiftNvalues_);
    voltage2_.shift_voltage(shiftNvalues_);
    voltage3_.shift_voltage(shiftNvalues_);
    voltage4_.shift_voltage(shiftNvalues_);
    signalCapturer_.catch_voltages(&voltage1_, &voltage2_, &voltage3_, &voltage4_, shiftNvalues_);
    voltage1_.calculate_spectrum();
    voltage2_.calculate_spectrum();
    voltage3_.calculate_spectrum();
    voltage4_.calculate_spectrum();
    if(stateStartStop_){
        state = draw_signals(userData);
    }else{
        voltage1_.pivot_ping_pong_voltages();
        voltage2_.pivot_ping_pong_voltages();
        voltage3_.pivot_ping_pong_voltages();
        voltage4_.pivot_ping_pong_voltages();
        state = draw_signals(userData);
        voltage1_.pivot_ping_pong_voltages();
        voltage2_.pivot_ping_pong_voltages();
        voltage3_.pivot_ping_pong_voltages();
        voltage4_.pivot_ping_pong_voltages();
    }
    return state;
}

gboolean Oscilloscope::draw_signals([[maybe_unused]]gpointer userData){
    Oscilloscope *osc = Oscilloscope::get_instance();

    if(!osc){
        return G_SOURCE_REMOVE;
    }

    if(GTK_IS_GL_AREA(osc->glAreaVoltage_pt_)){
        gtk_gl_area_queue_render(GTK_GL_AREA(osc->glAreaVoltage_pt_));
    }

    if(GTK_IS_GL_AREA(osc->glAreaSpectrum_pt_)){
        gtk_gl_area_queue_render(GTK_GL_AREA(osc->glAreaSpectrum_pt_));
    }

    return G_SOURCE_CONTINUE;
}

int Oscilloscope::createContextUdev(){
    //crear contexto udev
	udev_pt_ = udev_new();
	if(!udev_pt_){
		std::cerr << "No se pudo crear el contexto udev\n";
		return EXIT_FAILURE;
	}

	//Crear monitor para recibir eventos desde udev a través de netlink
	monitor_pt_ = udev_monitor_new_from_netlink(udev_pt_, "udev");
	if(!monitor_pt_){
		std::cerr << "No se pudo crear el monitor de udev\n";
		udev_unref(udev_pt_);
		return EXIT_FAILURE;
	}

	//Filtrar eventos para el subsistema "tty"
	if(udev_monitor_filter_add_match_subsystem_devtype(monitor_pt_, "tty", nullptr) < 0){
		std::cerr << "Error al aplicar el filtro para el subsistema tty\n";
		udev_monitor_unref(monitor_pt_);
		udev_unref(udev_pt_);
		return EXIT_FAILURE;
	}

	//Habilitar la recepcion de eventos
	if(udev_monitor_enable_receiving(monitor_pt_) < 0){
		std::cerr << "Error al habilitar la recepcion de eventos\n";
		udev_monitor_unref(monitor_pt_);
		udev_unref(udev_pt_);
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}

int Oscilloscope::deleteContextUdev(){
    if(monitor_pt_){
        udev_monitor_unref(monitor_pt_);
    }
    if(udev_pt_){
        udev_unref(udev_pt_);
    }

    return EXIT_SUCCESS;
}

bool Oscilloscope::createVAO(SignalObject &signalObject){
    glGenVertexArrays(1, &signalObject.vao);
    glGenBuffers(2, signalObject.vbo);

    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 2*signalObject.length*sizeof(float), nullptr, GL_STATIC_DRAW);

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
        gtk_text_buffer_get_end_iter(bufferConsole_pt_, &end);
        gtk_text_buffer_insert(bufferConsole_pt_, &end, "OpenGL Error in createVAO\n", -1);
        std::cerr << "OpenGL Error in createVAO: " << error << std::endl;
        return true;
    }

    return false;
}

bool Oscilloscope:: deleteVAO(SignalObject &signalObject){
    glDeleteVertexArrays(1, &signalObject.vao);
    glDeleteBuffers(2, signalObject.vbo);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        std::cerr << "OpenGL Error in deleteVAO: " << error << std::endl;
        return true;
    }

    return false;
}

bool Oscilloscope:: drawVAO(SignalObject &signalObject){
    glBindVertexArray(signalObject.vao);
    glBindBuffer(GL_ARRAY_BUFFER, signalObject.vbo[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 2*signalObject.length*sizeof(float), signalObject.vertex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glLineWidth(2);
    glBindVertexArray(signalObject.vao);
    glDrawArrays(GL_LINE_STRIP, 0, signalObject.length);
    glBindVertexArray(0);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(bufferConsole_pt_, &end);
        gtk_text_buffer_insert(bufferConsole_pt_, &end, "OpenGL Error in drawVAO\n", -1);
        std::cerr << "OpenGL Error in drawVAO: " << error << std::endl;
        return true;
    }

    return false;
}

void Oscilloscope::updateDropPort(const bool add, const char *text){
    GtkDropDown *dropPort = GTK_DROP_DOWN(dropPort_pt_);
    GListModel *model = gtk_drop_down_get_model(dropPort);

    if(model == nullptr){
        if(add){
            gtk_string_list_append(deviceListPort_pt_, text);
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
                    gtk_string_list_remove(deviceListPort_pt_, i);
                }
            }
        }
        g_object_unref(item);
    }

    if(foundIndex == -1){
        if(add){
            gtk_string_list_append(deviceListPort_pt_, text);
        }
    }
}

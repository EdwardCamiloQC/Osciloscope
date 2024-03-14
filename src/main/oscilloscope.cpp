#include <oscilloscope.h>
#include <epoxy/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <grid.h>

unsigned int idS;
unsigned int VAOs[3];
unsigned int VBOs[3];
unsigned int EBO;
struct Cuadricula gridDivition;

//------------------------------------------------
//------------------------------------------------
//                  STATIC FUNCTIONS (signals)
//------------------------------------------------
//------------------------------------------------
static void resize(GtkGLArea* self, gint width, gint height, gpointer user_data){
    glViewport(0, 0, width, height);
}

static void realize(GtkWidget *widget, gpointer userData){
    //GdkGLContext *context;
    gtk_gl_area_make_current(GTK_GL_AREA(widget));
    if (gtk_gl_area_get_error(GTK_GL_AREA(widget)) != NULL){
        std::cout << "Failed to initialiize buffers : realize" << std::endl;
        return;
    }
    //context = gtk_gl_area_get_context(GTK_GL_AREA (widget));

    ProgramShaders programShaders("./src/shaders/nivel1.vs", "./src/shaders/nivel1.fs");
    idS = programShaders.shaderProgramId;
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);  
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridDivition.vertex) + sizeof(((Oscilloscope*)userData)->psoc.dataVoltage1) + sizeof(((Oscilloscope*)userData)->psoc.dataVoltage2), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(gridDivition.vertex), gridDivition.vertex);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(gridDivition.vertex), sizeof(((Oscilloscope*)userData)->psoc.dataVoltage1), ((Oscilloscope*)userData)->psoc.dataVoltage1);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(gridDivition.vertex) + sizeof(((Oscilloscope*)userData)->psoc.dataVoltage1), sizeof(((Oscilloscope*)userData)->psoc.dataVoltage2), ((Oscilloscope*)userData)->psoc.dataVoltage2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(((Oscilloscope*)userData)->psoc.elementSignals), ((Oscilloscope*)userData)->psoc.elementSignals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(((Oscilloscope*)userData)->psoc.colorSignal1), ((Oscilloscope*)userData)->psoc.colorSignal1, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(((Oscilloscope*)userData)->psoc.colorSignal2), ((Oscilloscope*)userData)->psoc.colorSignal2, GL_STATIC_DRAW);

    glBindVertexArray(VAOs[0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindVertexArray(VAOs[1]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(sizeof(gridDivition.vertex)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    glBindVertexArray(VAOs[2]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(sizeof(gridDivition.vertex)+sizeof(((Oscilloscope*)userData)->psoc.dataVoltage1)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    glBindVertexArray(0);

}

static void unrealize(GtkWidget *widget){
    gtk_gl_area_make_current(GTK_GL_AREA(widget));
    if(gtk_gl_area_get_error(GTK_GL_AREA (widget)) != NULL){
        std::cout << "Failed to initialiize buffers : unrealize" << std::endl;
        return;
    }
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(3, VBOs);
    glDeleteVertexArrays(3, VAOs);
    glDeleteProgram (idS);
}

static gboolean render (GtkGLArea *area, GdkGLContext *context, gpointer userData){
    gtk_gl_area_make_current(GTK_GL_AREA(area));
    gdk_gl_context_make_current(context);
    if(gtk_gl_area_get_error (area) != NULL){
        std::cout << "Failed to initialiize buffers : render" << std::endl;
        return FALSE;
    }

    ((Oscilloscope*)userData)->psoc.run(20);

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(gridDivition.vertex), sizeof(((Oscilloscope*)userData)->psoc.dataVoltage1), ((Oscilloscope*)userData)->psoc.dataVoltage1);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(gridDivition.vertex) + sizeof(((Oscilloscope*)userData)->psoc.dataVoltage1), sizeof(((Oscilloscope*)userData)->psoc.dataVoltage2), ((Oscilloscope*)userData)->psoc.dataVoltage2);

    glUseProgram(idS);
    //draw;
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_LINES, 0, sizeof(gridDivition.vertex)/(sizeof(float)*2));
    
    glPointSize(2);
    glBindVertexArray(VAOs[1]);
    //glDrawArrays(GL_POINTS, 0, sizeof(psoc.dataVoltage1)/sizeof(float));
    glDrawElements(GL_LINES, (sizeof(((Oscilloscope*)userData)->psoc.dataVoltage1)/sizeof(float))/2, GL_UNSIGNED_INT, 0);

    glBindVertexArray(VAOs[2]);
    //glDrawArrays(GL_POINTS, 0, sizeof(psoc.dataVoltage2)/sizeof(float));
    glDrawElements(GL_LINES, (sizeof(((Oscilloscope*)userData)->psoc.dataVoltage2)/sizeof(float))/2, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    
    glFlush();
    gtk_gl_area_attach_buffers(area);
    gtk_gl_area_queue_render(area);
    gtk_widget_queue_draw((GtkWidget*)area);
    return TRUE;
}

int grab_int_value(GtkSpinButton *button, gpointer userData){
    int value = gtk_spin_button_get_value_as_int(button);
    std::cout << value << std::endl;
    return value;
}

static void clickComboBox(GtkWidget *widget, gpointer userData){
    const char *id = gtk_combo_box_get_active_id(GTK_COMBO_BOX(widget));
    const char *item = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
    std::cout << id << std::endl;
    std::cout << item << std::endl;
}

static void activate(GtkApplication* app, gpointer userData){
    GtkWidget *window;
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Oscilloscope");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);

    GtkWidget *boxPrincipal, *boxCanva, *boxControls;
    boxPrincipal = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    boxCanva = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    boxControls = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

    GtkWidget *canva;
    canva = gtk_gl_area_new();

    GtkWidget *labelFreq, *comboBoxFreq, *buttonFreq;
    //GtkListStore *listFreq;
    GtkAdjustment *adjustment;
    labelFreq = gtk_label_new("Frequency");
    comboBoxFreq = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(comboBoxFreq), "1", "Hz");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(comboBoxFreq), "2", "Khz");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(comboBoxFreq), "3", "Mhz");
    adjustment = gtk_adjustment_new(50.0, 0.0, 100.0, 1.0, 5.0, 0.0);
    buttonFreq = gtk_spin_button_new(adjustment, 1.0, 0);

    GtkWidget *separator1;
    separator1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    GtkWidget *labelVolToDivision, *menuButtonVolt;
    GMenu *menuVolDiv;
    labelVolToDivision = gtk_label_new("Vol/Div");
    menuVolDiv = g_menu_new();
    g_menu_append(menuVolDiv, "0.1v/div", "0.1");
    g_menu_append(menuVolDiv, "0.2v/div", "0.2");
    g_menu_append(menuVolDiv, "0.3v/div", "0.3");
    g_menu_append(menuVolDiv, "0.4v/div", "0.4");
    g_menu_append(menuVolDiv, "0.5v/div", "0.5");
    g_menu_append(menuVolDiv, "1v/div", "1");
    g_menu_append(menuVolDiv, "2v/div", "2");
    g_menu_append(menuVolDiv, "3v/div", "3");
    g_menu_append(menuVolDiv, "4v/div", "4");
    g_menu_append(menuVolDiv, "5v/div", "5");
    menuButtonVolt = gtk_menu_button_new();
    gtk_menu_button_set_menu_model(GTK_MENU_BUTTON(menuButtonVolt), G_MENU_MODEL(menuVolDiv));

    gtk_box_pack_start(GTK_BOX(boxCanva), canva, true, true, 0);
    gtk_box_pack_start(GTK_BOX(boxControls), labelFreq, false, false, 2);
    gtk_box_pack_start(GTK_BOX(boxControls), comboBoxFreq, false, false, 2);
    gtk_box_pack_start(GTK_BOX(boxControls), buttonFreq, false, false, 2);
    gtk_box_pack_start(GTK_BOX(boxControls), separator1, false, false, 10);
    gtk_box_pack_start(GTK_BOX(boxControls), labelVolToDivision, false, false, 2);
    gtk_box_pack_start(GTK_BOX(boxControls), menuButtonVolt, false, false, 2);
    gtk_box_pack_start(GTK_BOX(boxPrincipal), boxCanva, true, true, 0);
    gtk_box_pack_start(GTK_BOX(boxPrincipal), boxControls, false, false, 0);
    gtk_container_add(GTK_CONTAINER(window), boxPrincipal);

    g_signal_connect(canva, "realize", G_CALLBACK(realize), userData);
    g_signal_connect(canva, "unrealize", G_CALLBACK(unrealize), userData);
    g_signal_connect(canva, "resize", G_CALLBACK(resize), NULL);
    g_signal_connect(canva, "render", G_CALLBACK(render), userData);
    g_signal_connect(buttonFreq, "value_changed", G_CALLBACK(grab_int_value), NULL);
    g_signal_connect(comboBoxFreq, "changed", G_CALLBACK(clickComboBox), NULL);

    gtk_window_present(GTK_WINDOW(window));
    gtk_widget_show_all(window);
}

//------------------------------------------------
//------------------------------------------------
//                  METHODS
//------------------------------------------------
//------------------------------------------------
Oscilloscope :: Oscilloscope(int& argc, char**& argv, const char* port):psoc(port){
    oscilloscope = gtk_application_new("com.oscilloscope", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(oscilloscope, "activate", G_CALLBACK(activate), this);
}

int Oscilloscope::oscilloscope_run(int &argc, char**& argv){
    //gtk_main();
    int status;
    status = g_application_run(G_APPLICATION(oscilloscope), argc, argv);
    return status;
}

Oscilloscope::~Oscilloscope(){
    g_object_unref(oscilloscope);
}

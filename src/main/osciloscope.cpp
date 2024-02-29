#include <glad/glad.h>
#include <osciloscope.h>
#include <glm/glm.hpp>
#include <iostream>
#include <shaders.h>

#ifdef VIEW_2D_ENABLE
    #define VISION true
#else
    #define VISION false
#endif

bool point1 = true, point2 = false;
float lastX = 600 / 2.0f;
float lastY = 800 / 2.0f;
float fov = 45.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void inputKeys(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

WindowOsciloscope::WindowOsciloscope(uint16_t width, uint16_t height, const char* port):ancho(width), alto(height), psoc(port), window(nullptr){
    flagRun = true;
    backgroundColor[0] = 0.3f;
    backgroundColor[1] = 0.3f;
    backgroundColor[2] = 0.3f;
    backgroundColor[3] = 1.0f;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, NAME_WINDOW, NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initializze GLAD" << std::endl;
    }
    if(VISION){
        glDisable(GL_DEPTH_TEST);
    }else{
        glEnable(GL_DEPTH_TEST);
    }
    ProgramShaders program("./src/shaders/nivel1.vs", "./src/shaders/nivel1.fs");
    idP = program.shaderProgramId;

    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grid.vertex) + sizeof(psoc.dataVoltage1) + sizeof(psoc.dataVoltage2), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(grid.vertex), grid.vertex);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(grid.vertex), sizeof(psoc.dataVoltage1), psoc.dataVoltage1);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(grid.vertex) + sizeof(psoc.dataVoltage1), sizeof(psoc.dataVoltage2), psoc.dataVoltage2);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(psoc.colorSignal1), psoc.colorSignal1, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(psoc.colorSignal2), psoc.colorSignal2, GL_STATIC_DRAW);

    glBindVertexArray(VAOs[0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);

    glBindVertexArray(VAOs[1]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(sizeof(grid.vertex)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    glBindVertexArray(VAOs[2]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(sizeof(grid.vertex)+sizeof(psoc.dataVoltage1)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    glBindVertexArray(0);
}

WindowOsciloscope::WindowOsciloscope(const WindowOsciloscope& w):psoc(nullptr), window(nullptr){
    ancho = w.ancho;
    alto = w.alto;
    for(size_t i = 0; i < 4; i++){
        backgroundColor[i] = w.backgroundColor[i];
    }
    if(w.window){
        window = w.window;
    }
}

bool WindowOsciloscope::getFlagRun(){
    return flagRun;
}

void WindowOsciloscope::run(){
    while(!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        inputKeys(window);
        psoc.run(10);

        glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(grid.vertex), sizeof(psoc.dataVoltage1), psoc.dataVoltage1);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(grid.vertex) + sizeof(psoc.dataVoltage1), sizeof(psoc.dataVoltage2), psoc.dataVoltage2);

        glUseProgram(idP);
        
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_LINES, 0, sizeof(grid.vertex)/(sizeof(float)*2));
        
        glPointSize(2);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_POINTS, 0, sizeof(psoc.dataVoltage1)/sizeof(float));

        glBindVertexArray(VAOs[2]);
        glDrawArrays(GL_POINTS, 0, sizeof(psoc.dataVoltage2)/sizeof(float));

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

WindowOsciloscope::~WindowOsciloscope(){
    flagRun = false;
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(idP);
    glfwTerminate();
}

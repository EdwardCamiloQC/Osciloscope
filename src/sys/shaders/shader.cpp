#include <sys/shaders/shader.hpp>
extern "C"{
    #include <epoxy/gl.h>
}
#include <fstream>
#include <sstream>
#include <iostream>

ProgramShader::ProgramShader(const char* vertexPath, const char* fragmentPath){
    vertex = compileShader(vertexPath, GL_VERTEX_SHADER);
    fragment = compileShader(fragmentPath, GL_FRAGMENT_SHADER);
    if(vertex != 0 && fragment != 0){
        linkShaders();
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int ProgramShader::compileShader(const std::string& shaderPath, unsigned int moduleType){
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line, shaderSource;
    unsigned int shaderModule = 0;
    file.open(shaderPath);
    if(file.is_open()){
        while(std::getline(file, line)){
            bufferedLines << line << "\n";
        }
        shaderSource = bufferedLines.str();
        const char* shaderSrc = shaderSource.c_str();
        bufferedLines.str("");
        file.close();
        shaderModule = glCreateShader(moduleType);
        glShaderSource(shaderModule, 1, &shaderSrc, NULL);
        glCompileShader(shaderModule);
        int success;
        glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
        if(!success){
            char infoLog[512];
            glGetShaderInfoLog(shaderModule, 512, NULL, infoLog);
            std::cerr << shaderPath << "\n" << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
            return 0;
        }
    }else{
        std::cerr << shaderPath << "\n" << "No open file" << std::endl;
    }
    return shaderModule;
}

void ProgramShader::linkShaders(){
    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertex);
    glAttachShader(shaderProgramId, fragment);
    glLinkProgram(shaderProgramId);
    // check errors
    int success;
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

ProgramShader::~ProgramShader(){
}

#include <shaders.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

ProgramShaders :: ProgramShaders(const char* vertexPath, const char* fragmentPath){
    vertex = compileShaders(vertexPath, GL_VERTEX_SHADER);
    fragment = compileShaders(fragmentPath, GL_FRAGMENT_SHADER);
    linkShaders();
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int ProgramShaders :: compileShaders(const std::string& shaderPath, unsigned int moduleType){
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;
    file.open(shaderPath);
    while(std::getline(file, line)){
        bufferedLines << line << "\n";
    }
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();
    unsigned int shaderModule = glCreateShader(moduleType);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);
    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(shaderModule, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shaderModule;
}

void ProgramShaders :: linkShaders(){
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
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}
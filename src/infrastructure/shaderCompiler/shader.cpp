extern "C"{
    #include <epoxy/gl.h>
}
#include <fstream>
#include <sstream>
#include <iostream>

#include <infrastructure/shaderCompiler/shader.hpp>

ProgramShader::ProgramShader(const char* vertexPath, const char* fragmentPath){
    vertex = compileShader(vertexPath, GL_VERTEX_SHADER);
    fragment = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

    if(vertex && fragment) linkShaders();

    if(vertex) glDeleteShader(vertex);
    
    if(fragment) glDeleteShader(fragment);
}

unsigned int ProgramShader::compileShader(const std::string& shaderPath, unsigned int moduleType){
    std::ifstream file(shaderPath, std::ios::binary);

    if(!file.is_open()){
        std::cerr << shaderPath << "\nNo open file\n";
        return 0;
    }

    std::stringstream bufferedLines;
    std::string line;

    while(std::getline(file, line))
        bufferedLines << line << "\n";

    std::string shaderSource = bufferedLines.str();

    if(shaderSource.empty()){
        std::cerr << shaderPath << " Shader source empty\n";
        return 0;
    }

    const char* shaderSrc = shaderSource.c_str();

    unsigned int shaderModule = glCreateShader(moduleType);

    if(shaderModule == 0){
        std::cerr << "glCreateShader failed\n";
        return 0;
    }

    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);

    if(!success){
        char infoLog[512];
        glGetShaderInfoLog(shaderModule, 512, NULL, infoLog);

        std::cerr << shaderPath << "\n";
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n";
        std::cerr << infoLog << std::endl;

        return 0;
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

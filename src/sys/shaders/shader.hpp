#ifndef _SHADERS_H_
    #define _SHADERS_H_

    #include <string>

    class ProgramShader{
        //      Methods     //
        public:
            ProgramShader(const char* pathVertex, const char* pathFragment);
            ~ProgramShader();
        private:
            unsigned int compileShader(const std::string& shaderPath, unsigned int moduleType);
            void linkShaders();
        //      Attributes  //
        public:
            unsigned int shaderProgramId;
        private:
            unsigned int vertex, fragment;
    };
#endif

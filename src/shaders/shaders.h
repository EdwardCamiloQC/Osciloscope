#ifndef _SHADERS_H_
    #define _SHADERS_H_

    #include <string>

    class ProgramShaders{
        public:
            ProgramShaders(const char* pathVertex, const char* pathFragment);
            ~ProgramShaders() = default;
        public:
            unsigned int shaderProgramId;
        private:
            unsigned int vertex, fragment;
        private:
            unsigned int compileShaders(const std::string& shaderPath, unsigned int moduleType);
            void linkShaders();
    };
#endif
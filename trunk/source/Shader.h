#ifndef SHADER_H
#define SHADER_H

#include "OGL.h"

class Shader
{
    public:
        Shader(const char* inFile);
        virtual ~Shader();

        inline GLuint handle() { return mHandle; }

    private:
        static char* fileToBuffer(const char* inFile);

        GLuint mHandle;
};

#endif

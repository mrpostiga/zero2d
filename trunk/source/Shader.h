#ifndef SHADER_H
#define SHADER_H

#include "OGL.h"

#include <string>

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

class ShaderException
{
    public:
        ShaderException(const std::string& inReason);
        const std::string reason;
};

#endif

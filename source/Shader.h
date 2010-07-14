#ifndef SHADER_H
#define SHADER_H

#include "OGL.h"

#include <string>
#include <map>

class Shader
{
    public:
        virtual ~Shader();

        inline GLuint handle() { return mHandle; }

        static Shader* load(const char* inFile);
        static void unloadAll();

    private:
        Shader(const char* inFile);

        static char* fileToBuffer(const char* inFile);
        static std::map<std::string, Shader*> mShaders;

        GLuint mHandle;
};

class ShaderException
{
    public:
        ShaderException(const std::string& inReason);
        const std::string reason;
};

#endif

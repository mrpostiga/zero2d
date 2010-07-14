#include "Shader.h"
#include "DisplayEngine.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

map<string, Shader*> Shader::mShaders;

Shader* Shader::load(const char* inFile)
{
    map<string, Shader*>::iterator i = mShaders.find(inFile);

    if (i == mShaders.end())
    {
        Shader* s = new Shader(inFile);
        mShaders[inFile] = s;
        return s;
    }

    return i->second;
}

void Shader::unloadAll()
{
    for (map<string, Shader*>::iterator i = mShaders.begin();
        i != mShaders.end(); ++i)
    {
        delete i->second;
    }
}

char* Shader::fileToBuffer(const char* inFile)
{
    /// TODO: convert to C++ equivalents, remove dependency on cstdio

    char *outBuffer;
    FILE *fptr;
    long length;

    fptr = fopen(inFile, "r");
    if (!fptr) return NULL;
    fseek(fptr, 0, SEEK_END);
    length = ftell(fptr);
    outBuffer = new char[length + 1];
    fseek(fptr, 0, SEEK_SET);
    size_t v;
    v = fread(outBuffer, length, 1, fptr);
    fclose(fptr);
    outBuffer[length] = 0;

    return outBuffer;
}

Shader::Shader(const char* inFile) : mHandle(0)
{
    string s(DisplayEngine::getShaderFolder());
    s += inFile;
    GLenum shaderType = GL_VERTEX_SHADER;
    if (s.find(".fs") != string::npos) shaderType = GL_FRAGMENT_SHADER;

    char* source = fileToBuffer(s.c_str());

    if (source == NULL)
    {
        string e("could not open shader file: ");
        e += s;
        throw ShaderException(e);
    }

    mHandle = glCreateShader(shaderType);
    if (!mHandle)
        throw ShaderException("unable to create shader (glCreateShader)");

    glShaderSource(mHandle, 1, (const GLchar**)&source, 0);
    glCompileShader(mHandle);
    GLint compiled;
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLchar log[2048];
        GLsizei length;
        glGetShaderInfoLog(mHandle, 2048, &length, log);
        throw ShaderException(log);
    }

    delete [] source;
}

Shader::~Shader()
{
    if (mHandle) glDeleteShader(mHandle);
}

ShaderException::ShaderException(const string& inReason) : reason(inReason)
{
}

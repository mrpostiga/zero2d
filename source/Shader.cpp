#include "Shader.h"
#include "DisplayEngine.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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
    fread(outBuffer, length, 1, fptr);
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
    delete [] source;
}

Shader::~Shader()
{
    if (mHandle) glDeleteShader(mHandle);
}

ShaderException::ShaderException(const string& inReason) : reason(inReason)
{
}

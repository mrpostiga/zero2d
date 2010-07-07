#include "Shader.h"

#include <cstdlib>
#include <cstdio>
#include <string>
using namespace std;

char* Shader::fileToBuffer(const char* inFile)
{
    /// TODO: convert to C++ equivalents

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

Shader::Shader(const char* inFile)
{
    string s(inFile);
    GLenum shaderType = GL_VERTEX_SHADER;
    if (s.find(".fs") != string::npos) shaderType = GL_FRAGMENT_SHADER;

    char* source = fileToBuffer(inFile);
    mHandle = glCreateShader(shaderType);
    glShaderSource(mHandle, 1, (const GLchar**)&source, 0);
    glCompileShader(mHandle);
    delete [] source;
}

Shader::~Shader()
{
    glDeleteShader(mHandle);
}

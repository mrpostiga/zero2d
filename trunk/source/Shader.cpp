/**
 *  This file is part of Zero2D.
 *
 *  Zero2D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Zero2D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Zero2D.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        throw Shader::Exception(e);
    }

    mHandle = glCreateShader(shaderType);
    if (!mHandle)
        throw Shader::Exception("unable to create shader (glCreateShader)");

    glShaderSource(mHandle, 1, (const GLchar**)&source, 0);
    glCompileShader(mHandle);
    GLint compiled;
    glGetShaderiv(mHandle, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLchar log[2048];
        GLsizei length;
        glGetShaderInfoLog(mHandle, 2048, &length, log);
        throw Shader::Exception(log);
    }

    delete [] source;
}

Shader::~Shader()
{
    if (mHandle) glDeleteShader(mHandle);
}

Shader::Exception::Exception(const string& inReason) : reason(inReason)
{
}

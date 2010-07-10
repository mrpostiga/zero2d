#include "ShaderProgram.h"

#include <iostream>
using namespace std;

ShaderProgram::ShaderProgram(size_t inCapacity) : mHandle(0),
    mCapacity(inCapacity), mSize(0), mLink(false), mCreate(false)
{
    if (mCapacity < 2) mCapacity = 2;
    mShaders = new Shader*[mCapacity];
}

ShaderProgram::~ShaderProgram()
{
    for (size_t i = 0; i < mSize; ++i)
    {
        glDetachShader(mHandle, mShaders[i]->handle());
        delete mShaders[i];
    }

    glDeleteProgram(mHandle);
    delete [] mShaders;
}

void ShaderProgram::attachShader(Shader* inShader)
{
    if (inShader == NULL || mSize >= mCapacity) return;

    if (!mCreate)
    {
        // moved out of the constructor so that exceptions are thrown at the
        // proper times
        mCreate = true;
        mHandle = glCreateProgram();
        if (!mHandle)
            throw ShaderException("unable to create program (glCreateProgram)");
    }

    glAttachShader(mHandle, inShader->handle());
    mShaders[mSize] = inShader;
    ++mSize;
}

void ShaderProgram::bindAndLink()
{
    // can only be bound and linked once
    if (mLink) return;
    mLink = true;

    GLuint topIndex = 0;

    for (map<string, GLuint>::iterator i = mBindings.begin();
        i != mBindings.end(); ++i)
    {
        i->second = topIndex;
        glBindAttribLocation(mHandle, topIndex, i->first.c_str());
        ++topIndex;
    }

    glLinkProgram(mHandle);

    GLint linked;
    glGetProgramiv(mHandle, GL_LINK_STATUS, &linked);

    if (!linked)
        throw ShaderException("failed to link program (glLinkProgram)");

    mUniformMatrix = glGetUniformLocation(mHandle, "MVPM");
}

void ShaderProgram::setMatrix(const Matrix3D& inMatrix)
{
    glUniformMatrix4fv(mUniformMatrix, 1, GL_TRUE, inMatrix.array());
}

GLuint ShaderProgram::getBinding(const string& inName)
{
    if (!mLink) throw ShaderException("cannot request binding before link");

    map<string, GLuint>::iterator i = mBindings.find(inName);

    if (i == mBindings.end())
    {
        string s("request for non-existent binding: ");
        s += inName;
        throw ShaderException(s);
    }

    return i->second;
}

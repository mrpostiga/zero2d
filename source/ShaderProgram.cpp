#include "ShaderProgram.h"

#include <iostream>
using namespace std;

ShaderProgram::ShaderProgram(size_t inCapacity) : mHandle(0), mTopIndex(0),
    mCapacity(inCapacity), mSize(0), mLink(false)
{
    if (mCapacity < 2) mCapacity = 2;
    mShaders = new Shader*[mCapacity];
    mHandle = glCreateProgram();
    if (!mHandle)
        throw ShaderException("unable to create program (glCreateProgram)");
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

    glAttachShader(mHandle, inShader->handle());
    mShaders[mSize] = inShader;
    ++mSize;
}

void ShaderProgram::bindAttributeLocations(ShaderVBO& inSVBO)
{
    if (mLink) return;

    for (size_t i = 0; i < inSVBO.getSize(); ++i)
    {
        ShaderVBO::DataArray* da = inSVBO.getArrays() + i;
        da->VAindex = mTopIndex;
        glBindAttribLocation(mHandle, mTopIndex, da->attributeBinding.c_str());
        ++mTopIndex;
    }
    glLinkProgram(mHandle);

    mUniformMatrix = glGetUniformLocation(mHandle, "MVPM");
}

void ShaderProgram::setMatrix(const Matrix3D& inMatrix)
{
    glUniformMatrix4fv(mUniformMatrix, 1, GL_TRUE, inMatrix.array());
}

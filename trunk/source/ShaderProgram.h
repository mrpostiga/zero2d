#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "ShaderVBO.h"
#include "Matrix3D.h"

class ShaderProgram
{
    public:
        ShaderProgram(size_t inCapacity = 2);
        virtual ~ShaderProgram();

        void attachShader(Shader* inShader);
        void bindAttributeLocations(ShaderVBO& inSVBO);
        void setMatrix(const Matrix3D& inMatrix);

        inline void use() { glUseProgram(mHandle); }
        inline GLint getUniformLocation(const char* inName)
        {
            return glGetUniformLocation(mHandle, inName);
        }

    protected:
    private:
        GLuint mHandle;
        GLuint mTopIndex;
        Shader** mShaders;
        size_t mCapacity;
        size_t mSize;
        GLint mUniformMatrix;
        bool mLink;
        bool mCreate;
};

#endif

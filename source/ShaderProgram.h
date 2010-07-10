#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Shader.h"
#include "ShaderVBO.h"
#include "Matrix3D.h"

#include <map>

class ShaderProgram
{
    public:
        ShaderProgram(size_t inCapacity = 2);
        virtual ~ShaderProgram();

        void attachShader(Shader* inShader);
        void setMatrix(const Matrix3D& inMatrix);
        GLuint getBinding(const std::string& inName);
        void bindAndLink();

        inline void use() { glUseProgram(mHandle); }

        inline void addVariable(const std::string& inName)
        {
            mBindings[inName] = 0;
        }

        inline GLint getUniformLocation(const char* inName)
        {
            return glGetUniformLocation(mHandle, inName);
        }

    protected:
    private:
        GLuint mHandle;
        Shader** mShaders;
        size_t mCapacity;
        size_t mSize;
        GLint mUniformMatrix;
        bool mLink;
        bool mCreate;

        std::map<std::string, GLuint> mBindings;
};

#endif

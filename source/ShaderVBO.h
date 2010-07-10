#ifndef SHADERVBO_H
#define SHADERVBO_H

#include "OGL.h"

#include <string>
#include <vector>

class ShaderVBO
{
    public:
        ShaderVBO();
        ~ShaderVBO();

        struct DataArray
        {
            GLuint VAindex; // vertex attribute index
            GLuint VBOindex; // vertex buffer object index
            GLuint valuesPerVertex;
        };

        struct IndexArray
        {
            GLenum drawMode;
            GLsizei size;
            GLuint VBOindex;
        };

        void displayIndexed();
        void displayLinear(GLenum inMode, GLint inFirst, GLsizei inCount);
        void loadVAA(GLuint inVAindex, GLuint inValuesPerVertex, GLuint inSize,
            const GLfloat* inData, GLenum inUsage = GL_STATIC_DRAW);
        void loadIA(GLenum inDrawMode, GLsizei inSize, const GLuint* inData,
            GLenum inUsage = GL_STATIC_DRAW);

    protected:
    private:
        std::vector<DataArray> mData;
        IndexArray mIndexArray;
};

#endif

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
            std::string attributeBinding;
        };

        struct IndexArray
        {
            GLenum drawMode;
            GLsizei size;
            GLuint VBOindex;
        };

        void displayIndexed();
        void displayLinear(GLenum inMode, GLint inFirst, GLsizei inCount);
        void loadVAA(const char* inAttribute, GLuint inValuesPerVertex,
            GLuint inSize, const GLfloat* inData,
            GLenum inUsage = GL_STATIC_DRAW);
        void loadIA(GLenum inDrawMode, GLsizei inSize, const GLuint* inData,
            GLenum inUsage = GL_STATIC_DRAW);

        inline DataArray* getArrays() { return &mData[0]; }
        inline size_t getSize() { return mData.size(); }

    protected:
    private:
        std::vector<DataArray> mData;
        IndexArray mIndexArray;
};

#endif

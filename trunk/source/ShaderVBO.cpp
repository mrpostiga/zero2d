#include "ShaderVBO.h"

ShaderVBO::ShaderVBO()
{
    mIndexArray.VBOindex = 0;
}

ShaderVBO::~ShaderVBO()
{
    for (size_t i = 0; i < mData.size(); ++i)
        glDeleteBuffers(1, &mData[i].VBOindex);

    if (mIndexArray.VBOindex > 0)
        glDeleteBuffers(1, &mIndexArray.VBOindex);
}

void ShaderVBO::displayIndexed()
{
    for (size_t i = 0; i < mData.size(); ++i)
    {
        DataArray& da = mData[i];
        glBindBuffer(GL_ARRAY_BUFFER, da.VBOindex);
        glVertexAttribPointer(da.VAindex, da.valuesPerVertex, GL_FLOAT,
            GL_FALSE, 0, 0);
        glEnableVertexAttribArray(da.VAindex);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexArray.VBOindex);
    glDrawElements(mIndexArray.drawMode, mIndexArray.size, GL_UNSIGNED_INT, 0);

    for (size_t i = 0; i < mData.size(); ++i)
    {
        glDisableVertexAttribArray(mData[i].VAindex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ShaderVBO::displayLinear(GLenum inMode, GLint inFirst, GLsizei inCount)
{
    for (size_t i = 0; i < mData.size(); ++i)
    {
        DataArray& da = mData[i];
        glBindBuffer(GL_ARRAY_BUFFER, da.VBOindex);
        glVertexAttribPointer(da.VAindex, da.valuesPerVertex, GL_FLOAT,
            GL_FALSE, 0, 0);
        glEnableVertexAttribArray(da.VAindex);
    }

    glDrawArrays(inMode, inFirst, inCount);

    for (size_t i = 0; i < mData.size(); ++i)
    {
        glDisableVertexAttribArray(mData[i].VAindex);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/// load Vertex Attribute Array
void ShaderVBO::loadVAA(GLuint inVAindex, GLuint inValuesPerVertex,
    GLuint inSize, const GLfloat* inData, GLenum inUsage)
{
    DataArray da;
    da.VAindex = inVAindex;
    da.valuesPerVertex = inValuesPerVertex;
    glGenBuffers(1, &da.VBOindex);
    glBindBuffer(GL_ARRAY_BUFFER, da.VBOindex);
    glBufferData(GL_ARRAY_BUFFER, inSize * sizeof(GLfloat) * inValuesPerVertex, inData, inUsage);
    mData.push_back(da);
}

/// load Index Array
void ShaderVBO::loadIA(GLenum inDrawMode, GLsizei inSize, const GLuint* inData,
    GLenum inUsage)
{
    mIndexArray.drawMode = inDrawMode;
    mIndexArray.size = inSize;
    glGenBuffers(1, &mIndexArray.VBOindex);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexArray.VBOindex);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inSize, inData,
        inUsage);
}

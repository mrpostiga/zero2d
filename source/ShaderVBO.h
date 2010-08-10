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

#ifndef SHADERVBO_H
#define SHADERVBO_H

#include "OGL.h"
#include "IndexVBO.h"

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

        void displayLinear(GLenum inMode, GLint inFirst, GLsizei inCount);
        void displayIndexed(const IndexVBO& inVBO);
        void loadVAA(GLuint inVAindex, GLuint inValuesPerVertex, GLuint inSize,
            const GLfloat* inData, GLenum inUsage = GL_STATIC_DRAW);
        void reloadVAA(GLuint inVAindex, GLuint inSize, const GLfloat* inData,
            GLenum inUsage = GL_STATIC_DRAW);

    protected:
    private:
        std::vector<DataArray> mData;
};

#endif

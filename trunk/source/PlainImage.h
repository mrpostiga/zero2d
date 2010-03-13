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

#ifndef _PLAINIMAGE_H_
#define _PLAINIMAGE_H_

#include "DisplayEngine.h"

class PlainImage
{
    public:
        PlainImage(const char* inFile);
        ~PlainImage();

        void display();
        void setTransparency(GLfloat inLevel);

    private:
        GLuint _texture;
        GLint _filter;
        Vector3D<GLfloat> _color;

        Point _quadUL;
        Point _quadLR;
};

inline void PlainImage::setTransparency(GLfloat inLevel)
{
    _color[3] = inLevel;
}

#endif

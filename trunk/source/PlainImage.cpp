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

#include "PlainImage.h"
#include "DisplayEngine.h"

PlainImage::PlainImage(const char* inFile) : _filter(GL_MODULATE), _color(1.0f)
{
    glGenTextures(1, &_texture);
    Pixel size = DisplayEngine::loadTexture(inFile, _texture);
    _quadUL.x = P2O(size.x / -2);
    _quadUL.y = P2O(size.y / 2);
    _quadLR.x = -_quadUL.x;
    _quadLR.y = -_quadUL.y;
}

PlainImage::~PlainImage()
{
    glDeleteTextures(1, &_texture);
}

void PlainImage::display()
{
    glEnable(GL_TEXTURE_2D);
    TEXTURE_MODE(_filter);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glBegin(GL_QUADS);
    {
        glColor4fv(_color.array());
        glNormal3f(0.0f, 0.0f, 1.0f);

        glTexCoord2i(0, 0);
        glVertex2f(_quadUL.x, _quadUL.y);
        glTexCoord2i(1, 0);
        glVertex2f(_quadLR.x, _quadUL.y);
        glTexCoord2i(1, 1);
        glVertex2f(_quadLR.x, _quadLR.y);
        glTexCoord2i(0, 1);
        glVertex2f(_quadUL.x, _quadLR.y);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

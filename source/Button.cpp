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

#include "Button.h"
#include "DisplayEngine.h"
#include "SpriteProgram.h"

#include <string>
#include <iostream>
using namespace std;

Button::Button(const char* inFile, Point inPosition, Point inSize) : mFirst(0)
{
    mPosition = inPosition;
    mSize = inSize;

    glGenTextures(1, &mTexture);

    string s("data/images/");
    s += inFile;
    DisplayEngine::loadTexture(s.c_str(), mTexture);

    GLfloat data[48];
    float w = inSize[0] / 2.0f;
    float h = inSize[1] / 2.0f;
    for (size_t i = 0; i < 4; ++i)
    {
        size_t j = i * 12;
        data[j + 0] = w;
        data[j + 1] = h;
        data[j + 2] = 0.0f;

        data[j + 3] = w;
        data[j + 4] = -h;
        data[j + 5] = 0.0f;

        data[j + 6] = -w;
        data[j + 7] = -h;
        data[j + 8] = 0.0f;

        data[j + 9] = -w;
        data[j + 10] = h;
        data[j + 11] = 0.0f;
    }

    mVBO.loadVAA(SpriteProgram::VERTEX, 3, 16, data);

    for (size_t i = 0; i < 4; ++i)
    {
        size_t j = i * 8;
        float offset = float(i) * 0.25f;
        data[j + 0] = 1.0f;
        data[j + 1] = offset;
        data[j + 2] = 1.0f;
        data[j + 3] = offset + 0.25f;
        data[j + 4] = 0.0f;
        data[j + 5] = offset + 0.25f;
        data[j + 6] = 0.0f;
        data[j + 7] = offset;
    }

    mVBO.loadVAA(SpriteProgram::TEXTURE, 2, 16, data);

    mMatrix.translate(mPosition[0], mPosition[1], 0.0f);
}

Button::~Button()
{
    glDeleteTextures(1, &mTexture);
}

void Button::display()
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
    mVBO.displayLinear(GL_QUADS, mFirst, 4);
}

void Button::onMouseStateChange()
{
    if (isDisabled())
    {
        mFirst = 12;
    }
    else
    {
        switch (mCurrentState)
        {
            case Widget::OUT: { mFirst = 0; break; }
            case Widget::HOVER: { mFirst = 4; break; }
            case Widget::PRESS: { mFirst = 8; break; }
            default: {}
        }
    }
}

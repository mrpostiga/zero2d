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

#include "TestModule.h"
#include "DisplayEngine.h"
#include "Sprite.h"

#include <SDL_opengl.h>

bool TestModule::onLoad()
{
    _sub = Sprite::load("data/fighters/subzero");
    return true;
}

void TestModule::onOpen()
{
    DisplayEngine::ortho(4.0);
    glEnable(GL_TEXTURE_2D);
}

void TestModule::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);

    _sub->bindTexture();
    glBegin(GL_QUADS);
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2i(0, 1);
        glVertex2f(-4.0f, -0.5f);
        glTexCoord2i(0, 0);
        glVertex2f(-4.0f, 0.5f);
        glTexCoord2i(1, 0);
        glVertex2f(4.0f, 0.5f);
        glTexCoord2i(1, 1);
        glVertex2f(4.0f, -0.5f);
    }
    glEnd();
}

void TestModule::onFrame()
{
}

void TestModule::onClose()
{
    glDisable(GL_TEXTURE_2D);
}

void TestModule::onUnload()
{
    Sprite::unloadAll();
}

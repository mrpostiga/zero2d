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

#include "IntroPowerModule.h"
#include "TestModule.h"
#include "DisplayEngine.h"

bool IntroPowerModule::onLoad()
{
    _phase = 0;
    _freezeFrames = 0;
    _next = new TestModule;

    glGenTextures(1, &_logoID);
    DisplayEngine::loadTexture("data/images/sdl-powered.png", _logoID);

    return true;
}

void IntroPowerModule::onOpen()
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat whiteLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);

    glEnable(GL_LIGHT0);
    for (int i = 0; i < 4; ++i)
    {
        _pos[i] = 0.0f;
        _ambient[i] = 0.0f;
    }
    _ambient[3] = 1.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    DisplayEngine::ortho(1.0);

    glLoadIdentity();
}

void IntroPowerModule::onClose()
{
    glDisable(GL_BLEND);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void IntroPowerModule::onUnload()
{
    glDeleteTextures(1, &_logoID);
}

void IntroPowerModule::onPulse()
{
    switch (_phase)
    {
        case 0:
        {
            ++_freezeFrames;
            if (_freezeFrames > 30)
            {
                _freezeFrames = 0;
                ++_phase;
            }
            break;
        }

        case 1:
        {
            for (int i = 0; i < 3; ++i) _ambient[i] += 0.01f;

            if (_ambient[0] > 1.0f)
            {
                for (int i = 0; i < 3; ++i) _ambient[i] = 1.0f;

                ++_phase;
            }
            break;
        }

        case 2:
        {
            ++_freezeFrames;
            if (_freezeFrames > 120)
            {
                _freezeFrames = 0;
                ++_phase;
            }
            break;
        }

        case 3:
        {
            for (int i = 0; i < 3; ++i) _ambient[i] -= 0.01f;

            if (_ambient[0] < 0.0f)
            {
                for (int i = 0; i < 3; ++i) _ambient[i] = 0.0f;
                ++_phase;
            }
            break;
        }

        case 4:
        {
            ++_freezeFrames;
            if (_freezeFrames > 30)
            {
                _freezeFrames = 0;
                ++_phase;
            }
            break;
        }

        default:
        {
            _running = false;
            break;
        }
    }
}

void IntroPowerModule::onRender()
{
    glLightfv(GL_LIGHT0, GL_AMBIENT, _ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, _pos);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _logoID);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    {
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2i(0, 1);
        glVertex2f(-2.0f, -1.0f);
        glTexCoord2i(0, 0);
        glVertex2f(-2.0f, 1.0f);
        glTexCoord2i(1, 0);
        glVertex2f(2.0f, 1.0f);
        glTexCoord2i(1, 1);
        glVertex2f(2.0f, -1.0f);
    }
    glEnd();
}

void IntroPowerModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    _running = false;
}

void IntroPowerModule::onLButtonDown(int inX, int inY)
{
    _running = false;
}

void IntroPowerModule::onRButtonDown(int inX, int inY)
{
    _running = false;
}

void IntroPowerModule::onJoyButtonDown(Uint8 inWhich, Uint8 inButton)
{
    _running = false;
}

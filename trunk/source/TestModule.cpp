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
#include "Entity.h"

#include <SDL_opengl.h>

#define O_RANGE 40.0

Entity* TestModule::luaEntity = NULL;

int TestModule::luaShowArgs(lua_State* inState)
{
    int argc = lua_gettop(inState);

    cerr << "showArgs() called with " << argc << " arguments:" << endl;

    for (int n = 1; n <= argc; ++n)
    {
        cerr << "-- argument " << n << ": " << lua_tostring(inState, n) << endl;
    }

    lua_pushnumber(inState, 123); // return value
    return 1; // number of return values
}

int TestModule::luaSetColor(lua_State* inState)
{
    int outSuccess = 1;

    int argc = lua_gettop(inState);
    if (argc == 1)
    {
        Vector3D<float> color(lua_tonumber(inState, 1));
        luaEntity->setColorMod(color);
    }
    else if (argc >= 3)
    {
        Vector3D<float> color;
        for (int n = 1; n <= argc; ++n)
        {
            color[n - 1] = lua_tonumber(inState, n);
        }
        luaEntity->setColorMod(color);
    }
    else
    {
        outSuccess = 0;
    }

    lua_pushnumber(inState, outSuccess);
    return 1;
}

int TestModule::luaSetBlink(lua_State* inState)
{
    int outSuccess = 1;

    int argc = lua_gettop(inState);
    if (argc > 3)
    {
        int d = int(lua_tonumber(inState, 1));
        Vector3D<float> c(lua_tonumber(inState, 2), lua_tonumber(inState, 3),
            lua_tonumber(inState, 4));
        luaEntity->setBlink(c, d);
    }
    else
    {
        luaEntity->stopBlink();
    }

    lua_pushnumber(inState, outSuccess);
    return 1;
}

int TestModule::luaSetLocation(lua_State* inState)
{
    int outSuccess = 1;

    int argc = lua_gettop(inState);
    if (argc >= 2)
    {
        luaEntity->setLocation(lua_tonumber(inState, 1),
            lua_tonumber(inState, 2));
    }
    else
    {
        outSuccess = 0;
    }

    lua_pushnumber(inState, outSuccess);
    return 1;
}

bool TestModule::onLoad()
{
    _sub = new Entity(Sprite::load("data/fighters/subzero"));
    _cameraZoom = 1.0f;
    _consoleActive = false;
    _consoleOutput.loadFont("data/fonts/DejaVuSans.ttf", 16);

    _lua.addFunction("showArgs", luaShowArgs);
    _lua.addFunction("setColor", luaSetColor);
    _lua.addFunction("setBlink", luaSetBlink);
    _lua.addFunction("setLocation", luaSetLocation);
    return true;
}

void TestModule::onOpen()
{
    luaEntity = _sub;

    DisplayEngine::ortho(O_RANGE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TestModule::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glScalef(_cameraZoom, _cameraZoom, 1.0f);

    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(-10.0f, 0.0f); glVertex2f(10.0f, 0.0f);
        glVertex2f(0.0f, -10.0f); glVertex2f(0.0f, 10.0f);
    }
    glEnd();

    _sub->draw();
    glPopMatrix();

    if (_consoleActive)
        _consoleOutput.draw(-0.9f * O_RANGE, -0.4f * O_RANGE, O_RANGE / 16.0f);
}

void TestModule::onPulse()
{
    _sub->pulse();
}

void TestModule::onClose()
{
    glDisable(GL_BLEND);
}

void TestModule::onUnload()
{
    Entity::unloadAll();
    Sprite::unloadAll();
}

void TestModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    if (_consoleActive)
        onConsoleKey(inSym, inMod);
    else
        onOtherKey(inSym, inMod);
}

void TestModule::onMouseWheelUp()
{
    _cameraZoom += 0.1f;
}

void TestModule::onMouseWheelDown()
{
    _cameraZoom -= 0.1f;
    if (_cameraZoom < 0.0f) _cameraZoom = 0.0f;
}

void TestModule::onConsoleKey(SDLKey inSym, SDLMod inMod)
{
    switch (inSym)
    {
        case SDLK_BACKSPACE:
        {
            int l = _consoleInput.length();
            if (l > 0) _consoleInput = _consoleInput.substr(0, l - 1);
            updateConsole();
            break;
        }

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
        {
            if (_consoleInput.length() > 1)
                _lua.runCommand(_consoleInput.c_str());
        }
        case SDLK_ESCAPE:
        {
            _consoleActive = false;
            break;
        }

        default:
        {
            int c = DisplayEngine::processKey(inSym, inMod);
            if (c > 0) _consoleInput += c;
            updateConsole();
        }
    }
}

void TestModule::onOtherKey(SDLKey inSym, SDLMod inMod)
{
    switch (inSym)
    {
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
        {
            _consoleInput = "";
            _consoleActive = true;
            updateConsole();
            break;
        }

        case SDLK_ESCAPE:
        {
            _running = false;
            break;
        }

        case SDLK_SPACE:
        {
            _sub->flip();
            break;
        }

        case SDLK_PAGEUP:
        {
            _sub->rotate(-15.0f);
            break;
        }

        case SDLK_PAGEDOWN:
        {
            _sub->rotate(15.0f);
            break;
        }

        default:
        {
        }
    }
}

void TestModule::updateConsole()
{
    if (!_consoleActive) return;

    string s("LUA : ");
    s += _consoleInput;
    _consoleOutput.setText(s);
}

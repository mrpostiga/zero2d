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

#include "TextureModule.h"
#include "DisplayEngine.h"

#include <iostream>
using namespace std;

#define NUM_POINTS 4
#define NUM_PARTICLES 1000

float RV()
{
    return (float)rand() / RAND_MAX;
}

TextureModule::TextureModule()
{
}

TextureModule::~TextureModule()
{
}

void TextureModule::onLoad()
{
    mLoadScreen.setBackgroundImage("dragon.png");
    mLoadScreen.setLoadImage("loading.png");
    mLoadScreen.setLoadLocation(0, -200);
    mLoadScreen.setup();

    mLoadScreen.update(0);

    //set all the inputs
    for (int i = 0; i < SDLK_LAST; ++i)
    {
        mKeyboardInputs[i].player = NULL;
    }


    mLuaMachine.addFunction("zero2d_api_test", luaTest);
    mLuaMachine.runFile("data/scripts/api.lua");
    mLuaMachine.runFile("data/scripts/test.lua");

    //glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mBackTexture);
    DisplayEngine::loadTexture("data/images/dragon.png", mBackTexture);
    mLoadScreen.update(50);

    GLfloat* vertices = new GLfloat[NUM_PARTICLES * 3];
    GLfloat* colors = new GLfloat[NUM_PARTICLES * 3];
    GLfloat* velocities = new GLfloat[NUM_PARTICLES * 3];
    GLfloat* startTimes = new GLfloat[NUM_PARTICLES];
    mLoadScreen.update(75);

    srand(time(NULL));
    for (size_t i = 0; i < NUM_PARTICLES; ++i)
    {
        size_t j = i * 3;

        vertices[j] = 0.0;
        vertices[j + 1] = 0.0f;
        vertices[j + 2] = 0.0f;

        colors[j] = RV();
        colors[j + 1] = RV();
        colors[j + 2] = RV();

        velocities[j] = 2.0f * RV() - 1.0f;
        velocities[j + 1] = 4.0f * RV();
        velocities[j + 2] = 2.0f * RV() - 1.0f;

        startTimes[i] = RV() * 1.0f;
    }

    mLoadScreen.update(80);

    mParticleVBO.loadVAA(ParticleProgram::VERTEX, 3, NUM_PARTICLES, vertices);
    mParticleVBO.loadVAA(ParticleProgram::COLOR, 3, NUM_PARTICLES, colors);
    mParticleVBO.loadVAA(ParticleProgram::VELOCITY, 3, NUM_PARTICLES,
        velocities);
    mParticleVBO.loadVAA(ParticleProgram::START_TIME, 1, NUM_PARTICLES,
        startTimes);

    mLoadScreen.update(82);

    vertices[0] = 1024.0f;
    vertices[1] = 1024.0f;
    vertices[2] = 1024.0f;
    vertices[3] = -1024.0f;
    vertices[4] = -1024.0f;
    vertices[5] = -1024.0f;
    vertices[6] = -1024.0f;
    vertices[7] = 1024.0f;

    colors[0] = 1.0f;
    colors[1] = 0.0f;
    colors[2] = 1.0f;
    colors[3] = 1.0f;
    colors[4] = 0.0f;
    colors[5] = 1.0f;
    colors[6] = 0.0f;
    colors[7] = 0.0f;

    mBackVBO.loadVAA(SpriteProgram::VERTEX, 2, 4, vertices);
    mBackVBO.loadVAA(SpriteProgram::TEXTURE, 2, 4, colors);
    mLoadScreen.update(87);

    delete [] vertices;
    delete [] colors;
    delete [] velocities;
    delete [] startTimes;

    mSpriteInstance = new SpriteInstance(new Sprite("pimple"));
    //mSpriteInstance = new SpriteInstance(new Sprite("pimple"));
    mLoadScreen.update(90);

    mFade = 1.0f;
    mFading = false;

    mTickStart = SDL_GetTicks();
    glPointSize(1.0f);

    mSpriteProgram.use();
    mSpriteProgram.setFade(1.0f);
    mLoadScreen.update(92);

    float ratio = DisplayEngine::getAspectRatio();
    mProjection.orthographic(360.0f, ratio);
    //mProjection.perspective(30.0f, ratio, 1.0f, 100.0f);
    //mModelView.matrix().translate(0.0f, 0.0f, -5.0f);

    mPlayerControl = new PlayerControl(new Fighter("pimple"));

    setupInputs();

    mRotation = 0;
    mLoadScreen.update(100);
}

void TextureModule::setupInputs()
{
    ifstream input;
    input.open("data/key_config.txt");
    string nextLine;
    string event;
    char equals;
    int value;
    SDLKey key;
    stringstream parse;

    getline(input, nextLine);
    while (!input.eof())
    {
        parse << nextLine;
        parse >> event >> equals >> value;
        key = (SDLKey)value;
        cerr << "key: " << event << " value: " << SDL_GetKeyName(key) << endl;
        parse.clear();
        getline(input, nextLine);

        mKeyboardInputs[value].event = getEvent(event);
        mKeyboardInputs[value].player = mPlayerControl;

    }
}

State::Event TextureModule::getEvent(string inEvent)
{
    State::Event result = State::ON_END;

    if (inEvent == "attack")
    {
        result = State::ATTACK;
    }
    else if (inEvent == "defend")
    {
        result = State::DEFEND;
    }
    else if (inEvent == "jump")
    {
        result = State::JUMP;
    }
    else if (inEvent == "tilt-right")
    {
        result = State::TILT_RIGHT;
    }
    else if (inEvent == "tilt-left")
    {
        result = State::TILT_LEFT;
    }
    else if (inEvent == "tilt-up")
    {
        result = State::TILT_UP;
    }
    else if (inEvent == "tilt-down")
    {
        result = State::TILT_DOWN;
    }
    else if (inEvent == "smash-right")
    {
        result = State::SMASH_RIGHT;
    }
    else if (inEvent == "smash-left")
    {
        result = State::SMASH_LEFT;
    }
    else if (inEvent == "smash-down")
    {
        result = State::SMASH_DOWN;
    }
    else if (inEvent == "smash-up")
    {
        result = State::SMASH_UP;
    }

    return result;
}

SDLKey TextureModule::getBinding(string inBinding)
{
    int key = 0;

    stringstream ss;
    //ss << value;
    //ss >> key;

    return (SDLKey)key;
}


void TextureModule::onOpen()
{
    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    mModelView.reset();
}

void TextureModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModelView.push();
    mModelView.matrix().rotateZ(mRotation);

    mModelView.matrix().multiply(mCamera.matrix());
    (mMVPM = mProjection).multiply(mModelView.matrix());

    mSpriteProgram.use();
    mSpriteProgram.setMatrix(mMVPM);
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mBackTexture);
    mBackVBO.displayLinear(GL_QUADS, 0, 4);
    mPlayerControl->getEntity()->display(mMVPM);
    //mSpriteInstance->display();

    mParticleProgram.use();
    mModelView.matrix().scale(60.0f);
    (mMVPM = mProjection).multiply(mModelView.matrix());
    mParticleProgram.setMatrix(mMVPM);
    mParticleVBO.displayLinear(GL_POINTS, 0, NUM_PARTICLES);

    mModelView.pop();
}

void TextureModule::onFrame()
{
    mCamera.update();
    //mSpriteInstance->update();
    mPlayerControl->update();
    mParticleProgram.use();
    mParticleProgram.setTime(float(SDL_GetTicks() - mTickStart) * 0.0006f);

    if (mFading)
    {
        mFade -= 0.01;
        if (mFade < 0.0f)
        {
            mFade = 1.0f;
            mFading = false;
        }
    }

    mSpriteProgram.use();
    mSpriteProgram.setFade(mFade);

    //mRotation += 1.0f;
    //if (mRotation > 180.0f) mRotation -= 360.0f;
}

void TextureModule::onClose()
{
    glDisable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void TextureModule::onUnload()
{
    delete mSpriteInstance;
    glDeleteTextures(1, &mBackTexture);
}

void TextureModule::onMouseWheel(bool inUp, bool inDown)
{
    mCamera.zoom(inUp ? 0.1f : -0.1f);
}

void TextureModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    if (mKeyboardInputs[inSym].player != NULL)
    {
        mKeyboardInputs[inSym].player->onEvent(mKeyboardInputs[inSym].event);
    }

    switch (inSym)
    {
        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        case SDLK_SPACE:
        {
            mTickStart = SDL_GetTicks();
            mFading = true;
            break;
        }

        default:
        {
            break;
        }
    }
}

void TextureModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    if (mKeyboardInputs[inSym].player != NULL)
    {
        mKeyboardInputs[inSym].player->onEvent(State::ON_END);
    }

    switch (inSym)
    {

        default:
        {
            break;
        }
    }
}

void TextureModule::onJoyAxis(Uint8 inWhich, Uint8 inAxis, Sint16 inValue)
{
    cerr << "joystick: " << inWhich << " axis: " << inAxis << " value: " << inValue << endl;
}

void TextureModule::onJoyButtonDown(Uint8 inWhich, Uint8 inButton)
{
    cerr << "joystick: " << inWhich << "button: " << inButton << endl;
}

void TextureModule::onJoyButtonUp(Uint8 inWhich, Uint8 inButton)
{

}

void TextureModule::onJoyHat(Uint8 inWhich, Uint8 inHat, Uint8 inValue)
{
    cerr << "joystick: " << inWhich << " hat: " << inHat << " value: " << inValue << endl;
}

/// Lua interfacing
int TextureModule::luaTest(LuaState inState)
{
    int argc = lua_gettop(inState);

    if (argc)
    {
        for (int i = 0; i < argc; ++i)
        {
            const char* s = lua_tostring(inState, i + 1);
            cout << "Lua function parameter: " << s << endl;
        }
    }

    return 0;
}

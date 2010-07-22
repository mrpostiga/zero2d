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

    mLuaMachine.addFunction("z2dTest", luaTest);
    mLuaMachine.loadFile("data/scripts/test.lua");
    mLoadScreen.update(10);

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

    mRotation = 0;
    mLoadScreen.update(100);
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
    //mModelView.matrix().rotateZ(mRotation);

    mModelView.matrix().multiply(mCamera.matrix());
    (mMVPM = mProjection).multiply(mModelView.matrix());
    mSpriteProgram.setMatrix(mMVPM);

    mSpriteProgram.use();
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mBackTexture);
    mBackVBO.displayLinear(GL_QUADS, 0, 4);
    mSpriteInstance->display();

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
    mSpriteInstance->update();
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

        case SDLK_PAGEDOWN:
        {
            mSpriteInstance->onEvent(State::TILT_FORWARD);
            break;
        }

        case SDLK_PAGEUP:
        {
            mSpriteInstance->onEvent(State::ATTACK);
            break;
        }

        case SDLK_LEFT:
        {
            mSpriteInstance->faceRight(false);
            mSpriteInstance->onEvent(State::TILT_FORWARD);
            break;
        }

        case SDLK_RIGHT:
        {
            mSpriteInstance->faceRight(true);
            mSpriteInstance->onEvent(State::TILT_FORWARD);
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
    switch (inSym)
    {
        case SDLK_PAGEDOWN:
        {
            mSpriteInstance->onEvent(State::ON_END);
            break;
        }

        case SDLK_PAGEUP:
        {
            mSpriteInstance->onEvent(State::ON_END);
            break;
        }

        case SDLK_LEFT:
        {
            mSpriteInstance->onEvent(State::ON_END);
            break;
        }

        case SDLK_RIGHT:
        {
            mSpriteInstance->onEvent(State::ON_END);
            break;
        }

        default:
        {
            break;
        }
    }
}

/// Lua interfacing
int TextureModule::luaTest(LuaState inState)
{
    int argc = lua_gettop(inState);

    if (argc)
    {
        const char* s = lua_tostring(inState, 1);
        cout << "Lua input: " << s << endl;
    }

    return 0;
}

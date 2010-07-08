#include "TestModule.h"

#include <ctime>
#include <iostream>
using namespace std;

#define NUM_PARTICLES 1000

float randomValue()
{
    return (float)rand() / RAND_MAX;
}

TestModule::TestModule()
{
}

TestModule::~TestModule()
{
}

bool TestModule::onLoad()
{
    try
    {
        mSP.attachShader(new Shader("test2-particles.vs"));
        mSP.attachShader(new Shader("test2-particles.fs"));

        GLfloat* vertices = new GLfloat[NUM_PARTICLES * 3];
        GLfloat* colors = new GLfloat[NUM_PARTICLES * 3];
        GLfloat* velocities = new GLfloat[NUM_PARTICLES * 3];
        GLfloat* startTimes = new GLfloat[NUM_PARTICLES];

        srand(time(NULL));
        for (size_t i = 0; i < NUM_PARTICLES; ++i)
        {
            size_t j = i * 3;

            vertices[j] = 0.0;
            vertices[j + 1] = 0.0f;
            vertices[j + 2] = 0.0f;

            colors[j] = randomValue();
            colors[j + 1] = randomValue();
            colors[j + 2] = randomValue();

            velocities[j] = 2.0f * randomValue() - 1.0f;
            velocities[j + 1] = 4.0f * randomValue();
            velocities[j + 2] = 2.0f * randomValue() - 1.0f;

            startTimes[i] = randomValue() * 1.0f;
        }

        mSVBO.loadVAA("MCVertex", 3, NUM_PARTICLES, vertices);
        mSVBO.loadVAA("MColor", 3, NUM_PARTICLES, colors);
        mSVBO.loadVAA("Velocity", 3, NUM_PARTICLES, velocities);
        mSVBO.loadVAA("StartTime", 1, NUM_PARTICLES, startTimes);
        mSP.bindAttributeLocations(mSVBO);

        delete [] vertices;
        delete [] colors;
        delete [] velocities;
        delete [] startTimes;
    }
    catch (ShaderException& se)
    {
        cerr << "shader exception -- " << se.reason << endl;
        return false;
    }
    catch(...)
    {
        cerr << "unknown exception" << endl;
        return false;
    }

    mT = mSP.getUniformLocation("Time");

    mSP.use();

    mRotation = 0.0f;
    mTime = 0.0f;

    glPointSize(1.5f);

    float ratio = float(SDL_GetVideoSurface()->w)
        / float(SDL_GetVideoSurface()->h);
    mProjection.orthographic(1.5f, ratio);
    //mProjection.perspective(30.0f, ratio, 1.0f, 100.0f);
    //mModelView.matrix().translate(0.0f, 0.0f, -5.0f);

    return true;
}

void TestModule::onUnload()
{
}

void TestModule::onOpen()
{
    mTickStart = SDL_GetTicks();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TestModule::onClose()
{
    glDisable(GL_BLEND);
}

void TestModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModelView.push();
    mModelView.matrix().rotateY(mRotation);

    mMVPM = mProjection;
    mMVPM.multiply(mModelView.matrix());
    mSP.setMatrix(mMVPM);

    mSVBO.displayLinear(GL_POINTS, 0, NUM_PARTICLES);
    mModelView.pop();
}

void TestModule::onFrame()
{
    //mRotation += 5.0f;
    if (mRotation > 180.0f) mRotation -= 360.0f;

    glUniform1f(mT, float(SDL_GetTicks() - mTickStart) * 0.0004f);
}

void TestModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
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
            break;
        }

        default:
        {
            break;
        }
    }
}

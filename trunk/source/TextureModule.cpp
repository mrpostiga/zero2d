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


bool TextureModule::onLoad()
{
    mLoadScreen.setBackgroundImage("dragon.png");
    mLoadScreen.setLoadImage("loading.png");
    mLoadScreen.setLoadLocation(0, -200);
    mLoadScreen.setup();

    mLoadScreen.update(0);
    try
    {
        //glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &mBackTexture);
        DisplayEngine::loadTexture("data/images/dragon.png",
            mBackTexture);
        mLoadScreen.update(50);

        mParticleProgram.attachShader(new Shader("test2-particles.vs"));
        mLoadScreen.update(60);
        mParticleProgram.attachShader(new Shader("test2-particles.fs"));
        mLoadScreen.update(70);

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
        mLoadScreen.update(77);


        mParticleProgram.addVariable("MCVertex");
        mParticleProgram.addVariable("MColor");
        mParticleProgram.addVariable("Velocity");
        mParticleProgram.addVariable("StartTime");
        mParticleProgram.bindAndLink();
        mLoadScreen.update(80);

        mParticleVBO.loadVAA(mParticleProgram.getBinding("MCVertex"), 3,
            NUM_PARTICLES, vertices);
        mParticleVBO.loadVAA(mParticleProgram.getBinding("MColor"), 3,
            NUM_PARTICLES, colors);
        mParticleVBO.loadVAA(mParticleProgram.getBinding("Velocity"), 3,
            NUM_PARTICLES, velocities);
        mParticleVBO.loadVAA(mParticleProgram.getBinding("StartTime"), 1,
            NUM_PARTICLES, startTimes);
        mLoadScreen.update(82);

        mSpriteProgram.attachShader(new Shader("sprite.vs"));
        mSpriteProgram.attachShader(new Shader("sprite.fs"));
        mSpriteProgram.addVariable("CornerVertex");
        mSpriteProgram.addVariable("TexCoord");
        mSpriteProgram.bindAndLink();
        mLoadScreen.update(85);

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

        mBackVBO.loadVAA(mSpriteProgram.getBinding("CornerVertex"), 2, 4,
            vertices);
        mBackVBO.loadVAA(mSpriteProgram.getBinding("TexCoord"), 2, 4,
            colors);
        mLoadScreen.update(87);

        delete [] vertices;
        delete [] colors;
        delete [] velocities;
        delete [] startTimes;

        Sprite::setProgram(&mSpriteProgram);
        mSprite = new Sprite("pimple");
        mLoadScreen.update(90);

        GLint texLoc = mSpriteProgram.getUniformLocation("Texture");
        glUniform1i(texLoc, 0);

        GLint z = mSpriteProgram.getUniformLocation("z");
        glUniform1f(z, 0.0f);

        mFadeShader = mSpriteProgram.getUniformLocation("fade");
        mFade = 1.0f;
        mFading = false;

        mT = mParticleProgram.getUniformLocation("Time");
        mTickStart = SDL_GetTicks();
        glPointSize(1.0f);

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

    mSpriteProgram.use();
    mLoadScreen.update(92);

    float ratio = DisplayEngine::getAspectRatio();
    mProjection.orthographic(360.0f, ratio);
    //mProjection.perspective(30.0f, ratio, 1.0f, 100.0f);
    //mModelView.matrix().translate(0.0f, 0.0f, -5.0f);

    animation[0] = 2;
    animation[1] = 3;
    animation[2] = 4;
    animation[3] = 3;
    animation[4] = 5;
    animation[5] = 6;
    animation[6] = 7;
    animation[7] = 6;
    mCounter = 0;
    mCurrentIndex = 0;
    mRotation = 0;
    mLoadScreen.update(95);

    glUniform1f(mFadeShader, mFade);
    mLoadScreen.update(100);


    return true;
}

void TextureModule::onOpen()
{
    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform1f(mFadeShader, 1.0);
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
    mSprite->draw(animation[mCurrentIndex], true);

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
    mParticleProgram.use();
    glUniform1f(mT, float(SDL_GetTicks() - mTickStart) * 0.0006f);

    if (mCounter >= 4)
    {
        mCounter = 0;
        mCurrentIndex = (mCurrentIndex + 1) % 8;
    }
    else
    {
        ++mCounter;
    }

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
    glUniform1f(mFadeShader, mFade);

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
    delete mSprite;
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

        default:
        {
            break;
        }
    }
}

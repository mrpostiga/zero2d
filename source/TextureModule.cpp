#include "TextureModule.h"
#include "DisplayEngine.h"

#include <iostream>
using namespace std;

#define NUM_POINTS 4

TextureModule::TextureModule()
{
}

TextureModule::~TextureModule()
{
}


bool TextureModule::onLoad()
{
    try
    {
        mSprite = new Sprite("pimple");

        glGenTextures(1, &mTexture);
        DisplayEngine::loadTexture("data/sprites/pimple/sheet-0.png", mTexture);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTexture);
        mSP.attachShader(new Shader("sprite.vs"));
        mSP.attachShader(new Shader("sprite.fs"));

        GLfloat vertices[12] = {-4, 4, 0, 4, 4, 0,
                                4, -4, 0, -4, -4, 0};
        GLfloat texCoords[8] = {0, 0, 1, 0,
                                1, 1, 0, 1};

        mSVBO.loadVAA("MCVertex", 3, NUM_POINTS, vertices);
        mSVBO.loadVAA("TexCoord", 2, NUM_POINTS, texCoords);
        mSP.bindAttributeLocations(mSVBO);

        GLint texLoc = mSP.getUniformLocation("Texture");
        mFadeShader = mSP.getUniformLocation("fade");
        mFade = 1.0f;
        mFading = false;
        glUniform1i(texLoc, 0);

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

    mSP.use();

    float ratio = DisplayEngine::getAspectRatio();
    mProjection.orthographic(100.0f, ratio);
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

    return true;

}

void TextureModule::onOpen()
{
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUniform1f(mFadeShader, 1.0);
    mModelView.reset();
}

void TextureModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModelView.push();
    mModelView.matrix().rotateZ(mRotation);

    (mMVPM = mProjection).multiply(mModelView.matrix());
    mSP.setMatrix(mMVPM);
    mSprite->draw(animation[mCurrentIndex], true);

    mModelView.pop();
}

void TextureModule::onFrame()
{
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
        glUniform1f(mFadeShader, mFade);
    }

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
            mFading = true;
            break;
        }

        default:
        {
            break;
        }
    }
}

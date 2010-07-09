#include "TextureModule.h"
#include "DisplayEngine.h"

#include <iostream>
using namespace std;

#define NUM_PARTICLES 4

TextureModule::TextureModule()
{
}

TextureModule::~TextureModule()
{
    //dtor
}


bool TextureModule::onLoad()
{
    glEnable(GL_TEXTURE_2D);
    try
    {
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

        mSVBO.loadVAA("MCVertex", 3, NUM_PARTICLES, vertices);
        mSVBO.loadVAA("TexCoord", 2, NUM_PARTICLES, texCoords);
        mSP.bindAttributeLocations(mSVBO);

        GLint texLoc = mSP.getUniformLocation("Texture");
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

    //mT = mSP.getUniformLocation("Time");

    mSP.use();

    //glPointSize(1.5f);

    float ratio = float(SDL_GetVideoSurface()->w)
        / float(SDL_GetVideoSurface()->h);
    mProjection.orthographic(5.0f, ratio);
    //mProjection.perspective(30.0f, ratio, 1.0f, 100.0f);
    //mModelView.matrix().translate(0.0f, 0.0f, -5.0f);

    return true;

}

void TextureModule::onOpen()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TextureModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModelView.push();

    mMVPM = mProjection;
    mMVPM.multiply(mModelView.matrix());
    mMVPM.rotateZ(mRotation);
    mSP.setMatrix(mMVPM);

    mSVBO.displayLinear(GL_QUADS, 0, NUM_PARTICLES);
    mModelView.pop();
}

void TextureModule::onFrame()
{
    mRotation += 1.0f;
    if (mRotation > 180.0f) mRotation -= 360.0f;

    //glUniform1f(mT, float(SDL_GetTicks() - mTickStart) * 0.0004f);
}

void TextureModule::onClose()
{
    glDisable(GL_BLEND);
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

            break;
        }

        default:
        {
            break;
        }
    }
}

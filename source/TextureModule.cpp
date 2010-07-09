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

    return true;

}

void TextureModule::onOpen()
{
    glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TextureModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mModelView.push();
    mModelView.matrix().rotateZ(mRotation);

    (mMVPM = mProjection).multiply(mModelView.matrix());
    mSP.setMatrix(mMVPM);
    mSprite->draw(6, true);

    mModelView.pop();
}

void TextureModule::onFrame()
{
    mRotation += 1.0f;
    if (mRotation > 180.0f) mRotation -= 360.0f;
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

            break;
        }

        default:
        {
            break;
        }
    }
}

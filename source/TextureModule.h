#ifndef TEXTUREMODULE_H
#define TEXTUREMODULE_H

#include "Module.h"
#include "MatrixStack.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Camera.h"
#include "LoadScreen.h"

#include "OGL.h"


class TextureModule : public Module
{
    public:
        TextureModule();
        virtual ~TextureModule();

        /// module operation
        virtual bool onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:

        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onMouseWheel(bool inUp, bool inDown);

    private:
        GLuint mT;

        int animation[8];
        int mCounter;
        int mCurrentIndex;

        GLint mFadeShader;
        float mFade;
        bool mFading;

        float mRotation;
        Matrix3D mProjection;
        Matrix3D mMVPM;
        MatrixStack mModelView;
        ShaderProgram mParticleProgram;
        ShaderVBO mParticleVBO;
        ShaderProgram mSpriteProgram;
        ShaderVBO mBackVBO;
        GLuint mBackTexture;
        Sprite* mSprite;
        Uint32 mTickStart;
        Camera mCamera;

        LoadScreen mLoadScreen;
};

#endif

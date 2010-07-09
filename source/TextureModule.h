#ifndef TEXTUREMODULE_H
#define TEXTUREMODULE_H

#include "Module.h"
#include "MatrixStack.h"
#include "ShaderProgram.h"
#include "Sprite.h"

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

    private:
        GLuint mT;

        float mRotation;
        Matrix3D mProjection;
        Matrix3D mMVPM;
        MatrixStack mModelView;
        ShaderProgram mSP;
        ShaderVBO mSVBO;
        Sprite* mSprite;

        GLuint mTexture;
};

#endif

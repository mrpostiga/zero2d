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

#ifndef TEXTUREMODULE_H
#define TEXTUREMODULE_H

#include "Module.h"
#include "MatrixStack.h"
#include "SpriteInstance.h"
#include "SpriteProgram.h"
#include "ParticleProgram.h"
#include "Camera.h"
#include "LoadScreen.h"
#include "LuaMachine.h"

#include "OGL.h"


class TextureModule : public Module
{
    public:
        TextureModule();
        virtual ~TextureModule();

        /// module operation
        virtual void onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:

        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onMouseWheel(bool inUp, bool inDown);

    private:
        int animation[8];
        int mCounter;
        int mCurrentIndex;

        float mFade;
        bool mFading;

        float mRotation;
        Matrix3D mProjection;
        Matrix3D mMVPM;
        MatrixStack mModelView;
        ParticleProgram mParticleProgram;
        ShaderVBO mParticleVBO;
        SpriteProgram mSpriteProgram;
        ShaderVBO mBackVBO;
        GLuint mBackTexture;
        SpriteInstance* mSpriteInstance;
        Uint32 mTickStart;
        Camera mCamera;
        LuaMachine mLuaMachine;

        LoadScreen mLoadScreen;

        /// Lua interfacing
        static int luaTest(LuaState inState);
};

#endif

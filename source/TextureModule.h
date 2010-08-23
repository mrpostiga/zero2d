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
#include "LuaMachine.h"
#include "PlayerControl.h"
#include "GameEngine.h"
#include "OGL.h"
#include "TextPic.h"
#include "HUD.h"
#include "Button.h"
#include "Texture.h"

#include <sstream>
#include <fstream>

#define NUM_JOYSTICKS 32
#define NUM_AXES 4
#define NUM_JOYSTICK_BUTTONS 12
#define NUM_HAT_POSITIONS 13


class TextureModule : public Module
{
    public:
        TextureModule();
        virtual ~TextureModule();

        struct Inputs
        {
            State::Event event;
            Control* player;
        };

        struct JoystickInputs
        {
            Control* player;
            State::Event axis[NUM_AXES];
            State::Event button[NUM_JOYSTICK_BUTTONS];
            State::Event hat[NUM_HAT_POSITIONS];
        };

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
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onJoyAxis(Uint8 inWhich, Uint8 inAxis, Sint16 inValue);
        virtual void onJoyButtonDown(Uint8 inWhich, Uint8 inButton);
        virtual void onJoyButtonUp(Uint8 inWhich, Uint8 inButton);
        virtual void onJoyHat(Uint8 inWhich, Uint8 inHat, Uint8 inValue);

    private:
        void setupInputs();
        State::Event getEndEvent(State::Event inEvent);
        State::Event getEvent(std::string inEvent);

        int animation[8];
        int mCounter;
        int mCurrentIndex;

        Inputs mKeyboardInputs[SDLK_LAST];
        JoystickInputs mJoystickInputs[NUM_JOYSTICKS];

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
        Texture mBackTexture;
        SpriteInstance* mSpriteInstance;
        Uint32 mTickStart;
        Camera mCamera;
        LuaMachine mLuaMachine;
        PlayerControl* mPlayerControl;
        TextPic mTextPic;
        HUD mHUD;

        /// Lua interfacing
        static int luaTest(LuaState inState);
};

#endif

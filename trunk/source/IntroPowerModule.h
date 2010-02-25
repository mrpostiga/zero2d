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

#ifndef _INTROPOWERMODULE_H_
#define _INTROPOWERMODULE_H_

#include <SDL_opengl.h>

#include "Module.h"

class IntroPowerModule : public Module
{
    public:
        virtual bool onLoad();
        virtual void onOpen();
        virtual void onPulse();
        virtual void onRender();
        virtual void onClose();
        virtual void onUnload();

    protected:
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onRButtonDown(int inX, int inY);
        virtual void onJoyButtonDown(Uint8 inWhich, Uint8 inButton);

    private:
        int _phase;
        int _freezeFrames;
        GLuint _logoID;
        GLfloat _ambient[4];
        GLfloat _pos[4];
};

#endif

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

#ifndef SPRITEINSTANCE_H
#define SPRITEINSTANCE_H

#include "Sprite.h"

class SpriteInstance
{
    public:
        SpriteInstance(Sprite* inSprite);
        ~SpriteInstance();

        void update();
        void display();
        void changeState(size_t inNewState);

        void faceRight(bool inRight);
        bool getFaceRight() { return mFacingRight; }
        inline void flip() { mFacingRight = !mFacingRight; }
        void onEvent(State::Event inEvent);

    protected:
    private:
        Sprite* mSprite;
        bool mFacingRight;
        size_t mCurrentFrame;
        size_t mCurrentDuration;
        State* mCurrentState;
};

#endif

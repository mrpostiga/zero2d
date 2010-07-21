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

#include "SpriteInstance.h"

SpriteInstance::SpriteInstance(Sprite* inSprite) : mSprite(inSprite),
    mFacingRight(false), mCurrentFrame(0), mCurrentDuration(0), mCurrentState(0)
{
}

SpriteInstance::~SpriteInstance()
{
}

void SpriteInstance::update()
{
    ++mCurrentDuration;
    State* s = mSprite->getState(mCurrentState);
    if (mCurrentDuration > s->getFrame(mCurrentFrame).duration)
    {
        mCurrentDuration = 0;
        ++mCurrentFrame;
        if (mCurrentFrame >= s->getNumFrames()) mCurrentFrame = 0;
    }
}

void SpriteInstance::display()
{
    State* s = mSprite->getState(mCurrentState);
    mSprite->draw(s->getFrame(mCurrentFrame).frameIndex, mFacingRight);
}

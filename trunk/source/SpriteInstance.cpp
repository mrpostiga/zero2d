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
    mFacingRight(false), mCurrentFrame(0), mCurrentDuration(0), mCurrentState(mSprite->getState(Sprite::STAND))
{
}

SpriteInstance::~SpriteInstance()
{
}

void SpriteInstance::update()
{
    ++mCurrentDuration;
    if (mCurrentDuration > mCurrentState->getFrame(mCurrentFrame).duration)
    {
        mCurrentDuration = 0;
        ++mCurrentFrame;
        if (mCurrentFrame >= mCurrentState->getNumFrames()) mCurrentFrame = 0;
    }
}

void SpriteInstance::display()
{
    mSprite->draw(mCurrentState->getFrame(mCurrentFrame).frameIndex, mFacingRight);
}

void SpriteInstance::changeState(size_t inNewState)
{
    mCurrentFrame = 0;
    mCurrentDuration = 0;
    mCurrentState = mSprite->getState(inNewState);
}

void SpriteInstance::onEvent(State::Event inEvent)
{
    State* newState = mCurrentState->onEvent(inEvent);

    if (newState != NULL)
    {
        mCurrentFrame = 0;
        mCurrentDuration = 0;
        mCurrentState = newState;
    }
}

void SpriteInstance::faceRight(bool inRight)
{
    mFacingRight = inRight;
}

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

#include "GameController.h"

using namespace std;

int GameController::mCount = 0;
GameController* GameController::mControllers[MAX_CONTROLLERS];

void GameController::loadAll()
{
    mCount = SDL_NumJoysticks();
    for (int i = 0; i < mCount; ++i)
    {
        mControllers[i] = new GameController(i);
    }
}

void GameController::unloadAll()
{
    for (int i = 0; i < mCount; ++i)
    {
        delete mControllers[i];
    }
}

GameController::GameController(int inIndex) : mBinding(NULL), mName("none"),
    mIndex(inIndex), mAxes(0), mButtons(0), mBalls(0)
{
    if (SDL_NumJoysticks() > mIndex) mBinding = SDL_JoystickOpen(inIndex);

    if (mBinding)
    {
        mName = SDL_JoystickName(mIndex);
        mAxes = SDL_JoystickNumAxes(mBinding);
        mButtons = SDL_JoystickNumButtons(mBinding);
        mBalls = SDL_JoystickNumBalls(mBinding);
    }

    for (int i = 0; i < MAX_BUTTONS; ++i)
    {
        mButtonPresses[i] = false;
    }
}

GameController::~GameController()
{
    if (SDL_JoystickOpened(mIndex)) SDL_JoystickClose(mBinding);
}

void GameController::moveAxis(Uint8 inAxis, Sint16 inValue)
{
    mAxisPositions[inAxis] = inValue;
}

void GameController::moveHat(Uint8 inHat, Uint8 inPosition)
{
    mHatPositions[inHat] = inPosition;
}

void GameController::buttonDown(Uint8 inButton)
{
    mButtonPresses[inButton] = true;
}

void GameController::buttonUp(Uint8 inButton)
{
    mButtonPresses[inButton] = false;
}

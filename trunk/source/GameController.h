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

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <SDL.h>
typedef SDL_Joystick* Joystick;

#include <string>

#define MAX_CONTROLLERS 8
#define MAX_AXES 8
#define MAX_HATS 4
#define MAX_BUTTONS 32

class GameController
{
    public:
        ~GameController();

        static void loadAll();
        static void unloadAll();
        static inline int count() { return mCount; }
        static inline GameController* get(Uint8 inIndex)
        {
            return mControllers[inIndex];
        }

        inline bool isActive() { return mBinding != NULL; }
        inline const std::string& getName() const { return mName; }
        inline int getAxes() { return mAxes; }
        inline int getButtons() { return mButtons; }
        inline int getBalls() { return mBalls; }

    private:
        GameController(int inIndex);

        static int mCount;
        static GameController* mControllers[MAX_CONTROLLERS];

        Joystick mBinding;
        std::string mName;
        int mIndex;
        int mAxes;
        int mButtons;
        int mBalls;
};

#endif

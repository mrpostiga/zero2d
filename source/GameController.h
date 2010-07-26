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

#ifndef _GAMECONTROLLER_H_
#define _GAMECONTROLLER_H_

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
        static int count();
        static GameController* get(Uint8 inIndex);

        bool isActive();
        const std::string& getName();
        int getAxes();
        int getButtons();
        int getBalls();

        Sint16 getAxis(Uint8 inAxis);

        /// event handlers
        void moveAxis(Uint8 inAxis, Sint16 inValue);
        void moveHat(Uint8 inHat, Uint8 inPosition);
        void buttonDown(Uint8 inButton);
        void buttonUp(Uint8 inButton);

    private:
        GameController(int inIndex);

        static int _count;
        static GameController* _controllers[MAX_CONTROLLERS];

        Joystick _binding;
        std::string _name;
        int _index;
        int _axes;
        int _buttons;
        int _balls;

        Sint16 _axisPositions[MAX_AXES];
        Uint8 _hatPositions[MAX_HATS];
        bool _buttonPresses[MAX_BUTTONS];
};

inline int GameController::count()
{
    return _count;
}

inline GameController* GameController::get(Uint8 inIndex)
{
    return _controllers[inIndex];
}

inline bool GameController::isActive()
{
    return _binding != NULL;
}

inline const std::string& GameController::getName()
{
    return _name;
}

inline int GameController::getAxes()
{
    return _axes;
}

inline int GameController::getButtons()
{
    return _buttons;
}

inline int GameController::getBalls()
{
    return _balls;
}

inline Sint16 GameController::getAxis(Uint8 inAxis)
{
    return _axisPositions[inAxis];
}

#endif

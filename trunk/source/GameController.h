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
using namespace std;

class GameController
{
    public:
        GameController(int inIndex);
        ~GameController();

        const string& getName();
        int getAxes();
        int getButtons();
        int getBalls();

    private:
        Joystick _binding;
        string _name;
        int _index;
        int _axes;
        int _buttons;
        int _balls;
};

inline const string& GameController::getName()
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

#endif

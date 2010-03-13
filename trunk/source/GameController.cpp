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

GameController::GameController(int inIndex) : _binding(NULL), _name("none"),
    _index(inIndex), _axes(0), _buttons(0), _balls(0)
{
    if (SDL_NumJoysticks() > _index) _binding = SDL_JoystickOpen(inIndex);

    if (_binding)
    {
        _name = SDL_JoystickName(_index);
        _axes = SDL_JoystickNumAxes(_binding);
        _buttons = SDL_JoystickNumButtons(_binding);
        _balls = SDL_JoystickNumBalls(_binding);
    }
}

GameController::~GameController()
{
    if (SDL_JoystickOpened(_index)) SDL_JoystickClose(_binding);
}

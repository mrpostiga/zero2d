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

#include "CalibrationModule.h"
#include "DisplayEngine.h"

CalibrationModule::CalibrationModule() : Module(),
    _image("data/images/calibration.png")
{
}

bool CalibrationModule::onLoad()
{
    _controller = GameController::get(0);
    return GameController::count() > 0;
}

void CalibrationModule::onOpen()
{
    DisplayEngine::ortho();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CalibrationModule::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(_translate[0], _translate[1], _translate[2]);
    {
        _image.display();
    }
    glPopMatrix();
}

void CalibrationModule::onPulse()
{
    _translate[0] = P2O(_controller->getAxis(0) / AXIS_FACTOR);
    _translate[1] = P2O(_controller->getAxis(1) / -AXIS_FACTOR);
}

void CalibrationModule::onClose()
{
    glDisable(GL_BLEND);
}

void CalibrationModule::onUnload()
{
}

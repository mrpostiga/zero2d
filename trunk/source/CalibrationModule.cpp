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
    return true;
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
    _image.display();
}

void CalibrationModule::onPulse()
{
}

void CalibrationModule::onClose()
{
    glDisable(GL_BLEND);
}

void CalibrationModule::onUnload()
{
}

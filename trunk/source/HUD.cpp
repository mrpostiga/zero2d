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

#include "HUD.h"
#include "DisplayEngine.h"

#include <iostream>
using namespace std;

HUD::HUD(SpriteProgram& inProgram)
{
    mProgram = &inProgram;
    mProjection.orthographic(HUD_RANGE, DisplayEngine::getAspectRatio());
}

HUD::~HUD()
{
}

void HUD::add(Widget* inWidget)
{
    mWidgets.push_back(inWidget);
}

void HUD::remove(Widget* inWidget)
{
    mWidgets.remove(inWidget);
}

void HUD::display()
{
    for (list<Widget*>::iterator i = mWidgets.begin();
        i != mWidgets.end(); ++i)
    {
        Widget* w = *i;
        if (w->isVisible())
        {
            Matrix3D MVPM(mProjection);
            MVPM.multiply(w->matrix());
            mProgram->setMatrix(MVPM);
            w->display();
        }
    }
}

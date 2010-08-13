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

#include "Widget.h"
#include "HUD.h"
#include "DisplayEngine.h"

Widget::Widget() : mCurrentState(OUTSIDE), mLastState(OUTSIDE), mEnabled(true),
    mVisible(true), mCanFocus(false), mListener(NULL)
{
}

Widget::~Widget()
{
}

void Widget::onMouseStateChange()
{
}

void Widget::changeMouseState(MouseState inState)
{
    if (mCurrentState == inState) return;

    mLastState = mCurrentState;
    mCurrentState = inState;

    if (mListener)
    {
        if (mCurrentState == HOVER && mLastState == PRESS)
            mListener->onWidgetMouseClick();
        else if (mCurrentState == HOVER)
            mListener->onWidgetMouseHover();
    }

    onMouseStateChange();
}

void Widget::registerMouseEventListener(MouseEventListener* inListener)
{
    mListener = inListener;
}

void Widget::convertObjectToPixel()
{
    Pixel displaySize(DisplayEngine::getDisplaySize());
    Pixel center(displaySize[0] / 2, displaySize[1] / 2);

    float portion;

    portion = mObjectUL[0] / HUD_RANGE;
    mPixelUL[0] = center[0] + int(portion * float(center[0]));
    portion = mObjectUL[1] / HUD_RANGE;
    mPixelUL[1] = center[1] - int(portion * float(center[1]));

    portion = mObjectLR[0] / HUD_RANGE;
    mPixelLR[0] = center[0] + int(portion * float(center[0]));
    portion = mObjectLR[1] / HUD_RANGE;
    mPixelLR[1] = center[1] - int(portion * float(center[1]));
}

void Widget::convertPixelToObject()
{
    // to be implemented
}

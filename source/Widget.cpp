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

Widget::Widget() : mCurrentState(OUT), mLastState(OUT), mEnabled(true),
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

void Widget::registerEventListener(EventListener* inListener)
{
    mListener = inListener;
}

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

#ifndef WIDGET_H
#define WIDGET_H

#include "Vector2D.h"
#include "Matrix3D.h"

class Widget
{
    public:
        enum MouseState { OUTSIDE = 0, HOVER = 1, PRESS = 2 };

        class EventListener
        {
            public:
                virtual void onWidgetMouseHover() {}
                virtual void onWidgetMouseClick() {}
        };

        Widget();
        virtual ~Widget();

        virtual void display() = 0;
        virtual void onMouseStateChange();

        void changeMouseState(MouseState inState);
        void registerEventListener(EventListener* inListener);

        inline bool isEnabled() { return mEnabled; }
        inline bool isDisabled() { return !mEnabled; }
        inline bool isVisible() { return mVisible; }
        inline bool canFocus() { return mCanFocus; }

        inline void enable() { mEnabled = true; }
        inline void enable(bool inEnable) { mEnabled = inEnable; }
        inline void disable() { mEnabled = false; }
        inline void setVisible(bool inVisible) { mVisible = inVisible; }

        inline const Matrix3D& matrix() const { return mMatrix; }

    protected:
        MouseState mCurrentState;
        MouseState mLastState;

        Matrix3D mMatrix;
        Point mPosition;
        Point mSize;
        Point mObjectUL;
        Point mObjectLR;
        Pixel mPixelUL;
        Pixel mPixelLR;

    private:
        bool mEnabled;
        bool mVisible;
        bool mCanFocus;

        EventListener* mListener;
};

#endif

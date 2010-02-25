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

#ifndef _TESTMODULE_H_
#define _TESTMODULE_H_

#include "Module.h"

class TestModule : public Module
{
    public:
        virtual bool onLoad();
        virtual void onOpen();
        virtual void onRender();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:
    private:
};

#endif

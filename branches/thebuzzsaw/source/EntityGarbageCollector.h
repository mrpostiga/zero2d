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

#ifndef ENTITYGARBAGECOLLECTOR_H
#define ENTITYGARBAGECOLLECTOR_H

#include "Entity.h"
#include "Control.h"
#include "Thread.h"

#include <SDL.h>
#include <list>

class EntityGarbageCollector : public Thread
{
    public:
        EntityGarbageCollector();
        virtual ~EntityGarbageCollector();

        void addControl(Control* inControl);

    protected:
        virtual void run();

    private:
        std::list<Control*> mControls;
        SDL_mutex* mControlLock;

};

#endif

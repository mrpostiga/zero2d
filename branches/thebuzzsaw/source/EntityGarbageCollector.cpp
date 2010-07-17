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

#include "EntityGarbageCollector.h"

using namespace std;

EntityGarbageCollector::EntityGarbageCollector()
{
    mControlLock = SDL_CreateMutex();
}

EntityGarbageCollector::~EntityGarbageCollector()
{
    list<Control*>::iterator itControls = mControls.begin();

    for (; itControls != mControls.end(); ++itControls)
    {
        delete *itControls;
    }

    mControls.clear();
    SDL_DestroyMutex(mControlLock);
}


void EntityGarbageCollector::run()
{
    while (!stopRequested())
    {
        SDL_mutexP(mControlLock);
        list<Control*>::iterator itControls = mControls.begin();
        while (itControls != mControls.end())
        {
            if (!(*itControls)->getEntity()->isAlive()
                && (*itControls)->isGameDead()
                && (*itControls)->getEntity()->isRenderDead())
            {
                Control* dead = *itControls;
                itControls = mControls.erase(itControls);
                delete dead;
            }
            else
            {
                ++itControls;
            }
        }

        SDL_mutexV(mControlLock);
        SDL_Delay(100);
    }
}

void EntityGarbageCollector::addControl(Control* inControl)
{
    SDL_mutexP(mControlLock);
    mControls.push_back(inControl);
    SDL_mutexV(mControlLock);
}

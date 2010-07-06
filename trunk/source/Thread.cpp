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

#include "Thread.h"

Thread::Thread() : mRunning(false)
{
}

Thread::~Thread()
{
}

void Thread::start(Uint32 inDelay)
{
    if (mRunning) return;
    mThread = SDL_CreateThread(startThread, this);
    SDL_Delay(inDelay);
}

int Thread::startThread(void* inData)
{
    Thread* t = (Thread*)inData;
    t->mStop = false;
    t->mRunning = true;
    t->run();
    t->mRunning = false;
    return 0;
}

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

#ifndef THREAD_H
#define THREAD_H

#include <SDL.h>

class Thread
{
    public:
        Thread();
        virtual ~Thread();

        void start(Uint32 inDelay = 0);
        void wait();
        void stopAndWait();
        virtual void run() = 0;

        inline bool isRunning() { return mRunning; }
        inline void stop()
        {
            /// Requests a stop in the thread but continues normal execution
            /// without waiting.
            mStop = true;
        }

    protected:
        inline bool stopRequested() { return mStop; }

    private:
        static int startThread(void* inData);

        volatile bool mRunning;
        volatile bool mStop;
        SDL_Thread* mThread;
};

#endif

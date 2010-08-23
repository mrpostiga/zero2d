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

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "PropertyList.h"

#include <cstdlib>
#define UNIX_HOME_FOLDER getenv("HOME")

class GameEngine
{
    public:
        static void initialize(int inArgc, char** inArgv);

        static inline const std::string& getUserFolder()
        {
            return mUserFolder;
        }

        static PropertyList config;

    private:
        static std::string mUserFolder;
};

#endif

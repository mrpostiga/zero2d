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

#ifndef _LUAMACHINE_H_
#define _LUAMACHINE_H_

#include "LogFile.h"

#include <lua.hpp>

#include <string>
using namespace std;

class LuaMachine
{
    public:
        LuaMachine();
        virtual ~LuaMachine();

        void loadFile(const char* inFile);
        void runCommand(const char* inCommand);
        void addFunction(const char* inName, lua_CFunction inFunction);

        bool hasError();
        const string& getError();
        void clearError();

    private:
        void execute();
        void reportErrors();

        static LogFile _logFile;

        lua_State* _luaState;
        int _status;
        string _error;
        string _name;
};

inline void LuaMachine::addFunction(const char* inName,
    lua_CFunction inFunction)
{
    lua_register(_luaState, inName, inFunction);
}

inline const string& LuaMachine::getError()
{
    return _error;
}

inline void LuaMachine::clearError()
{
    _status = 0;
    _error.clear();
}

inline bool LuaMachine::hasError()
{
    return !!_status;
}

#endif

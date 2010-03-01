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

#include "LuaMachine.h"

LuaMachine::LuaMachine() : _luaState(luaL_newstate())
{
    luaL_openlibs(_luaState); // massive security hole
}

LuaMachine::~LuaMachine()
{
    lua_close(_luaState);
}

void LuaMachine::reportErrors()
{
    if (_status != 0)
    {
        _error = "-- ";
        _error += lua_tostring(_luaState, -1);
        lua_pop(_luaState, 1); // remove error message
    }
}

void LuaMachine::execute()
{
    _status = lua_pcall(_luaState, 0, LUA_MULTRET, 0);
    cout.flush();
    reportErrors();
}

void LuaMachine::loadFile(const char* inFile)
{
    _status = luaL_loadfile(_luaState, inFile);
    if (_status == 0) execute();
}

void LuaMachine::runCommand(const char* inCommand)
{
    _status = luaL_loadstring(_luaState, inCommand);
    if (_status == 0) execute();
}

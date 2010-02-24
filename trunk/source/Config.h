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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <string>
#include <sstream>
#include <map>
using namespace std;

#define DEFAULT_VALUE ""

class Config
{
    public:
        static void initialize(int inArgc, char** inArgv);
        static void loadFromFile(const char* inFile);
        static void outputSettings();
        static void outputSettings(ostream& inStream);
        static const char* getRaw(const char* inKey, const char* inDefault);
        static const char* getRaw(const char* inKey);
        static void set(const char* inKey, const char* inValue);
        template<class T> static T get(const char* inKey, T inDefault);

    private:
        static map<string, string> mSettings;
};

template<class T>
T Config::get(const char* inKey, T inDefault)
{
    T outValue;

    stringstream ss;
    ss << inDefault;
    string s(ss.str());

    ss.str(string());
    ss.clear();

    ss << getRaw(inKey, s.c_str());
    if(ss.str().length() > 0) ss >> outValue;
    return outValue;
}

void trim(string& inString);

#endif


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

#include "PropertyList.h"

using namespace std;

void trim(string& inString)
{
    size_t startPos = inString.find_first_not_of(" \t");
    size_t endPos = inString.find_last_not_of(" \t");

    if (startPos == string::npos || endPos == string::npos)
        inString = "";
    else
        inString = inString.substr(startPos, endPos - startPos + 1);
}

PropertyList::PropertyList()
{
}

PropertyList::~PropertyList()
{
}

void PropertyList::set(const string& inKey, const string& inValue)
{
    mProperties[inKey] = inValue;
}

const string& PropertyList::getRaw(const string& inKey, const string& inDefault)
{
    StringMap::iterator i = mProperties.find(inKey);

    if (i == mProperties.end())
    {
        mProperties[inKey] = inDefault;
        return inDefault;
    }

    return i->second;
}

istream& operator>>(istream& inStream, PropertyList& inList)
{
    string s;
    while (getline(inStream, s))
    {
        size_t eq = s.find_first_of('=');
        if (s.length() < 2 || s[0] == '`' || eq == string::npos) continue;

        string key = s.substr(0, eq);
        string value = s.substr(eq + 1);
        trim(key);
        trim(value);

        if (key.length() < 2 || value.length() < 1) continue;

        inList.mProperties[key] = value;
    }

    return inStream;
}

ostream& operator<<(ostream& inStream, const PropertyList& inList)
{
    bool first = true;
    for (StringMap::const_iterator i = inList.mProperties.begin();
        i != inList.mProperties.end(); ++i)
    {
        if (first)
            first = false;
        else
            inStream << '\n';

        inStream << i->first << " = " << i->second;
    }

    return inStream;
}

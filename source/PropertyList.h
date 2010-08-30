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

#ifndef PROPERTYLIST_H
#define PROPERTYLIST_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>

typedef std::map<std::string, std::string> StringMap;

class PropertyList
{
    public:
        PropertyList();
        ~PropertyList();

        void set(const std::string& inKey, const std::string& inValue);
        const std::string& getRaw(const std::string& inKey,
            const std::string& inDefault = std::string());

        template<class T>
        T get(const std::string& inKey, T inDefault)
        {
            T outValue;

            std::stringstream ss;
            ss << inDefault;
            std::string s(ss.str());

            ss.str(std::string());
            ss.clear();

            ss << getRaw(inKey, s.c_str());
            if(ss.str().length() > 0) ss >> outValue;
            return outValue;
        }

        friend std::istream& operator>>(std::istream& inStream,
            PropertyList& inList);
        friend std::ostream& operator<<(std::ostream& inStream,
            const PropertyList& inList);

    private:
        StringMap mProperties;
};

void trim(std::string& inString);

#endif

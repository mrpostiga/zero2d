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

#ifndef _LOGFILE_H_
#define _LOGFILE_H_

#include <iostream>
#include <fstream>
using namespace std;

class LogFile
{
    public:
        LogFile(const char* inTitle);
        virtual ~LogFile();

        operator ofstream&();

        void addLine(const char* inText);

    protected:
        ofstream _stream;

    private:
        string _timestamp;
};

inline LogFile::operator ofstream&()
{
    return _stream;
}

#endif

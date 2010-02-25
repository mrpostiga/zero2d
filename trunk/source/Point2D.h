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

#ifndef _POINT2D_H_
#define _POINT2D_H_

#include <iostream>
using namespace std;

template<class T>
struct Point2D
{
    Point2D<T>();
    Point2D<T>(T inValue);
    Point2D<T>(T inX, T inY);
    T x;
    T y;
};

template<class T>
Point2D<T>::Point2D() : x(0), y(0)
{
}

template<class T>
Point2D<T>::Point2D(T inValue) : x(inValue), y(inValue)
{
}

template<class T>
Point2D<T>::Point2D(T inX, T inY) : x(inX), y(inY)
{
}

template<class T>
istream& operator>>(istream& inStream, Point2D<T>& inPoint)
{
    inStream >> inPoint.x >> inPoint.y;
    return inStream;
}

template<class T>
ostream& operator<<(ostream& inStream, const Point2D<T>& inPoint)
{
    inStream << inPoint.x << ' ' << inPoint.y;
    return inStream;
}

#endif

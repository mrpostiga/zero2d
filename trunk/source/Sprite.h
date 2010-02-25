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

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "Point2D.h"

#include <SDL_opengl.h>

#include <iostream>
#include <map>
#include <string>
using namespace std;

struct FrameDatum
{
    Point2D<int> location;
    Point2D<int> size;
    Point2D<int> base;
    int duration;

    FrameDatum() : duration(0) {}
};

istream& operator>>(istream& inStream, FrameDatum& inFD);

class Sprite
{
    public:
        virtual ~Sprite();

        static Sprite* load(string inPath);
        static void unloadAll();

        void bindTexture();

    private:
        Sprite(const string& inPath);

        static map<string, Sprite*> _sprites;

        string _title;
        int _numFrames;
        GLuint _texture; // entire sprite sheet
        Point2D<int> _sheetSize;
        FrameDatum* _frameData;
};

inline void Sprite::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, _texture);
}

#endif

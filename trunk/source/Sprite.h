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
#include "Vector3D.h"

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
    int sheet;
    int duration;

    FrameDatum() : duration(0) {}
};

istream& operator>>(istream& inStream, FrameDatum& inFD);

struct DrawArgs
{
    int index;
    Point2D<int> location;
    float rotation;
    bool facingRight;
    Vector3D<float> colorMod;

    DrawArgs() : index(0), rotation(0.0f), facingRight(false), colorMod(1) {}
};

struct Sheet
{
    GLuint texture;
    Point2D<int> size;
};

class Sprite
{
    public:
        virtual ~Sprite();

        static Sprite* load(string inPath);
        static void unloadAll();

        void draw(const DrawArgs& inArgs);
        int getDuration(int inIndex);
        int getNumFrames();

    private:
        Sprite(const string& inPath);

        static map<string, Sprite*> _sprites;

        string _title;
        int _numFrames;
        int _numSheets;
        Sheet* _sheets;
        FrameDatum* _frameData;
};

inline int Sprite::getNumFrames()
{
    return _numFrames;
}

inline int Sprite::getDuration(int inIndex)
{
    return _frameData[inIndex].duration;
}

#endif

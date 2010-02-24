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

#ifndef _DISPLAYENGINE_H_
#define _DISPLAYENGINE_H_

#include "Point2D.h"

#include <SDL.h>
#include <SDL_opengl.h>
typedef SDL_Surface* Surface;

#include <iostream>
using namespace std;

#define ENGINE_FPS 60
#define FRAME_LENGTH (1000 / ENGINE_FPS)
#define FIELD_OF_VIEW 30.0
#define NEAR_CP 1.0
#define FAR_CP 10000.0

struct Mask
{
    Uint32 red;
    Uint32 green;
    Uint32 blue;
    Uint32 alpha;
};

class Module;

class DisplayEngine
{
    public:
        static void start(Module* inModule);
        static void initialize();
        static void logOpenGL(ostream& inStream);
        static Surface loadImage(const char* inFile);
        static bool loadTexture(Surface inSurface, GLuint inTexture);
        static bool loadTexture(const char* inFile, GLuint inTexture);

    private:
        static void cleanup();

        static void printErrors();

        static Surface _display;
        static Surface _windowIcon;
        static SDL_Rect** _modes;
        static bool _mipmapping;
        static Mask _mask;
};

#endif
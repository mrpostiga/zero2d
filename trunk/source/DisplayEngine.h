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

#include "Module.h"
#include "Point2D.h"

#include <SDL.h>
#include <SDL_opengl.h>
typedef SDL_Surface* Surface;

#include <iostream>
using namespace std;

// conversion from object space to pixel space
#define O2P(n) static_cast<int>((n) * 4.0f)

// conversion from pixel space to object space
#define P2O(n) (static_cast<float>(n) / 4.0f)

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

class DisplayEngine
{
    public:
        static void start(Module* inModule);
        static void initialize();

        /// OpenGL/SDL functionality wrappers
        static void ortho(double inRange);
        static Surface loadImage(const char* inFile);
        static bool loadTexture(Surface inSurface, GLuint inTexture,
            bool inDelete = true);
        static bool loadTexture(const char* inFile, GLuint inTexture);
        static int processKey(SDLKey inSym, SDLMod inMod);

        static void logOpenGL(ostream& inStream);
        static void logErrors(ostream& inStream);

    private:
        static void cleanup();

        static Surface _display;
        static Surface _windowIcon;
        static Surface _dot;
        static SDL_Rect** _modes;
        static bool _mipmapping;
        static Mask _mask;
};

#endif

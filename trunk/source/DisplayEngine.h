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
#include "Vector3D.h"
#include "Matrix.h"

#include <SDL.h>
#include "OpenGL.h"
typedef SDL_Surface* Surface;

// conversion from object space to pixel space
#define O2P(n) static_cast<int>((n) * 4.0f)

// conversion from pixel space to object space
#define P2O(n) (static_cast<float>(n) / 4.0f)

#define TEXTURE_MODE(n) glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, n)

#define ENGINE_FPS 60
#define FRAME_LENGTH (1000 / ENGINE_FPS)
#define NEAR_CP 1.0
#define FAR_CP 10000.0

struct ColorMask
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

        /// OpenGL/SDL functionality wrappers
        static void ortho();
        static void ortho(double inRange);
        static Surface loadImage(const char* inFile);
        static Pixel loadTexture(Surface inSurface, GLuint inTexture,
            bool inDelete = true);
        static Pixel loadTexture(const char* inFile, GLuint inTexture);
        static int processKey(SDLKey inSym, SDLMod inMod);

        static void logOpenGL(ostream& inStream);
        static void logErrors(ostream& inStream);

        template<class T> static void transform(Vector3D<T>& inVector,
            const Matrix<T>& inMatrix);

    private:
        static void cleanup();

        static Surface _display;
        static Surface _windowIcon;
        static Surface _dot;
        static SDL_Rect** _modes;
        static bool _mipmapping;
        static ColorMask _mask;
};

template<class T>
void DisplayEngine::transform(Vector3D<T>& inVector, const Matrix<T>& inMatrix)
{
    Vector3D<T> result;
    result[0] = inVector[0] * inMatrix[0] + inVector[1] * inMatrix[4]
        + inVector[2] * inMatrix[8] + inVector[3] * inMatrix[12];
    result[1] = inVector[0] * inMatrix[1] + inVector[1] * inMatrix[5]
        + inVector[2] * inMatrix[9] + inVector[3] * inMatrix[13];
    result[2] = inVector[0] * inMatrix[2] + inVector[1] * inMatrix[6]
        + inVector[2] * inMatrix[10] + inVector[3] * inMatrix[14];
    result[3] = inVector[0] * inMatrix[3] + inVector[1] * inMatrix[7]
        + inVector[2] * inMatrix[11] + inVector[3] * inMatrix[15];
    inVector = result;
}

#endif

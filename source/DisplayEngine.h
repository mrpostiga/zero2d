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

#ifndef DISPLAYENGINE_H
#define DISPLAYENGINE_H

#include "Vector2D.h"
#include "LogFile.h"

#include "CoreSDL.h"
#include "OGL.h"

#include <iostream>

#define ENGINE_FPS 60
#define FRAME_LENGTH (1000 / ENGINE_FPS)
#define FIELD_OF_VIEW 30.0
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
        static Surface loadImage(const char* inFile);
        static Pixel loadTexture(Surface inSurface, GLuint inTexture,
            bool inDelete = true);
        static Pixel loadTexture(const char* inFile, GLuint inTexture);
        static void openGLDriverInfo(std::ostream& inStream);

        static bool printErrors(const char* inMessage,
            std::ostream& inStream = mLogFile);

        static inline const std::string& getShaderFolder()
        {
            return mShaderFolder;
        }

        static inline float getAspectRatio() { return mAspectRatio; }
        static inline int getDisplayHeight() { return mDisplay->h; }
        static inline Pixel getDisplaySize() { return mDisplaySize; }
        static inline void render() { SDL_GL_SwapBuffers(); }

    private:
        static void cleanup();

        static Pixel mDisplaySize;
        static Surface mWindowIcon;
        static SDL_Rect** mModes;
        static unsigned int mNextFrame;
        static unsigned int mCurrentFrame;
        static int mMipmapping;
        static ColorMask mMask;
        static std::string mShaderFolder;
        static float mAspectRatio;
        static Surface mDisplay;
        static unsigned int mFPS;

        static LogFile mLogFile;
};

template<class T>
T linearInterpolate(const T& inMin, const T& inMax, double inT)
{
    return (inMin * (1.0 - inT)) + (inMax * inT);
}

#endif

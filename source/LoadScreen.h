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

#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "SpriteProgram.h"
#include "Matrix4x4.h"
#include "Texture.h"

#include <cmath>
#include <string>

class LoadScreen
{
    public:
        LoadScreen();
        virtual ~LoadScreen();

        void update(int inPercent);
        void update(float inPercent);

        void setBackgroundImage(const char* inBackgroundImage);
        void setLoadImage(const char* inLoadImage);
        void setLoadLocation(int inX, int inY);

        void setup();

    private:
        float mScreenRange;
        int mLoadLocationX;
        int mLoadLocationY;
        int mLoadScreenLocationX;
        int mLoadScreenLocationY;
        int mLoadWidth;
        int mLoadHeight;

        Matrix3D mProjection;
        Matrix3D mMVPM;
        Texture mBackTexture;
        Texture mLoadTexture;

        ShaderVBO mLoadingBarVBO;
        SpriteProgram mProgram;
        ShaderVBO mBackVBO;
};

#endif

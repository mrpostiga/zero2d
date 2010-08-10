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

#ifndef TEXTPIC_H
#define TEXTPIC_H

#include "ShaderVBO.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include <SDL_ttf.h>
#include <string>

class TextPic
{
    public:
        TextPic();
        ~TextPic();

        enum Quality { SOLID, SHADED, BLENDED };

        void loadFont(const char* inFile, int inSize);
        void setScale(float inScale);
        void setText(const std::string& inText);
        void display();

        inline const std::string& getText() { return mText; }
        inline void setQuality(Quality inQuality) { mQuality = inQuality; }

    private:
        void render();
        void updateVBO();

        TTF_Font* mFont;
        SDL_Color mColor;
        std::string mText;
        GLuint mTexture;
        float mScale;
        Point mScales;
        Pixel mTextSize;
        Quality mQuality;
        ShaderVBO mVBO;
};

#endif

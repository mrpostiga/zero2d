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

#ifndef SPRITE_H
#define SPRITE_H

#include "Vector2D.h"
#include "ShaderVBO.h"
#include "State.h"

#include <string>
#include <vector>

class Sprite
{
    public:
        Sprite(const char* inTitle);
        ~Sprite();

        struct Frame
        {
            Pixel location;
            Pixel size;
            Pixel base;
            size_t sheet;
        };

        struct Sheet
        {
            GLuint texture;
            Pixel size;
        };



        State* getState(size_t inIndex) { return mStateTree[inIndex]; }

        void draw(size_t inIndex, bool inFacingRight);

    private:
        ShaderVBO mSVBO;
        const std::string mKey;
        std::string mName;

        std::vector<Frame> mFrames;
        std::vector<Sheet> mSheets;
        std::vector<State*> mStateTree;

        void setupStateTree();
};

#endif

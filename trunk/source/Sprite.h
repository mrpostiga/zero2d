#ifndef SPRITE_H
#define SPRITE_H

#include "ShaderVBO.h"

class Sprite
{
    public:
        Sprite(const char* inTitle);
        ~Sprite();

        void draw(unsigned short inIndex, bool inFacingRight);

    private:
        ShaderVBO mSVBO;
};

#endif

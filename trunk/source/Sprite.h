#ifndef SPRITE_H
#define SPRITE_H

#include "ShaderVBO.h"
#include "Point2D.h"

#include <string>
#include <vector>
// yes

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

        struct SubState
        {
            size_t frame;
            unsigned int duration;
        };

        struct State
        {
            size_t size;
            SubState* animation;
        };

        void draw(int inIndex, bool inFacingRight);

    private:
        ShaderVBO mSVBO;
        const std::string mKey;
        std::string mName;

        std::vector<Frame> mFrames;
        std::vector<Sheet> mSheets;
        std::vector<State> mStates;
};

#endif

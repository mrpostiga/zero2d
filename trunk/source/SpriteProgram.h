#ifndef SPRITEPROGRAM_H
#define SPRITEPROGRAM_H

#include "ShaderProgram.h"

class SpriteProgram : public ShaderProgram
{
    public:
        SpriteProgram();
        virtual ~SpriteProgram();

        enum Bindings
        {
            VERTEX = 0,
            TEXTURE = 1
        };

        inline void setZ(float inZ) { glUniform1f(mUniformZ, inZ); }
        inline void setFade(float inFade) { glUniform1f(mUniformFade, inFade); }

    protected:
        virtual void bindUniforms();

    private:
        GLint mUniformTexture;
        GLint mUniformFade;
        GLint mUniformZ;
};

#endif

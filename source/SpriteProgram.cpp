#include "SpriteProgram.h"

SpriteProgram::SpriteProgram() : ShaderProgram(2)
{
    attachShader("sprite.vs");
    attachShader("sprite.fs");

    glBindAttribLocation(handle(), VERTEX, "CornerVertex");
    glBindAttribLocation(handle(), TEXTURE, "TexCoord");
    linkAndBind();
    glUniform1i(mUniformTexture, 0);
}

SpriteProgram::~SpriteProgram()
{
}

void SpriteProgram::bindUniforms()
{
    mUniformTexture = glGetUniformLocation(handle(), "Texture");
    mUniformFade = glGetUniformLocation(handle(), "fade");
    mUniformZ = glGetUniformLocation(handle(), "z");
}

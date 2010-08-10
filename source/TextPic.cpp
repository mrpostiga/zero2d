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

#include "TextPic.h"
#include "DisplayEngine.h"
#include "SpriteProgram.h"

#include <iostream>
using namespace std;

TextPic::TextPic() : mFont(NULL), mScale(1.0f), mQuality(BLENDED)
{
    mColor.r = 255;
    mColor.g = 255;
    mColor.b = 255;
    mColor.unused = 255;

    glGenTextures(1, &mTexture);

    GLfloat textureCoordinates[12] = {0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    mVBO.loadVAA(SpriteProgram::VERTEX, 3, 4, textureCoordinates);
    mVBO.loadVAA(SpriteProgram::TEXTURE, 2, 4, textureCoordinates);
}

TextPic::~TextPic()
{
    if (mFont) TTF_CloseFont(mFont);
    glDeleteTextures(1, &mTexture);
}

void TextPic::loadFont(const char* inFile, int inSize)
{
    if (mFont) TTF_CloseFont(mFont);
    mFont = TTF_OpenFont(inFile, inSize);
    if (!mFont)
    {
        cerr << "failed to load font: " << inFile << endl;
    }
}

void TextPic::setScale(float inScale)
{
    if (inScale <= 0.0f) inScale = 1.0f;
    mScale = inScale;
    render();
}

void TextPic::setText(const string& inText)
{
    mText = inText;
    render();
}

void TextPic::display()
{
    glBindTexture(GL_TEXTURE_2D, mTexture);
    mVBO.displayLinear(GL_QUADS, 0, 4);
}

void TextPic::render()
{
    Surface textSurface = NULL;
    const char* text = mText.length() ? mText.c_str() : " ";

    switch (mQuality)
    {
        case BLENDED:
        {
            textSurface = TTF_RenderText_Blended(mFont, text, mColor);
            break;
        }

        case SHADED:
        {
            //textSurface = TTF_RenderText_Shaded(mFont, text, mColor);
            break;
        }

        case SOLID:
        default:
        {
            textSurface = TTF_RenderText_Solid(mFont, text, mColor);
        }
    }

    if (!textSurface)
    {
        cerr << "failed to render text" << endl;
        DisplayEngine::loadTexture((Surface)NULL, mTexture);
        return;
    }

    mTextSize[0] = textSurface->w;
    mTextSize[1] = textSurface->h;

    int widthPower = int(log(mTextSize[0]) / log(2.0f)) + 1;
    int heightPower = int(log(mTextSize[1]) / log(2.0f)) + 1;

    widthPower = (int)pow(2.0f, widthPower);
    heightPower = (int)pow(2.0f, heightPower);

    Surface s = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, widthPower,
        heightPower, textSurface->format->BitsPerPixel,
        textSurface->format->Rmask, textSurface->format->Gmask,
        textSurface->format->Bmask, textSurface->format->Amask);

    SDL_SetAlpha(textSurface, 0, 0xff);
    SDL_SetAlpha(s, SDL_SRCALPHA, 0x00);

    if (SDL_BlitSurface(textSurface, NULL, s, NULL))
    {
        cerr << "TextPic::render() -- blitting error" << endl;
    }

    SDL_SetAlpha(s, SDL_SRCALPHA, textSurface->format->alpha);

    SDL_FreeSurface(textSurface);

    float ratio = float(mTextSize[0]) / float(mTextSize[1]);
    Point ratios;
    ratios[0] = float(mTextSize[0]) / (s == NULL ? 1.0f : s->w);
    ratios[1] = float(mTextSize[1]) / (s == NULL ? 1.0f : s->h);
    mScales[0] = ratio / ratios[0];
    mScales[1] = 1.0f / ratios[1];

    DisplayEngine::loadTexture(s, mTexture, true);
    updateVBO();
}

void TextPic::updateVBO()
{
    GLfloat vertices[12];
    for (size_t i = 0; i < 12; ++i) vertices[i] = 0.0f;

    vertices[1] = -mScale * mScales[1];
    vertices[3] = mScale * mScales[0];
    vertices[4] = -mScale * mScales[1];
    vertices[6] = mScale * mScales[0];

    mVBO.reloadVAA(SpriteProgram::VERTEX, 4, vertices);
}

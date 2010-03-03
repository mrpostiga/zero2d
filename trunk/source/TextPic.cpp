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

TextPic::TextPic() : _font(NULL), _colorMod(1.0f), _texture(0),
    _quality(TTF_BLENDED)
{
    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.unused = 255;

    glGenTextures(1, &_texture);
}

TextPic::~TextPic()
{
    if (_font != NULL)
    {
        TTF_CloseFont(_font);
        _font = NULL;
    }

    glDeleteTextures(1, &_texture);
}

void TextPic::loadFont(const char* inFile, int inSize)
{
    _font = TTF_OpenFont(inFile, inSize);

    if (_font == NULL)
    {
        cerr << "-- error on TTF_OpenFont -- " << TTF_GetError() << endl;
        exit(1);
    }
}

void TextPic::setText(const char* inText)
{
    if (_text == inText) return;
    _text = inText;
    render();
}

void TextPic::setText(const string& inText)
{
    if (_text == inText) return;
    _text = inText;
    render();
}

void TextPic::render()
{
    Surface textSurface = NULL;

    switch (_quality)
    {
        case TTF_BLENDED:
        {
            textSurface = TTF_RenderText_Blended(_font, _text.c_str(), _color);
            break;
        }

        case TTF_SHADED:
        {
            //textSurface = TTF_RenderText_Shaded(_font, _text.c_str(), _color);
            break;
        }

        case TTF_SOLID:
        default:
        {
            textSurface = TTF_RenderText_Solid(_font, _text.c_str(), _color);
        }
    }

    if (textSurface == NULL)
    {
        DisplayEngine::loadTexture((Surface)NULL, _texture);
        return;
    }

    Point2D<int> size(textSurface->w, textSurface->h);

    int widthPower = int(log(size.x) / log(2.0f)) + 1;
    int heightPower = int(log(size.y) / log(2.0f)) + 1;

    widthPower = (int)pow(2.0f, widthPower);
    heightPower = (int)pow(2.0f, heightPower);

    Surface s = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, widthPower,
        heightPower, textSurface->format->BitsPerPixel,
        textSurface->format->Rmask, textSurface->format->Gmask,
        textSurface->format->Bmask, textSurface->format->Amask);

    SDL_SetAlpha(textSurface, 0, 0xff);
    SDL_SetAlpha(s, SDL_SRCALPHA, 0x00);

    if (SDL_BlitSurface(textSurface, NULL, s, NULL) != 0)
    {
        cerr << "blitting error" << endl;
    }

    SDL_SetAlpha(s, SDL_SRCALPHA, textSurface->format->alpha);

    SDL_FreeSurface(textSurface);

    float ratio = float(size.x) / float(size.y);
    Point2D<float> ratios;
    ratios.x = float(size.x) / (s == NULL ? 1.0f : s->w);
    ratios.y = float(size.y) / (s == NULL ? 1.0f : s->h);
    _scales.x = ratio / ratios.x;
    _scales.y = 1.0f / ratios.y;

    DisplayEngine::loadTexture(s, _texture);
}

void TextPic::draw(float inX, float inY, float inScale)
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glBegin(GL_QUADS);
    {
        glColor4fv(_colorMod.array());
        glTexCoord2i(0, 1);
        glVertex2f(inX, inY - (inScale * _scales.y));
        glTexCoord2i(1, 1);
        glVertex2f(inX + (inScale * _scales.x), inY - (inScale * _scales.y));
        glTexCoord2i(1, 0);
        glVertex2f(inX + (inScale * _scales.x), inY);
        glTexCoord2i(0, 0);
        glVertex2f(inX, inY);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

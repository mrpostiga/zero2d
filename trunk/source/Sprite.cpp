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

#include "Sprite.h"
#include "DisplayEngine.h"

#include <cstdlib>
#include <fstream>

istream& operator>>(istream& inStream, FrameDatum& inFD)
{
    inStream >> inFD.location >> inFD.size >> inFD.base >> inFD.duration;
    return inStream;
}

map<string, Sprite*> Sprite::_sprites;

Sprite::Sprite(const string& inPath) : _numFrames(0)
{
    string filename(inPath);
    filename += "/static.02d";

    ifstream staticData;
    staticData.open(filename.c_str());

    if (staticData.fail())
    {
        cerr << "failed to open file: " << filename << endl;
        exit(1);
    }

    getline(staticData, _title) >> _numFrames;
    _frameData = new FrameDatum[_numFrames];

    for (int i = 0; i < _numFrames; ++i)
    {
        int index = 0;
        staticData >> index >> _frameData[i];
        //cerr << "loading frame " << index << endl;
    }

    staticData.close();

    filename = inPath;
    filename += "/sheet.png";
    glGenTextures(1, &_texture);
    Surface s = DisplayEngine::loadImage(filename.c_str());
    if (s == NULL)
    {
        cerr << "failed to load sprite sheet: " << filename << endl;
        exit(1);
    }

    _sheetSize.x = s->w;
    _sheetSize.y = s->h;
    DisplayEngine::loadTexture(s, _texture);
}

Sprite::~Sprite()
{
    if (_numFrames > 0)
    {
        glDeleteTextures(1, &_texture);
        delete [] _frameData;
    }
}

Sprite* Sprite::load(string inPath)
{
    map<string, Sprite*>::iterator i = _sprites.find(inPath);

    if (i == _sprites.end())
    {
        Sprite* s = new Sprite(inPath);
        _sprites[inPath] = s;
        return s;
    }

    return i->second;
}

void Sprite::unloadAll()
{
    for (map<string, Sprite*>::iterator i = _sprites.begin();
        i != _sprites.end(); ++i)
    {
        delete i->second;
    }

    _sprites.clear();
}

void Sprite::draw(const DrawArgs& inArgs)
{
    const FrameDatum& fd = _frameData[inArgs.index];

    Point2D<float> QuadUL;
    Point2D<float> QuadLR;

    QuadUL.x = P2O(-fd.base.x);
    QuadUL.y = P2O(fd.base.y);
    QuadLR.x = P2O(-fd.base.x + fd.size.x);
    QuadLR.y = P2O(fd.base.y - fd.size.y);

    Point2D<float> TextureUL;
    Point2D<float> TextureLR;

    TextureUL.x = float(fd.location.x) / float(_sheetSize.x);
    TextureLR.x = TextureUL.x + (float(fd.size.x) / float(_sheetSize.x));

    TextureLR.y = float(_sheetSize.y - fd.location.y) / float(_sheetSize.y);
    TextureUL.y = TextureLR.y - (float(fd.size.y) / float(_sheetSize.y));

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, _texture);
    glBegin(GL_QUADS);
    {
        glColor4fv(inArgs.colorMod.array());
        glNormal3f(0.0f, 0.0f, 1.0f);

        glTexCoord2f(TextureUL.x, TextureUL.y);
        glVertex2f(QuadUL.x, QuadUL.y);

        glTexCoord2f(TextureLR.x, TextureUL.y);
        glVertex2f(QuadLR.x, QuadUL.y);

        glTexCoord2f(TextureLR.x, TextureLR.y);
        glVertex2f(QuadLR.x, QuadLR.y);

        glTexCoord2f(TextureUL.x, TextureLR.y);
        glVertex2f(QuadUL.x, QuadLR.y);
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

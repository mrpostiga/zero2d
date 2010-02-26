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
#include <sstream>

istream& operator>>(istream& inStream, FrameDatum& inFD)
{
    inStream >> inFD.sheet >> inFD.location >> inFD.size >> inFD.base
        >> inFD.duration;
    return inStream;
}

map<string, Sprite*> Sprite::_sprites;

Sprite::Sprite(const string& inPath) : _numFrames(0), _numSheets(0)
{
    stringstream filename;
    filename << inPath << "/static.02d";

    ifstream staticData;
    staticData.open(filename.str().c_str());

    if (staticData.fail())
    {
        cerr << "failed to open file: " << filename << endl;
        exit(1);
    }

    getline(staticData, _title) >> _numSheets >> _numFrames;
    _sheets = new Sheet[_numSheets];
    _frameData = new FrameDatum[_numFrames];

    for (int i = 0; i < _numFrames; ++i)
    {
        int index = 0;
        staticData >> index >> _frameData[i];
        //cerr << "loading frame " << index << endl;
    }

    staticData.close();

    for (int i = 0; i < _numSheets; ++i)
    {
        filename.str(string());
        filename << inPath << "/sheet-" << i << ".png";
        glGenTextures(1, &_sheets[i].texture);
        Surface s = DisplayEngine::loadImage(filename.str().c_str());

        if (s == NULL)
        {
            cerr << "failed to load sprite sheet: " << filename << endl;
            exit(1);
        }

        _sheets[i].size.x = s->w;
        _sheets[i].size.y = s->h;

        DisplayEngine::loadTexture(s, _sheets[i].texture);
    }
}

Sprite::~Sprite()
{
    if (_numFrames > 0)
    {
        for (int i = 0; i < _numSheets; ++i)
            glDeleteTextures(1, &_sheets[i].texture);

        delete [] _frameData;
        delete [] _sheets;
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

    TextureUL.x = float(fd.location.x) / float(_sheets[fd.sheet].size.x);
    float width = float(fd.size.x) / float(_sheets[fd.sheet].size.x);
    if (inArgs.facingRight) TextureUL.x += width;
    TextureLR.x = TextureUL.x + width;

    TextureLR.y = float(_sheets[fd.sheet].size.y - fd.location.y)
        / float(_sheets[fd.sheet].size.y);
    TextureUL.y = TextureLR.y - (float(fd.size.y)
        / float(_sheets[fd.sheet].size.y));

    glPushMatrix();
    glRotatef(inArgs.rotation, 0.0f, 0.0f, -1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _sheets[fd.sheet].texture);
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
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

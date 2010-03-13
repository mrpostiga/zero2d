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

map<string, Sprite*> Sprite::_sprites;

Sprite::Sprite(const string& inPath) : _numFrames(0), _numSheets(0),
    _numStates(0)
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

    char key;
    FrameDatum currentData;
    getline(staticData, _title) >> _numSheets >> _numFrames >> key;
    _sheets = new Sheet[_numSheets];
    _frameData = new FrameDatum[_numFrames];

    while (key != 'x')
    {
        switch (key)
        {
            case 'b':
            {
                staticData >> currentData.base;
                break;
            }

            case 's':
            {
                staticData >> currentData.size;
                break;
            }

            case 'f':
            {
                int index;
                staticData >> index;
                staticData >> _frameData[index].sheet
                    >> _frameData[index].location;
                _frameData[index].base = currentData.base;
                _frameData[index].size = currentData.size;
                break;
            }

            default:
            {
                cerr << "unrecognized data key: " << key << endl;
                key = 'x';
            }
        }

        if (key != 'x') staticData >> key;
    }

    staticData >> _numStates;
    _stateData = new StateDatum[_numStates];
    int currentDuration = 0;
    int currentState = 0;

    staticData >> key;

    while (key != 'x')
    {
        switch (key)
        {
            case 's':
            {
                staticData >> currentState;
                break;
            }

            case 'd':
            {
                staticData >> currentDuration;
                break;
            }

            case 'f':
            {
                FramePair fp;
                fp.duration = currentDuration;
                staticData >> fp.index;
                _stateData[currentState].frames.push_back(fp);
                break;
            }


            default:
            {
                cerr << "unrecognized data key: " << key << endl;
                key = 'x';
            }
        }

        if (key != 'x') staticData >> key;
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
        delete [] _stateData;
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

    static Point QuadUL;
    static Point QuadLR;

    if (inArgs.facingRight)
    {
        QuadUL.x = P2O(fd.base.x - fd.size.x);
        QuadUL.y = P2O(fd.base.y);
        QuadLR.x = P2O(fd.base.x);
        QuadLR.y = P2O(fd.base.y - fd.size.y);
    }
    else
    {
        QuadUL.x = P2O(-fd.base.x);
        QuadUL.y = P2O(fd.base.y);
        QuadLR.x = P2O(fd.size.x - fd.base.x);
        QuadLR.y = P2O(fd.base.y - fd.size.y);
    }

    static Point TextureUL;
    static Point TextureLR;

    TextureUL.x = float(fd.location.x) / float(_sheets[fd.sheet].size.x);
    float width = float(fd.size.x) / float(_sheets[fd.sheet].size.x);
    if (inArgs.facingRight) TextureUL.x += width;
    TextureLR.x = TextureUL.x + width;

    TextureUL.y = float(fd.location.y)
        / float(_sheets[fd.sheet].size.y);
    TextureLR.y = TextureUL.y + (float(fd.size.y)
        / float(_sheets[fd.sheet].size.y));

    glPushMatrix();
    glTranslatef(inArgs.location.x, inArgs.location.y, 0.0f);
    glRotatef(inArgs.rotation, 0.0f, 0.0f, -1.0f);
    glEnable(GL_TEXTURE_2D);
    TEXTURE_MODE(inArgs.textureParam);
    glBindTexture(GL_TEXTURE_2D, _sheets[fd.sheet].texture);
    glBegin(GL_QUADS);
    {
        glColor4fv(inArgs.colorMod.array());
        glNormal3f(0.0f, 0.0f, 1.0f);

        glTexCoord2f(TextureUL.x, TextureUL.y); // 0.0
        glVertex2f(QuadUL.x, QuadUL.y);

        glTexCoord2f(TextureLR.x, TextureUL.y); // 0.0
        glVertex2f(QuadLR.x, QuadUL.y);

        glTexCoord2f(TextureLR.x, TextureLR.y); // 0.25
        glVertex2f(QuadLR.x, QuadLR.y);

        glTexCoord2f(TextureUL.x, TextureLR.y); // 0.25
        glVertex2f(QuadUL.x, QuadLR.y);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

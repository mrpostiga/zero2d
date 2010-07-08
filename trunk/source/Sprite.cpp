#include "Sprite.h"
#include "Point2D.h"
#include "DisplayEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Sprite::Sprite(const char* inKey) : mKey(inKey)
{
    string path("data/sprites/");
    path += mKey;
    path += '/';
    string spriteFile(path);
    spriteFile += "static.02d";

    ifstream fin(spriteFile.c_str());
    if (fin.fail())
    {
        cerr << "failed to load sprite: " << spriteFile << endl;
        return;
    }

    char c;
    Frame currentFrame;

    size_t numSheets;
    size_t numFrames;
    getline(fin, mName) >> numSheets >> numFrames >> c;
    mSheets.resize(numSheets);
    mFrames.resize(numFrames);
    while (c != 'x')
    {
        switch (c)
        {
            case 'b':
            {
                fin >> currentFrame.base;
                break;
            }

            case 's':
            {
                fin >> currentFrame.size;
                break;
            }

            case 'f':
            {
                size_t index;
                fin >> index;
                Frame& f = mFrames[index];
                fin >> f.sheet >> f.location;
                f.base = currentFrame.base;
                f.size = currentFrame.size;
                break;
            }

            default:
            {
                cerr << "unrecognized guide: " << c << endl;
                c = 'x';
            }
        }

        if (c != 'x') fin >> c;
    }

    size_t numStates;
    fin >> numStates >> c;
    mStates.resize(numStates);
    size_t currentState = 0;
    size_t currentSize = 0;
    size_t currentDuration = 0;
    size_t currentAnimation = 0;

    while (c != 'x')
    {
        switch (c)
        {
            case 's':
            {
                fin >> currentState >> currentSize;
                mStates[currentState].animation = new SubState[currentSize];
                currentAnimation = 0;
                break;
            }

            case 'd':
            {
                fin >> currentDuration;
                break;
            }

            case 'f':
            {
                SubState ss;
                ss.duration = currentDuration;
                fin >> ss.frame;
                mStates[currentState].animation[currentAnimation] = ss;
                ++currentAnimation;
                break;
            }

            default:
            {
                cerr << "unrecognized guide: " << c << endl;
                c = 'x';
            }
        }

        if (c != 'x') fin >> c;
    }

    fin.close();

    for (size_t i = 0; i < mSheets.size(); ++i)
    {
        stringstream ss;
        ss << path << "sheet-" << i << ".png";
        glGenTextures(1, &mSheets[i].texture);
        Surface s = DisplayEngine::loadImage(ss.str().c_str());

        if (s == NULL)
        {
            cerr << "failed to load sprite sheet: " << ss.str() << endl;
            exit(1);
        }

        mSheets[i].size.x = s->w;
        mSheets[i].size.y = s->h;

        DisplayEngine::loadTexture(s, mSheets[i].texture);
    }
}

Sprite::~Sprite()
{
    for (size_t i = 0; i < mSheets.size(); ++i)
    {
        glDeleteTextures(1, &mSheets[i].texture);
    }

    for (size_t i = 0; i < mStates.size(); ++i)
    {
        delete [] mStates[i].animation;
    }
}

void Sprite::draw(int inIndex, bool inFacingRight)
{
    GLint target = inIndex * 8;
    if (inFacingRight) target += 4;
    mSVBO.displayLinear(GL_QUADS, target, 4);
}

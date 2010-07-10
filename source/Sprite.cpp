#include "Sprite.h"
#include "Point2D.h"
#include "DisplayEngine.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

ShaderProgram* Sprite::mShaderProgram = NULL;

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

    GLfloat* vertices = new GLfloat[mFrames.size() * 16];
    GLfloat* coordinates = new GLfloat[mFrames.size() * 16];

    for (size_t i = 0; i < mFrames.size(); ++i)
    {
        const Frame& f = mFrames[i];
        const Sheet& s = mSheets[f.sheet];

        Point halves;
        halves.x = float(f.size.x) / 2.0f;
        halves.y = float(f.size.y) / 2.0f;

        float width = float(f.size.x) / float(s.size.x);
        float height = float(f.size.y) / float(s.size.y);

        Point textureUL;
        textureUL.x = float(f.location.x) / float(s.size.x);
        textureUL.y = float(f.location.y) / float(s.size.y);

        Point textureLR;
        textureLR.x = textureUL.x + width;
        textureLR.y = textureUL.y + height;

        for (size_t j = 0; j < 2; ++j)
        {
            size_t k = (i * 16) + (j * 8);
            float offset = width * float(j);

            /// upper right
            vertices[k + 0] = halves.x;
            vertices[k + 1] = halves.y;
            coordinates[k + 0] = textureLR.x + offset;
            coordinates[k + 1] = textureUL.y;

            /// lower right
            vertices[k + 2] = halves.x;
            vertices[k + 3] = -halves.y;
            coordinates[k + 2] = textureLR.x + offset;
            coordinates[k + 3] = textureLR.y;

            /// lower left
            vertices[k + 4] = -halves.x;
            vertices[k + 5] = -halves.y;
            coordinates[k + 4] = textureUL.x + offset;
            coordinates[k + 5] = textureLR.y;

            /// upper left
            vertices[k + 6] = -halves.x;
            vertices[k + 7] = halves.y;
            coordinates[k + 6] = textureUL.x + offset;
            coordinates[k + 7] = textureUL.y;
        }
    }

    try
    {
        if (!mShaderProgram) throw ShaderException("no program pointer");

        mSVBO.loadVAA(mShaderProgram->getBinding("CornerVertex"), 2,
            mFrames.size() * 8, vertices);
        mSVBO.loadVAA(mShaderProgram->getBinding("TexCoord"), 2,
            mFrames.size() * 8, coordinates);
    }
    catch (ShaderException& se)
    {
        cerr << "sprite shader exception -- " << se.reason << endl;
        exit(1);
    }
    catch (...)
    {
        cerr << "unknown sprite exception" << endl;
        exit(1);
    }

    delete [] coordinates;
    delete [] vertices;
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

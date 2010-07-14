#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "DisplayEngine.h"
#include "ShaderProgram.h"
#include "Matrix3D.h"

#include <cmath>
#include <string>

#include <iostream>
using namespace std;

class LoadScreen
{
    public:
        LoadScreen();
        virtual ~LoadScreen();

        void update(unsigned int inPercent);
        void update(float inPercent);

        void setBackgroundImage(char* inBackgroundImage);
        void setLoadImage(char* inLoadImage);
        void setLoadLocation(int inX, int inY);

        void setup();

    private:
        float mScreenRange;
        int mLoadLocationX;
        int mLoadLocationY;
        int mLoadScreenLocationX;
        int mLoadScreenLocationY;
        int mLoadWidth;
        int mLoadHeight;

        Matrix3D mProjection;
        Matrix3D mMVPM;
        GLuint mBackTexture;
        GLuint mLoadTexture;

        ShaderVBO mLoadingBarVBO;
        ShaderProgram mProgram;
        ShaderVBO mBackVBO;
};

#endif // LOADSCREEN_H

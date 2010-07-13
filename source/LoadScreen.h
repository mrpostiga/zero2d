#ifndef LOADSCREEN_H
#define LOADSCREEN_H

#include "DisplayEngine.h"
#include "ShaderProgram.h"
#include "Matrix3D.h"

#include <cmath>
#include <string>

class LoadScreen
{
    public:
        LoadScreen();
        virtual ~LoadScreen();

        void update(unsigned int inPercent);
        void update(float inPercent);

        void setBackgroundImage(char* inBackgroundImage);
        void setLoadImage(char* inLoadImage);
        void setLoadLocation(float inLocation);
        void setRange(float inRange);

        void setup();

    private:
        float mScreenRange;
        float mLoadLocation;

        Matrix3D mProjection;
        Matrix3D mMVPM;
        GLuint mBackTexture;
        GLuint mLoadTexture;

        ShaderProgram mParticleProgram;
        ShaderVBO mParticleVBO;
        ShaderProgram mBackProgram;
        ShaderVBO mBackVBO;
};

#endif // LOADSCREEN_H

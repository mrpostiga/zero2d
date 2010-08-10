#include "Fighter.h"

#include <iostream>
using namespace std;

Fighter::Fighter(const char* inSprite)
{
    mSpriteInstance = new SpriteInstance(new Sprite(inSprite));

    mSpriteProgram.use();
    mSpriteProgram.setFade(1.0f);
    mMaxSpeed = 2.5f;
}

Fighter::~Fighter()
{
    //dtor
}


void Fighter::onCollision(Entity* inEntity)
{

}

void Fighter::move()
{
    mPosition += mMomentum;
    mSpriteInstance->update();
}

void Fighter::display(Matrix3D& inMVPM)
{
    mModelView.loadIdentity();
    mModelView.translate(mPosition[0], mPosition[1], 0);
    mModelView.rotateZ(mRotation);
    mSpriteProgram.use();
    mSpriteProgram.setMatrix(inMVPM * mModelView);

    mSpriteInstance->display();
}

void Fighter::changeDirection(float inDirection)
{

}

void Fighter::moveForward(float inRate)
{
    inRate = (inRate > 1.0f) ? 1.0f : inRate;

    mCurrentSpeed = mMaxSpeed * inRate;

    mMomentum[0] = mCurrentSpeed;

    if (!mSpriteInstance->getFaceRight())
    {
        mMomentum[0] *= -1;
    }
}

void Fighter::moveBack(float inRate)
{
    inRate = (inRate > 1.0f) ? 1.0f : inRate;

    mCurrentSpeed = mMaxSpeed * inRate;

    mMomentum[0] = mCurrentSpeed;

    mSpriteInstance->faceRight(!mSpriteInstance->getFaceRight());

    if (!mSpriteInstance->getFaceRight())
    {
        mMomentum[0] *= -1;
    }
}

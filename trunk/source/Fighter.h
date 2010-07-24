#ifndef FIGHTER_H
#define FIGHTER_H

#include "Entity.h"


#include "SpriteInstance.h"
#include "SpriteProgram.h"

class Fighter : public Entity
{
    public:
        Fighter(char* inSprite);
        virtual ~Fighter();

        virtual void onCollision(Entity* inEntity);
        virtual void move();
        virtual void display(Matrix3D& inMVPM);
        virtual void changeDirection(float inDirection);

        inline SpriteInstance* getSpriteInstance() { return mSpriteInstance; }
    protected:
    private:
        SpriteInstance* mSpriteInstance;
        SpriteProgram mSpriteProgram;

};

#endif // FIGHTER_H

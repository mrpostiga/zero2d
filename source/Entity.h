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

#ifndef ENTITY_H
#define ENTITY_H

#include "Vector2D.h"
#include "Matrix3D.h"
#include "Sprite.h"
#include "ShaderProgram.h"


class Entity
{
    public:
        Entity();
        virtual ~Entity();

        enum Type { FIGHTER, HITBOX };

        virtual void onCollision(Entity* inEntity) = 0;
        virtual void update();
        virtual void move() = 0;
        virtual void display(Matrix3D& inMVPM) = 0;
        virtual void changeDirection(float inDirection) = 0;
        virtual void changeSpeed(float inSpeed) { mMaxSpeed = inSpeed; }

        inline void setGameDead() { mGameDead = true; }
        inline void setRenderDead() { mRenderDead = true; }

        inline bool isGameDead() { return mGameDead; }
        inline bool isRenderDead() { return mRenderDead; }



        inline const Point& getMomentum() const
        {
            return mMomentum;
        }

        void setMomentum(Point inMomentum)
        {
            mMomentum = inMomentum;
            mMomentum.normalizeTo(mCurrentSpeed);
        }

        inline const Point& getPosition() const
        {
            return mPosition;
        }

        inline void setPosition(const Vector2D<float>& inPosition)
        {
            mPosition = inPosition;
        }

        inline void setProgram(ShaderProgram* inProgram) { mProgram = inProgram; }

        inline Type getWhatIAm() { return mWhatAmI; }
        inline bool isAlive() { return mAlive; }
        inline void die() { mAlive = false;}

    protected:
        Type mWhatAmI;

        ShaderProgram* mProgram;

        Matrix3D mTranslation;
        //Matrix3D mRotation;
        float mRotation;
        Matrix3D mModelView;

        bool volatile mGameDead;
        bool volatile mRenderDead;

        Point mMomentum;
        Point mPosition;

        float mMaxSpeed;
        float mCurrentSpeed;

        bool mAlive;
};


#endif

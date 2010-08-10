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

#ifndef CONTROL_H
#define CONTROL_H

#include "Vector3D.h"
#include "Entity.h"


class Control
{
    protected:

    public:
        Control(Entity* inEntity);
        virtual ~Control();

        virtual void update() = 0;
        virtual void onEvent(State::Event inEvent, float inValue = 1.0) = 0;

        Entity::Type getWhatIAm();
        virtual void setGameDead();
        virtual bool isGameDead();
        Entity* getEntity();

    protected:
        Entity* mEntity;

        bool mGameDead;

};

inline void Control::setGameDead()
{
    mGameDead = true;
}

inline bool Control::isGameDead()
{
    return mGameDead;
}

inline Entity* Control::getEntity()
{
    return mEntity;
}

inline Entity::Type Control::getWhatIAm()
{
    return mEntity->getWhatIAm();
}

#endif

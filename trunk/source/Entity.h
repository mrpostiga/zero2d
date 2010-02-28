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

#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "Sprite.h"

#include <list>
using namespace std;

class Entity
{
    public:
        Entity(Sprite* inSprite);
        virtual ~Entity();

        static void unloadAll();

        void pulse();
        void flip();
        void draw();

        void setColorMod(const Vector3D<float>& inVector);
        void setRotation(float inDegrees);
        void setLocation(float inX, float inY);
        void rotate(float inDegrees);

    private:
        static list<Entity*> _entities;

        Sprite* _sprite;
        DrawArgs _args;
        int _pulseCount;
};

inline void Entity::draw()
{
    _sprite->draw(_args);
}

#endif

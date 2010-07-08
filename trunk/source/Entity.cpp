/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Entity.h"
#include "Control.h"

Entity::Entity()
{
    mAlive = true;
    mGameDead = false;
    mRenderDead = false;
}

Entity::~Entity()
{
    //delete mControl;
}

void Entity::update()
{
    //mControl->update();
    move();
}

void Entity::setPosition(const Vector3D<float>& inPosition)
{
    mPosition = inPosition;
}

Entity::EntityType Entity::getWhatIAm()
{
    return mWhatAmI;
}

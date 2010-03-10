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

#include "Entity.h"

list<Entity*> Entity::_entities;

Entity::Entity(Sprite* inSprite) : _sprite(inSprite), _state(0), _subState(0),
    _pulseCount(0)
{
    _blink.duration = 0;
    _blink.tick = 0;

    _stateDatum = _sprite->getState(_state);
    _args.index = _stateDatum->frames[_subState].index;

    _entities.push_back(this);
}

Entity::~Entity()
{
    _entities.remove(this);
}

void Entity::unloadAll()
{
    while (!_entities.empty()) delete _entities.back();
}

void Entity::jumpToState(int inState)
{
    _state = inState;
    _pulseCount = 0;
    _subState = 0;
    _stateDatum = _sprite->getState(_state);
    _args.index = _stateDatum->frames[_subState].index;
}

void Entity::pulse()
{
    ++_pulseCount;

    if (_pulseCount >= _stateDatum->frames[_subState].duration)
    {
        _subState = (_subState + 1) % _stateDatum->frames.size();
        _args.index = _stateDatum->frames[_subState].index;
        _pulseCount = 0;
    }

    if (_blink.duration > 0)
    {
        ++_blink.tick;
        if (_blink.tick >= _blink.duration)
        {
            _blink.tick = 0;
            _blink.blinking = !_blink.blinking;

            if (_blink.blinking)
            {
                _args.colorMod = _blink.blinkColor;
                _args.textureParam = GL_ADD;
            }
            else
            {
                _args.colorMod = _blink.oldColor;
                _args.textureParam = GL_MODULATE;
            }
        }
    }
}

void Entity::flip()
{
    _args.facingRight = !_args.facingRight;
}

void Entity::setColorMod(const Vector3D<float>& inVector)
{
    _args.colorMod = inVector;
}

void Entity::setRotation(float inDegrees)
{
    _args.rotation = inDegrees;
}

void Entity::setLocation(float inX, float inY)
{
    _args.location.x = inX;
    _args.location.y = inY;
}

void Entity::rotate(float inDegrees)
{
    _args.rotation += inDegrees;
    if (_args.rotation > 180.0f) _args.rotation -= 360.0f;
    if (_args.rotation < -180.0f) _args.rotation += 360.0f;
}

void Entity::setBlink(const Vector3D<float>& inVector, int inDuration)
{
    if (_blink.duration > 0)
        _args.colorMod = _blink.oldColor;
    else
        _blink.oldColor = _args.colorMod;

    _blink.blinkColor = inVector;
    _blink.duration = inDuration;
    _blink.tick = 0;
    _blink.blinking = false;
}

void Entity::stopBlink()
{
    _blink.duration = 0;
    _args.colorMod = _blink.oldColor;
    _args.textureParam = GL_MODULATE;
}

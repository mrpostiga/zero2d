#include "PlayerControl.h"

PlayerControl::PlayerControl(Entity* inEntity) : Control(inEntity)
{
    //ctor
}

PlayerControl::~PlayerControl()
{
    //dtor
}

void PlayerControl::update()
{
    mEntity->update();
}

void PlayerControl::onEvent(State::Event inEvent)
{
    SpriteInstance* sprite = ((Fighter*)mEntity)->getSpriteInstance();
    switch (inEvent)
    {
        case State::TILT_RIGHT:
        {
            sprite->faceRight(true);
            mEntity->setMomentum(Point(1.0f, 0.0f));
            break;
        }

        case State::TILT_LEFT:
        {
            sprite->faceRight(false);
            mEntity->setMomentum(Point(-1.0f, 0.0f));
            break;
        }

        case State::ON_END:
        {
            mEntity->setMomentum(Point(0.0f, 0.0f));
        }

        default:
        {
            break;
        }
    }

    sprite->onEvent(inEvent);
}

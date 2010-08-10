#include "PlayerControl.h"

#include <iostream>
using namespace std;

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

/********************************************
*   Special function for player controls to handle the difference
*   between a left/right input and a characters forward/back facing
*   This is not needed for any other type of control
********************************************/
void PlayerControl::onHorizontalDirectionEvent(State::Event inEvent)
{

}

void PlayerControl::onEvent(State::Event inEvent, float inValue)
{
    SpriteInstance* sprite = ((Fighter*)mEntity)->getSpriteInstance();
    Fighter* fighter = ((Fighter*)mEntity);
    switch (inEvent)
    {
        case State::TILT_RIGHT:
        {
            if (sprite->getFaceRight())
            {
                fighter->moveForward(inValue);
                inEvent = State::TILT_FORWARD;
            }
            else
            {
                fighter->moveBack(inValue);
                inEvent = State::TILT_BACK;
            }
            //sprite->faceRight(true);
            //mEntity->setMomentum(Point(1.0f, 0.0f));
            break;
        }

        case State::TILT_LEFT:
        {
            if (!sprite->getFaceRight())
            {
                fighter->moveForward(inValue);
                inEvent = State::TILT_FORWARD;
            }
            else
            {
                fighter->moveBack(inValue);
                inEvent = State::TILT_BACK;
            }

            //sprite->faceRight(false);
            //mEntity->setMomentum(Point(-1.0f, 0.0f));
            break;
        }

        case State::END_TILT:
        {
            mEntity->setMomentum(Point(0.0f, 0.0f));
            break;
        }

        case State::DO_NOTHING:
        {
            return;
            break;
        }

        default:
        {
            break;
        }
    }

    sprite->onEvent(inEvent);
}

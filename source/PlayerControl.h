#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include "Control.h"

#include "Fighter.h"

class PlayerControl : public Control
{
    public:
        PlayerControl(Entity* inEntity);
        virtual ~PlayerControl();

        virtual void update();

        void onHorizontalDirectionEvent(State::Event inEvent);
        void onEvent(State::Event inEvent, float inValue = 1.0);

    protected:
    private:
};

#endif // PLAYERCONTROL_H

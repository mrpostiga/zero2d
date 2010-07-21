#ifndef STATE_H
#define STATE_H


class State
{
    public:
        State();
        virtual ~State();

        enum Event { ATTACK = 0, DEFEND = 1, JUMP = 2, TILT_FORWARD = 3,
            TILT_BACK = 4, TILT_UP = 5, TILT_DOWN = 6, SMASH_FORWARD = 7,
            SMASH_BACK = 8, SMASH_UP = 9, SMASH_DOWN = 10 };

        State* onEvent(Event inEvent);

        void setStateMapping(Event inEvent, State* inState);

    protected:
    private:
        State* mEventMappings[11];
};

#endif // STATE_H

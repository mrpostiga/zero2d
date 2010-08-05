#ifndef STATE_H
#define STATE_H

#include <cstdlib>

#define NUM_STATES 16

/*********************************
*   Maintains mappings from this state to other states
*   according to what events happen from this state
*********************************/
class State
{
    public:
        struct FrameRate
        {
            size_t frameIndex;
            unsigned int duration;
        };

        State(size_t inNumFrames);
        virtual ~State();

        /*******
        *   These enums are defined both here and in the lua initialization
        *   script 'api.lua'.  If you change them in one place, make sure
        *   you change them in the other as well!!!
        *********/
        enum States { STAND = 0, RUN = 1 };

        enum Event {
            DO_NOTHING = 0,
            ON_END = 1,
            ATTACK = 2,
            DEFEND = 3,
            JUMP = 4,
            TILT_RIGHT = 5,
            TILT_LEFT = 6,
            TILT_UP = 7,
            TILT_DOWN = 8,
            TILT_RIGHT_ATTACK = 9,
            TILT_LEFT_ATTACK = 10,
            TILT_UP_ATTACK = 11,
            SMASH_RIGHT = 12,
            SMASH_LEFT = 13,
            SMASH_UP = 14,
            SMASH_DOWN = 15 };

        inline size_t getNumFrames() { return mSize; }
        inline FrameRate getFrame(size_t inIndex) { return mAnimation[inIndex]; }

        State* onEvent(Event inEvent);

        void setStateMapping(Event inEvent, State* inState, bool inCallLua);

        void setFrame(size_t inFrame, size_t inFrameIndex, unsigned int inFrameDuration);

    protected:
    private:
        State* mEventMappings[NUM_STATES];
        bool mCallLuaOnEvent[NUM_STATES];
        size_t mSize;
        FrameRate* mAnimation;

};

#endif // STATE_H

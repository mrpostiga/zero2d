#ifndef STATE_H
#define STATE_H

#include <cstdlib>

#define NUM_STATES 21

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
        enum States {
            STAND = 0,
            RUN = 1 };


        /****************************
        *   Events to control the state tree
        *   The extra left/right events at the end
        *   are special for player controls only and are excluded
        *   from the array on purpose
        *   DO NOT MODIFY THEM
        *******************************/
        enum Event {
            ON_ANIMATION_END = 0,
            ATTACK = 1,
            SPECIAL = 2,
            DEFEND = 3,
            JUMP = 4,
            END_ATTACK = 5,
            END_SPECIAL = 6,
            END_DEFEND = 7,
            END_JUMP = 8,
            END_TILT = 9,
            TILT_FORWARD = 10,
            TILT_BACK = 11,
            TILT_UP = 12,
            TILT_DOWN = 13,
            SMASH_FORWARD = 14,
            SMASH_BACK = 15,
            SMASH_UP = 16,
            SMASH_DOWN = 17,
            GOT_HIT = 18,
            GOT_GRABBED = 19,
            GOT_RELEASED = 20,
            DO_NOTHING,
            TILT_LEFT,
            TILT_RIGHT,
            SMASH_LEFT,
            SMASH_RIGHT };

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

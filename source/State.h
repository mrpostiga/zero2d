#ifndef STATE_H
#define STATE_H

#include <cstdlib>

#define NUM_STATES 17

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


        /****************************
        *   Events to control the state tree
        *   The extra left/right events at the end
        *   are special for player controls only and are excluded
        *   from the array on purpose
        *   DO NOT MODIFY THEM
        *******************************/
        enum Event {
            DO_NOTHING = 0,
            ON_END = 1,
            ON_ANIMATION_END = 2,
            ATTACK = 3,
            DEFEND = 4,
            JUMP = 5,
            TILT_RIGHT = 6,
            TILT_LEFT = 7,
            TILT_UP = 8,
            TILT_DOWN = 9,
            TILT_RIGHT_ATTACK = 10,
            TILT_LEFT_ATTACK = 11,
            TILT_UP_ATTACK = 12,
            SMASH_RIGHT = 13,
            SMASH_LEFT = 14,
            SMASH_UP = 15,
            SMASH_DOWN = 16,
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

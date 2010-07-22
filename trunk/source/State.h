#ifndef STATE_H
#define STATE_H

#include <cstdlib>

#define NUM_STATES 12

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

        enum Event { ON_END = 0, DEFEND = 1, JUMP = 2, TILT_FORWARD = 3,
            TILT_BACK = 4, TILT_UP = 5, TILT_DOWN = 6, SMASH_FORWARD = 7,
            SMASH_BACK = 8, SMASH_UP = 9, SMASH_DOWN = 10, ATTACK = 11 };

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

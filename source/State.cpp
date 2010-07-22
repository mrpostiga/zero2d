#include "State.h"

#include <iostream>
using namespace std;

State::State(size_t inNumFrames)
{
    mAnimation = new FrameRate[inNumFrames];
    mSize = inNumFrames;

    for (int i = 0; i < NUM_STATES; ++i)
    {
        mEventMappings[i] = NULL;
        mCallLuaOnEvent[i] = false;
    }
}

State::~State()
{
    delete [] mAnimation;
}

/**********************************
*   Returns the state to go to on the passed in event
*   from this state
**********************************/
State* State::onEvent(State::Event inEvent)
{
    return mEventMappings[inEvent];
}

/***********************************
*   Sets which state should be next from this state
*   when inEvent happens
***********************************/
void State::setStateMapping(Event inEvent, State* inState, bool inCallLua)
{
    mEventMappings[inEvent] = inState;
    mCallLuaOnEvent[inEvent] = inCallLua;
}

void State::setFrame(size_t inFrame, size_t inFrameIndex, unsigned int inFrameDuration)
{
    mAnimation[inFrame].frameIndex = inFrameIndex;
    mAnimation[inFrame].duration = inFrameDuration;
}

#include "State.h"

using namespace std;

State::State()
{

}

State::~State()
{
    //dtor
}

State* State::onEvent(State::Event inEvent)
{
    return mEventMappings[inEvent];
}

void State::setStateMapping(Event inEvent, State* inState)
{
    mEventMappings[inEvent] = inState;
}

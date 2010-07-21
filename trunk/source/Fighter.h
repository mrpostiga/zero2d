#ifndef FIGHTER_H
#define FIGHTER_H

#include "Entity.h"

#include "State.h"
#include <vector>

class Fighter : public Entity
{
    public:
        Fighter();
        virtual ~Fighter();

    protected:
    private:
        std::vector<State> mStateTree;
};

#endif // FIGHTER_H

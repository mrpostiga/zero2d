#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "Module.h"
#include "NetworkStream.h"

class TestModule : public Module
{
    public:
        TestModule();
        virtual ~TestModule();

        /// module operation
        virtual bool onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:

    private:
        Uint32 mTimer;
        NetworkStream mNet;
};

#endif

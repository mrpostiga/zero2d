#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "Module.h"
#include "ShaderProgram.h"

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
        ShaderProgram mSP;
        Shader* mShaders[2];
        ShaderVBO mSVBO;
};

#endif

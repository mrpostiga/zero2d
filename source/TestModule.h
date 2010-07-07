#ifndef TESTMODULE_H
#define TESTMODULE_H

#include "Module.h"
#include "MatrixStack.h"
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
        float mRotation;
        Matrix3D mProjection;
        MatrixStack mModelView;
        ShaderProgram mSP;
        ShaderVBO mSVBO;
};

#endif

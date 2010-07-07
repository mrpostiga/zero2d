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
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);

    private:
        float mRotation;
        float mTime;
        Uint32 mTickStart;
        Matrix3D mProjection;
        Matrix3D mMVPM;
        MatrixStack mModelView;
        ShaderProgram mSP;
        ShaderVBO mSVBO;
};

#endif

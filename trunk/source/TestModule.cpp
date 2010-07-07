#include "TestModule.h"

TestModule::TestModule()
{
}

TestModule::~TestModule()
{
}

bool TestModule::onLoad()
{
    mSP.attachShader(new Shader("data/shaders/test1.vs"));
    mSP.attachShader(new Shader("data/shaders/test1.fs"));

    const GLfloat diamond[8] = {
    0.0,  1.0, /* Top point */
    1.0,  0.0, /* Right point */
    0.0, -1.0, /* Bottom point */
    -1.0,  0.0}; /* Left point */

    const GLfloat colors[12] = {
      1.0,  0.0,  0.0  , /* Red */
      0.0,  1.0,  0.0  , /* Green */
      0.0,  0.0,  1.0  , /* Blue */
      1.0,  1.0,  1.0  }; /* White */

    const GLuint indices[6] = {0, 1, 2, 0, 2, 3};

    mSVBO.loadVAA("in_Position", 2, 8, diamond);
    mSVBO.loadVAA("in_Color", 3, 12, colors);
    mSVBO.loadIA(GL_TRIANGLES, 6, indices);
    mSP.bindAttributeLocations(mSVBO);

    mRotation = 0.0f;
    float ratio = float(SDL_GetVideoSurface()->w)
        / float(SDL_GetVideoSurface()->h);
    //mProjection.orthographic(1.5f, ratio);
    mProjection.perspective(30.0f, ratio, 1.0f, 100.0f);
    mModelView.matrix().translate(0.0f, 0.0f, -10.0f);

    return true;
}

void TestModule::onUnload()
{
}

void TestModule::onOpen()
{
    mSP.use();
}

void TestModule::onClose()
{

}

void TestModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT);
    mModelView.push();
    mModelView.matrix().rotateX(mRotation);
    mModelView.matrix().rotateY(mRotation);
    mSP.setMatrix(mProjection * mModelView.matrix());
    mSVBO.display();
    mModelView.pop();
}

void TestModule::onFrame()
{
    mRotation += 1.0f;
    if (mRotation > 180.0f) mRotation -= 360.0f;
}

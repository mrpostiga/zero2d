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
    mSVBO.display();
}

void TestModule::onFrame()
{

}

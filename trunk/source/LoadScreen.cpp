#include "LoadScreen.h"

LoadScreen::LoadScreen()
{
}

LoadScreen::~LoadScreen()
{
    //dtor
}

void LoadScreen::update(unsigned int inPercent)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mBackProgram.use();
    glBindTexture(GL_TEXTURE_2D, mBackTexture);
    mBackVBO.displayLinear(GL_QUADS, 0, 4);
    DisplayEngine::render();
}

void LoadScreen::update(float inPercent)
{
    inPercent = fabs(inPercent);

    if (inPercent > 1.0f)
    {
        inPercent = 1.0f;
    }

    update(inPercent * 100);
}

void LoadScreen::setup()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    mBackProgram.attachShader(new Shader("sprite.vs"));
    mBackProgram.attachShader(new Shader("sprite.fs"));
    mBackProgram.addVariable("CornerVertex");
    mBackProgram.addVariable("TexCoord");
    mBackProgram.bindAndLink();

    GLfloat* vertices = new GLfloat[8];
    GLfloat* texCoord = new GLfloat[8];

    vertices[0] = 1024.0f;
    vertices[1] = 1024.0f;
    vertices[2] = 1024.0f;
    vertices[3] = -1024.0f;
    vertices[4] = -1024.0f;
    vertices[5] = -1024.0f;
    vertices[6] = -1024.0f;
    vertices[7] = 1024.0f;


    texCoord[0] = 1.0f;
    texCoord[1] = 0.0f;
    texCoord[2] = 1.0f;
    texCoord[3] = 1.0f;
    texCoord[4] = 0.0f;
    texCoord[5] = 1.0f;
    texCoord[6] = 0.0f;
    texCoord[7] = 0.0f;

    mBackVBO.loadVAA(mBackProgram.getBinding("CornerVertex"), 2, 4,
        vertices);
    mBackVBO.loadVAA(mBackProgram.getBinding("TexCoord"), 2, 4,
        texCoord);

    GLint texLoc = mBackProgram.getUniformLocation("Texture");
    glUniform1i(texLoc, 0);

    GLint z = mBackProgram.getUniformLocation("z");
    glUniform1f(z, 0.0f);

    mBackProgram.setMatrix(mProjection);
}

void LoadScreen::setBackgroundImage(char* inBackgroundImage)
{
    glGenTextures(1, &mBackTexture);
    std::string path = "data/images/";
    path += inBackgroundImage;
    DisplayEngine::loadTexture(path.c_str(), mBackTexture);
}

void LoadScreen::setLoadImage(char* inLoadImage)
{
    glGenTextures(1, &mLoadTexture);
    std::string path = "data/images/";
    path += inLoadImage;
    DisplayEngine::loadTexture(path.c_str(), mLoadTexture);
}

void LoadScreen::setLoadLocation(float inLocation)
{
    mLoadLocation = inLocation;
}

void LoadScreen::setRange(float inRange)
{
    mScreenRange = inRange;

    float ratio = DisplayEngine::getAspectRatio();
    mProjection.orthographic(inRange, ratio);
}

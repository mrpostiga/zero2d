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

    mProgram.use();
    glBindTexture(GL_TEXTURE_2D, mBackTexture);
    mBackVBO.displayLinear(GL_QUADS, 0, 4);

    glBindTexture(GL_TEXTURE_2D, mLoadTexture);
    mLoadingBarVBO.displayLinear(GL_QUADS, 0, 4);
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

    mProgram.attachShader(new Shader("sprite.vs"));
    mProgram.attachShader(new Shader("sprite.fs"));
    mProgram.addVariable("CornerVertex");
    mProgram.addVariable("TexCoord");
    mProgram.bindAndLink();

    GLfloat* vertices = new GLfloat[8];
    GLfloat* texCoord = new GLfloat[8];

    float ratio = DisplayEngine::getAspectRatio();
    mProjection.orthographic(mScreenRange, ratio);

    vertices[0] = mScreenRange * ratio;
    vertices[1] = mScreenRange;
    vertices[2] = mScreenRange * ratio;
    vertices[3] = -mScreenRange;
    vertices[4] = -mScreenRange * ratio;
    vertices[5] = -mScreenRange;
    vertices[6] = -mScreenRange * ratio;
    vertices[7] = mScreenRange;


    texCoord[0] = 1.0f;
    texCoord[1] = 0.0f;
    texCoord[2] = 1.0f;
    texCoord[3] = 1.0f;
    texCoord[4] = 0.0f;
    texCoord[5] = 1.0f;
    texCoord[6] = 0.0f;
    texCoord[7] = 0.0f;

    mBackVBO.loadVAA(mProgram.getBinding("CornerVertex"), 2, 4,
        vertices);
    mBackVBO.loadVAA(mProgram.getBinding("TexCoord"), 2, 4,
        texCoord);

    GLint texLoc = mProgram.getUniformLocation("Texture");
    glUniform1i(texLoc, 0);

    GLint z = mProgram.getUniformLocation("z");
    glUniform1f(z, 0.0f);

    mProgram.setMatrix(mProjection);


    vertices[0] = 100.0f;
    vertices[1] = 10.0f;
    vertices[2] = 100.0f;
    vertices[3] = -10.0f;
    vertices[4] = -100.0f;
    vertices[5] = -10.0f;
    vertices[6] = -100.0f;
    vertices[7] = 10.0f;


    texCoord[0] = 1.0f;
    texCoord[1] = 0.0f;
    texCoord[2] = 1.0f;
    texCoord[3] = 1.0f;
    texCoord[4] = 0.0f;
    texCoord[5] = 1.0f;
    texCoord[6] = 0.0f;
    texCoord[7] = 0.0f;

    mLoadingBarVBO.loadVAA(mProgram.getBinding("CornerVertex"), 2, 4,
        vertices);
    mLoadingBarVBO.loadVAA(mProgram.getBinding("TexCoord"), 2, 4,
        texCoord);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLint fade = mProgram.getUniformLocation("fade");
    glUniform1f(fade, 1.0);

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

void LoadScreen::setLoadLocation(float inX, float inY)
{
    mLoadLocationX = inX;
    mLoadLocationY = inY;
}

void LoadScreen::setRange(float inRange)
{
    mScreenRange = inRange;
}

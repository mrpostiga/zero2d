/**
 *  This file is part of Zero2D.
 *
 *  Zero2D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Zero2D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Zero2D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DisplayEngine.h"
#include "Shader.h"
#include "Module.h"
#include "GameEngine.h"
#include "GameController.h"

#include "OGL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <boost/filesystem.hpp>

#if SDL_IMAGE_PATCHLEVEL == 9
// SDL_image 1.2.9 has a bug that prevents it from loading more than one image.
#error Do not use SDL_image 1.2.9
#endif

#include <list>
#include <fstream>
#include <sstream>
using namespace std;

Pixel DisplayEngine::mDisplaySize;
Surface DisplayEngine::mDisplay = NULL;
Surface DisplayEngine::mWindowIcon = NULL;
SDL_Rect** DisplayEngine::mModes = NULL;
int DisplayEngine::mMipmapping = 0;
ColorMask DisplayEngine::mMask;
unsigned int DisplayEngine::mFPS = 0;
string DisplayEngine::mShaderFolder("data/shaders/");
float DisplayEngine::mAspectRatio;
LogFile DisplayEngine::mLogFile;

void DisplayEngine::start(Module* inModule)
{
    if (inModule == NULL)
    {
        cleanup();
        return;
    }

    Module* currentModule = inModule;

    list<Module*> moduleStack;

    Uint32 nextFrame = SDL_GetTicks() + FRAME_LENGTH;
    SDL_Event event;

    while (currentModule != NULL || moduleStack.size() > 0)
    {
        if (currentModule == NULL)
        {
            currentModule = moduleStack.back();
            moduleStack.pop_back();
        }

        try
        {
            currentModule->onLoad();
        }
        catch (const CoreException& ce)
        {
            cerr << ce.type << " -- " << ce.reason << endl;
            currentModule = NULL;
        }
        catch (...)
        {
            cerr << "unknown exception" << endl;
            currentModule = NULL;
        }

        if (!currentModule) break;

        currentModule->onOpen();

        unsigned int nextSecond = SDL_GetTicks() + 1000u;
        int framesPerSecond = 0;
        mFPS = 0; //setup the frames per second

        while (currentModule->isRunning())
        {
            while (SDL_PollEvent(&event)) currentModule->onEvent(&event);

            unsigned int ticks = SDL_GetTicks();

            if (ticks > nextSecond)
            {
                nextSecond += 1000u;

                //store the # of frames printed this second
                mFPS = framesPerSecond;
                framesPerSecond = 0;
            }

            if (ticks > nextFrame)
            {
                currentModule->onFrame();
                nextFrame += FRAME_LENGTH;
            }
            else
            {
                currentModule->onLoop();
                //SDL_GL_SwapBuffers();
                render();
                ++framesPerSecond;
            }

            SDL_Delay(1); // prevent CPU abuse
        }

        currentModule->onClose();

        Module* deadModule = currentModule;
        currentModule = currentModule->next();

        if (deadModule->isDead())
        {
            // LOL @ "if dead module is dead..."
            deadModule->onUnload();
            delete deadModule;
        }
        else
        {
            moduleStack.push_back(deadModule);
            deadModule = NULL;
        }

    }

    cleanup();
}

void DisplayEngine::initialize()
{
    {
        using namespace boost::filesystem;

        //delete any old log files
        string logsDir(GameEngine::getUserFolder());
        logsDir += "logs/";

        if (is_directory(logsDir))
        {
            for (directory_iterator itr(logsDir); itr != directory_iterator();
                ++itr)
            {
                if (is_regular_file(itr->status()))
                {
                    remove(itr->path());
                }
            }
        }
    }

    mLogFile.start("ogl");

    putenv((char*)"SDL_VIDEO_CENTERED=1");

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        cerr << "-- error on SDL_Init --" << endl;
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        cerr << "-- error on TTF_Init -- " << TTF_GetError() << endl;
        exit(1);
    }

    if (SDLNet_Init() < 0)
    {
        cerr << "-- error on SDLNet_Init -- " << SDLNet_GetError() << endl;
        exit(1);
    }

    GameController::loadAll();


#ifdef __WIN32__
    // redirect output to screen (instead of text files)
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
#endif

    // get available full screen modes
    mModes = SDL_ListModes(NULL, SDL_FULLSCREEN);

    if (mModes == (SDL_Rect**)0)
    {
        cout << "No modes available!\n";
    }
    else if (mModes == (SDL_Rect**)-1)
    {
        cout << "All resolutions available.\n";
    }
    else
    {
        cout << "-- Available Full Screen Resolutions --\n";
        for (int i = 0; mModes[i]; ++i)
            cout << "  " << mModes[i]->w << " x " << mModes[i]->h << endl;
    }

    stringstream logStream;

    int setting;
    /**
     *  All of the SDL OpenGL settings need to be changed from outside the
     *  program. Difficulties with ATI graphics cards seem to result from
     *  improper settings here. This is will allow anyone to tweak settings
     *  appropriately. If the setting file specifies a negative value, its
     *  respective setting is not even set.
     *
     *  The attributes are all listed at
     *  http://www.libsdl.org/cgi/docwiki.cgi/SDL_GLattr
     */


    logStream << "setting SDL OpenGL settings (0 indicates success)" << endl
        << endl;

    #define SET_SDL_SETTING(n) setting = GameEngine::config.get<int>(#n, -1); \
        if (setting >= 0) { logStream << #n << " -- " \
            << SDL_GL_SetAttribute(n, setting) << endl; }

    SET_SDL_SETTING(SDL_GL_RED_SIZE);
    SET_SDL_SETTING(SDL_GL_GREEN_SIZE);
    SET_SDL_SETTING(SDL_GL_BLUE_SIZE);
    SET_SDL_SETTING(SDL_GL_ALPHA_SIZE);
    SET_SDL_SETTING(SDL_GL_BUFFER_SIZE);
    SET_SDL_SETTING(SDL_GL_DOUBLEBUFFER);
    SET_SDL_SETTING(SDL_GL_DEPTH_SIZE);
    SET_SDL_SETTING(SDL_GL_STENCIL_SIZE);
    SET_SDL_SETTING(SDL_GL_ACCUM_RED_SIZE);
    SET_SDL_SETTING(SDL_GL_ACCUM_GREEN_SIZE);
    SET_SDL_SETTING(SDL_GL_ACCUM_BLUE_SIZE);
    SET_SDL_SETTING(SDL_GL_ACCUM_ALPHA_SIZE);
    SET_SDL_SETTING(SDL_GL_STEREO);
    SET_SDL_SETTING(SDL_GL_MULTISAMPLEBUFFERS);
    SET_SDL_SETTING(SDL_GL_MULTISAMPLESAMPLES);
    SET_SDL_SETTING(SDL_GL_SWAP_CONTROL);
    SET_SDL_SETTING(SDL_GL_ACCELERATED_VISUAL);

    int width = GameEngine::config.get<int>("display width", 800);
    int height = GameEngine::config.get<int>("display height", 600);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    mMask.red   = 0xff000000;
    mMask.green = 0x00ff0000;
    mMask.blue  = 0x0000ff00;
    mMask.alpha = 0x000000ff;
#else
    mMask.red   = 0x000000ff;
    mMask.green = 0x0000ff00;
    mMask.blue  = 0x00ff0000;
    mMask.alpha = 0xff000000;
#endif


    Uint32 flags = SDL_OPENGL;

    if (GameEngine::config.get<int>("full screen", 0) == 1) flags |= SDL_FULLSCREEN;

    SDL_WM_SetCaption("Zero2D - Development Build", "Zero2D");
    mDisplay = SDL_SetVideoMode(width, height,
        GameEngine::config.get<int>("bits per pixel", 24), flags);
    mDisplaySize[0] = mDisplay->w;
    mDisplaySize[1] = mDisplay->h;


    logStream << endl << "current SDL OpenGL settings" << endl << endl;

    int value;

    #define GET_SDL_SETTING(n) SDL_GL_GetAttribute(n, &value); \
        logStream << #n << " : " << value << endl;

    GET_SDL_SETTING(SDL_GL_RED_SIZE);
    GET_SDL_SETTING(SDL_GL_GREEN_SIZE);
    GET_SDL_SETTING(SDL_GL_BLUE_SIZE);
    GET_SDL_SETTING(SDL_GL_ALPHA_SIZE);
    GET_SDL_SETTING(SDL_GL_BUFFER_SIZE);
    GET_SDL_SETTING(SDL_GL_DOUBLEBUFFER);
    GET_SDL_SETTING(SDL_GL_DEPTH_SIZE);
    GET_SDL_SETTING(SDL_GL_STENCIL_SIZE);
    GET_SDL_SETTING(SDL_GL_ACCUM_RED_SIZE);
    GET_SDL_SETTING(SDL_GL_ACCUM_GREEN_SIZE);
    GET_SDL_SETTING(SDL_GL_ACCUM_BLUE_SIZE);
    GET_SDL_SETTING(SDL_GL_ACCUM_ALPHA_SIZE);
    GET_SDL_SETTING(SDL_GL_STEREO);
    GET_SDL_SETTING(SDL_GL_MULTISAMPLEBUFFERS);
    GET_SDL_SETTING(SDL_GL_MULTISAMPLESAMPLES);
    GET_SDL_SETTING(SDL_GL_SWAP_CONTROL);
    GET_SDL_SETTING(SDL_GL_ACCELERATED_VISUAL);
    mLogFile.addLine(logStream.str());

#ifndef __APPLE__
    // OSX does not support window icons
    mWindowIcon = loadImage("data/images/icon.bmp");
    if (mWindowIcon != NULL)
    {
        SDL_WM_SetIcon(mWindowIcon, NULL);
    }
    else
    {
        cerr << "window icon failed" << endl;
    }
#endif

    mMipmapping = GameEngine::config.get<int>("mipmapping", 1);
    if (mMipmapping == 1)
    {
        if (GLEE_ARB_framebuffer_object || GLEE_EXT_framebuffer_object)
        {
            mLogFile.addLine("extension found -- using glGenerateMipmap");
            mMipmapping = 3;
        }
        else
        {
            mLogFile.addLine("extension not found -- using SGI method");
            mMipmapping = 2;
        }
    }

    int shaderSetting = GameEngine::config.get<int>("shader", 1);
    double shaderVersion;
    stringstream ss;
    ss << glGetString(GL_SHADING_LANGUAGE_VERSION);
    ss >> shaderVersion;
    if (shaderSetting == 2 || (shaderSetting == 1 && shaderVersion >= 1.4))
    {
        mLogFile.addLine("using high version shaders");
        mShaderFolder += "high/";
    }
    else
    {
        mLogFile.addLine("using low version shaders");
        mShaderFolder += "low/";
    }

    GLsizei w = SDL_GetVideoSurface()->w;
    GLsizei h = SDL_GetVideoSurface()->h;
    mAspectRatio = float(w) / float(h);
    glViewport(0, 0, w, h);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    openGLDriverInfo(mLogFile);
}

void DisplayEngine::cleanup()
{
    GameController::unloadAll();
    Shader::unloadAll();

#ifndef __APPLE__
    SDL_FreeSurface(mWindowIcon);
#endif

    SDLNet_Quit();
    TTF_Quit();
    SDL_Quit();
}

Surface DisplayEngine::loadImage(const char* inFile)
{
    Surface t = IMG_Load(inFile);
    Surface outSurface = NULL;

    if (t == NULL)
    {
        //cerr << "failed to load image: " << inFile << endl;
        return NULL;
    }

    outSurface = SDL_DisplayFormatAlpha(t);
    SDL_FreeSurface(t);

    return outSurface;
}

bool DisplayEngine::printErrors(const char* inMessage, ostream& inStream)
{
    GLenum error;
    bool isError = false;

    error = glGetError();

    if (error != GL_NO_ERROR)
    {
        inStream << inMessage;
        isError = true;
    }

    while (error != GL_NO_ERROR)
    {
        inStream << gluErrorString(error) << endl;
        error = glGetError();
    }
    return isError;
}

Pixel DisplayEngine::loadTexture(Surface inSurface, GLuint inTexture,
    bool inDelete)
{
    Pixel outSize;
    if (!inSurface)
    {
        inDelete = true;
        Uint32 flags = SDL_SWSURFACE | SDL_ASYNCBLIT;
        int bits = 0;

        Surface t;
        t = SDL_CreateRGBSurface(flags, 1, 1, bits, mMask.red, mMask.green,
            mMask.blue, mMask.alpha);
        inSurface = SDL_DisplayFormat(t);
        SDL_FreeSurface(t);
    }

    outSize[0] = inSurface->w;
    outSize[1] = inSurface->h;

    glBindTexture(GL_TEXTURE_2D, inTexture);

    GLint nOfColors = inSurface->format->BytesPerPixel;
    GLenum tFormat = GL_RGBA;
    if (nOfColors == 4)
    {
        if (inSurface->format->Rmask == 0x000000ff)
            tFormat = GL_RGBA;
        else
            tFormat = GL_BGRA;
    }
    else if (nOfColors == 3)
    {
        if (inSurface->format->Rmask == 0x000000ff)
            tFormat = GL_RGB;
        else
            tFormat = GL_BGR;
    }
    else
    {
        cerr << "failed to load texture -- not true color\n";
        SDL_FreeSurface(inSurface);
        return false;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (mMipmapping == 2)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, inSurface->w, inSurface->h,
        0, tFormat, GL_UNSIGNED_BYTE, inSurface->pixels);

    if (mMipmapping == 3)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if (inDelete) SDL_FreeSurface(inSurface);
    return outSize;
}

Pixel DisplayEngine::loadTexture(const char* inFile, GLuint inTexture)
{
    Surface t = loadImage(inFile);
    return loadTexture(t, inTexture);
}

void DisplayEngine::openGLDriverInfo(ostream& inStream)
{
    inStream << "Vendor: " << (char*)glGetString(GL_VENDOR) << endl;
    inStream << "Renderer: " << (char*)glGetString(GL_RENDERER) << endl;
    inStream << "OpenGL version: " << (char*)glGetString(GL_VERSION) << endl;
    inStream << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
        << endl;

    inStream << "\n---[ EXTENSIONS ]---" << endl;

    string stuff((char*)glGetString(GL_EXTENSIONS));

    for (size_t i = 0; i < stuff.length(); ++i)
    {
        size_t j = stuff.find_first_of(' ', i);
        if (j != string::npos)
        {
            inStream << stuff.substr(i, j - i) << endl;
            i = j;
        }
        else
        {
            inStream << stuff.substr(i) << endl;
            i = stuff.length();
        }
    }
    inStream << endl;

    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    inStream << "\nmax texture size -- " << maxTextureSize << endl;
}

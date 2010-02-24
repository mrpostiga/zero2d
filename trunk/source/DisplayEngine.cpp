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
#include "Module.h"
#include "Config.h"

#include <SDL_opengl.h>
#include <SDL_image.h>

#if SDL_IMAGE_PATCHLEVEL == 9
// SDL_image 1.2.9 has a bug that prevents it from loading more than one image.
#error Do not use SDL_image 1.2.9
#endif

#include <list>
#include <ctime>
#include <fstream>

Surface DisplayEngine::_display = NULL;
Surface DisplayEngine::_windowIcon = NULL;
SDL_Rect** DisplayEngine::_modes = NULL;
bool DisplayEngine::_mipmapping = false;
Mask DisplayEngine::_mask;

void DisplayEngine::start(Module* inModule)
{
    if (inModule == NULL) return;
    Module* currentModule = inModule;

    list<Module*> moduleStack;

    unsigned nextFrame = SDL_GetTicks() + FRAME_LENGTH;
    SDL_Event event;

    while (currentModule != NULL || moduleStack.size() > 0)
    {
        if (currentModule == NULL)
        {
            currentModule = moduleStack.back();
            moduleStack.pop_back();
        }
        else if (!currentModule->onLoad())
        {
            cerr << "module failed to load" << endl;
            currentModule = NULL;
            break;
        }

        currentModule->onInit();


        unsigned int nextSecond = SDL_GetTicks() + 1000u;
        int framesPerSecond = 0;

        while (currentModule->isRunning())
        {
            while (SDL_PollEvent(&event)) currentModule->onEvent(&event);

            currentModule->onLoop();
            SDL_GL_SwapBuffers();
            ++framesPerSecond;

            unsigned int ticks = SDL_GetTicks();

            if (ticks > nextSecond)
            {
                //cout << framesPerSecond << " frames per second" << endl;
                nextSecond += 1000u;
                framesPerSecond = 0;
            }

            if (ticks > nextFrame)
            {
                currentModule->onFrame();
                nextFrame += FRAME_LENGTH;
            }

            SDL_Delay(1); // prevent CPU abuse
        }

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
    putenv((char*)"SDL_VIDEO_CENTERED=1");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cerr << "error on SDL_Init" << endl;
        exit(1);
    }

    #ifdef __WIN32__
    // redirect output to screen (instead of text files)
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    #endif

    // get available full screen modes
    _modes = SDL_ListModes(NULL, SDL_FULLSCREEN);

    if (_modes == (SDL_Rect**)0)
    {
        cout << "No modes available!\n";
    }
    else if (_modes == (SDL_Rect**)-1)
    {
        cout << "All resolutions available.\n";
    }
    else
    {
        cout << "-- Available Full Screen Resolutions --\n";
        for (int i = 0; _modes[i]; ++i)
            cout << "  " << _modes[i]->w << " x " << _modes[i]->h << endl;
    }

    cerr << "SDL_GL_ACCELERATED_VISUAL -- "
        << SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) << endl;
    cerr << "SDL_GL_DOUBLEBUFFER -- "
        << SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) << endl;

    int width = Config::get<int>("display width", 800);
    int height = Config::get<int>("display height", 600);

    _mipmapping = Config::get<bool>("mipmapping", false);

    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        _mask.red   = 0xff000000;
        _mask.green = 0x00ff0000;
        _mask.blue  = 0x0000ff00;
        _mask.alpha = 0x000000ff;
    }
    else
    {
        _mask.red   = 0x000000ff;
        _mask.green = 0x0000ff00;
        _mask.blue  = 0x00ff0000;
        _mask.alpha = 0xff000000;
    }


    Uint32 flags = SDL_OPENGL;

    if (Config::get<int>("full screen", 0) == 1) flags |= SDL_FULLSCREEN;

    _display = SDL_SetVideoMode(width, height,
        Config::get<int>("bits per pixel", 32), flags);

    #ifndef __APPLE__
    // OSX does not support window icons
    _windowIcon = SDL_LoadBMP("data/images/icon.bmp");
    if (_windowIcon != NULL) SDL_WM_SetIcon(_windowIcon, NULL);
    #endif

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_WM_SetCaption("Zero2D version 0.0.1", "Zero2D");

    ofstream logFile;
    stringstream ss;
    ss << "data/logs/ogl-" << time(NULL) << ".txt";
    //cerr << "logging " << ss.str() << endl;
    logFile.open(ss.str().c_str(), ios::trunc);
    if (logFile.fail())
    {
        cerr << "failed to log: " << ss.str() << endl;
        return;
    }
    logOpenGL(logFile);
    logFile.close();
}

void DisplayEngine::cleanup()
{
    #ifndef __APPLE__
    SDL_FreeSurface(_windowIcon);
    #endif

    SDL_Quit();
}

Surface DisplayEngine::loadImage(const char* inFile)
{
    Surface t = IMG_Load(inFile);
    Surface outSurface = NULL;

    if (t == NULL)
    {
        cerr << "failed to load image: " << inFile << endl;
        return NULL;
    }

    outSurface = SDL_DisplayFormatAlpha(t);
    SDL_FreeSurface(t);

    return outSurface;
}

void DisplayEngine::printErrors()
{
    GLenum error;

    error = glGetError();

    if (error != GL_NO_ERROR)
    {
        cerr << "Opengl Error: ";
    }

    while (error != GL_NO_ERROR)
    {
        switch(error)
        {
            case GL_INVALID_ENUM:
            {
                cerr << "Invalid enum." << endl;
                break;
            }
            case GL_INVALID_VALUE:
            {
                cerr << "Invalid value." << endl;
                break;
            }
            case GL_INVALID_OPERATION:
            {
                cerr << "Invalid operation." << endl;
                break;
            }
            case GL_STACK_OVERFLOW:
            {
                cerr << "Stack Overflow" << endl;
                break;
            }
            case GL_STACK_UNDERFLOW:
            {
                cerr << "Stack Underflow" << endl;
                break;
            }
            case GL_OUT_OF_MEMORY:
            {
                cerr << "Out of memory." << endl;
                break;
            }
            case GL_TABLE_TOO_LARGE:
            {
                cerr << "Table too large." << endl;
                break;
            }
        }

        error = glGetError();
    }
}

bool DisplayEngine::loadTexture(Surface inSurface, GLuint inTexture)
{
    /*
    cerr << "Pre-texture errors:\n";
    printErrors();
*/
    bool outSuccess = true;
    if (inSurface == NULL)
    {
        Uint32 flags = SDL_SWSURFACE | SDL_ASYNCBLIT;
        int bits = 0;

        Surface t;
        t = SDL_CreateRGBSurface(flags, 1, 1, bits, _mask.red, _mask.green,
            _mask.blue, _mask.alpha);
        inSurface = SDL_DisplayFormat(t);
        SDL_FreeSurface(t);
        outSuccess = false;
    }

    glBindTexture(GL_TEXTURE_2D, inTexture);

    if (_mipmapping)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
    }

    if (_mipmapping)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



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

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, inSurface->w, inSurface->h,
        0, tFormat, GL_UNSIGNED_BYTE, inSurface->pixels);

/*
    cerr << "Post-texture errors:\n";
    printErrors();
*/

    if (!outSuccess) SDL_FreeSurface(inSurface);
    return outSuccess;
}

bool DisplayEngine::loadTexture(const char* inFile, GLuint inTexture)
{
    Surface t = loadImage(inFile);
    if (t == NULL) return false;
    loadTexture(t, inTexture);
    SDL_FreeSurface(t);
    return true;
}

void DisplayEngine::logOpenGL(ostream& inStream)
{
    inStream << "Vendor: " << (char*)glGetString(GL_VENDOR) << endl;
    inStream << "Renderer: " << (char*)glGetString(GL_RENDERER) << endl;
    inStream << "OpenGL Version: " << (char*)glGetString(GL_VERSION) << endl;
    string stuff = (char*)glGetString(GL_EXTENSIONS);
    inStream << "\nString size: " << stuff.size() << endl << endl;
    for (unsigned int i = 0; i < stuff.size(); ++i)
    {
        unsigned int j = stuff.find_first_of(' ', i);
        inStream << stuff.substr(i, j - i) << endl;

        if (j > i) i = j;
    }
}

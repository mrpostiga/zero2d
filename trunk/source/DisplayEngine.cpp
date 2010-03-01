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
#include "LogFile.h"

#include <SDL_image.h>
#include <SDL_ttf.h>

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

    unsigned nextPulse = SDL_GetTicks() + FRAME_LENGTH;
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

        currentModule->onOpen();

        unsigned int nextSecond = SDL_GetTicks() + 1000u;
        int framesPerSecond = 0;

        while (currentModule->isRunning())
        {
            while (SDL_PollEvent(&event)) currentModule->onEvent(&event);

            currentModule->onRender();
            SDL_GL_SwapBuffers();
            ++framesPerSecond;

            unsigned int ticks = SDL_GetTicks();

            if (ticks > nextSecond)
            {
                //cout << framesPerSecond << " frames per second" << endl;
                nextSecond += 1000u;
                framesPerSecond = 0;
            }

            if (ticks > nextPulse)
            {
                currentModule->onPulse();
                nextPulse += FRAME_LENGTH;
            }

            SDL_Delay(1); // prevent CPU abuse
        }

        currentModule->onClose();

        Module* oldModule = currentModule;
        currentModule = currentModule->next();

        if (oldModule->isDead())
        {
            oldModule->onUnload();
            delete oldModule;
        }
        else
        {
            moduleStack.push_back(oldModule);
            oldModule = NULL;
        }
    }

    cleanup();
}

void DisplayEngine::initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cerr << "-- error on SDL_Init --" << endl;
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        cerr << "-- error on TTF_Init -- " << TTF_GetError() << endl;
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    SDL_WM_SetCaption("Zero2D version 0.0.1", "Zero2D");

    LogFile oglLog("ogl");
    logOpenGL(oglLog);
}

void DisplayEngine::cleanup()
{
    #ifndef __APPLE__
    SDL_FreeSurface(_windowIcon);
    #endif

    TTF_Quit();
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

void DisplayEngine::logErrors(ostream& inStream)
{
    GLenum error;

    error = glGetError();

    if (error != GL_NO_ERROR)
    {
        inStream << "OpenGL Error: ";
    }

    while (error != GL_NO_ERROR)
    {
        switch(error)
        {
            case GL_INVALID_ENUM:
            {
                inStream << "invalid enum";
                break;
            }
            case GL_INVALID_VALUE:
            {
                inStream << "invalid value";
                break;
            }
            case GL_INVALID_OPERATION:
            {
                inStream << "invalid operation";
                break;
            }
            case GL_STACK_OVERFLOW:
            {
                inStream << "stack overflow";
                break;
            }
            case GL_STACK_UNDERFLOW:
            {
                inStream << "stack underflow";
                break;
            }
            case GL_OUT_OF_MEMORY:
            {
                inStream << "out of memory";
                break;
            }
            case GL_TABLE_TOO_LARGE:
            {
                inStream << "table too large";
                break;
            }
            default:
            {
                break;
            }
        }

        inStream << endl;

        error = glGetError();
    }
}

bool DisplayEngine::loadTexture(Surface inSurface, GLuint inTexture,
    bool inDelete)
{
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
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
        cerr << "failed to load texture -- not true color -- " << nOfColors
            << endl;
        SDL_FreeSurface(inSurface);
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, inSurface->w, inSurface->h,
        0, tFormat, GL_UNSIGNED_BYTE, inSurface->pixels);

    if (inDelete || !outSuccess) SDL_FreeSurface(inSurface);
    return outSuccess;
}

bool DisplayEngine::loadTexture(const char* inFile, GLuint inTexture)
{
    Surface t = loadImage(inFile);
    return loadTexture(t, inTexture, true);
}

void DisplayEngine::logOpenGL(ostream& inStream)
{
    inStream << "Vendor: " << (char*)glGetString(GL_VENDOR) << endl;
    inStream << "Renderer: " << (char*)glGetString(GL_RENDERER) << endl;
    inStream << "OpenGL Version: " << (char*)glGetString(GL_VERSION) << endl;
    string stuff((char*)glGetString(GL_EXTENSIONS));
    inStream << "\nString size: " << stuff.size() << endl << endl;
    for (unsigned int i = 0; i < stuff.length(); ++i)
    {
        unsigned int j = stuff.find_first_of(' ', i);
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
}

void DisplayEngine::ortho(double inRange)
{
    int width = _display->w;
    int height = _display->h;

    double aspect = double(width) / double(height);

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width < height)
    {
        glOrtho(-inRange, inRange, -inRange / aspect, inRange / aspect,
            -inRange, inRange);
    }
    else
    {
        glOrtho(-inRange * aspect, inRange * aspect, -inRange, inRange,
            -inRange, inRange);
    }

    glMatrixMode(GL_MODELVIEW);
}

int DisplayEngine::processKey(SDLKey inSym, SDLMod inMod)
{
    int c = inSym;
    if (inMod & (KMOD_LSHIFT | KMOD_RSHIFT))
    {
        if (c >= 'a' && c <= 'z')
        {
            c -= 32;
        }
        else
        {
            switch (inSym)
            {
                case SDLK_1: c = '!'; break;
                case SDLK_2: c = '@'; break;
                case SDLK_3: c = '#'; break;
                case SDLK_4: c = '$'; break;
                case SDLK_5: c = '%'; break;
                case SDLK_6: c = '^'; break;
                case SDLK_7: c = '&'; break;
                case SDLK_8: c = '*'; break;
                case SDLK_9: c = '('; break;
                case SDLK_0: c = ')'; break;

                case SDLK_LEFTBRACKET: c = '{'; break;
                case SDLK_RIGHTBRACKET: c = '}'; break;
                case SDLK_SEMICOLON: c = ':'; break;
                case SDLK_QUOTE: c = '"'; break;
                case SDLK_COMMA: c = '<'; break;
                case SDLK_PERIOD: c = '>'; break;
                case SDLK_SLASH: c = '?'; break;
                case SDLK_MINUS: c = '_'; break;
                case SDLK_EQUALS: c = '+'; break;
                case SDLK_BACKQUOTE: c = '~'; break;
                case SDLK_BACKSLASH: c = '|'; break;
                case SDLK_SPACE: c = ' '; break;

                default:
                {
                    c = 0;
                }
            }
        }
    }
    else if (c < 273 && (c < 160 || c > 255))
    {
        switch (inSym)
        {
            case SDLK_KP_PLUS: c = '+'; break;
            case SDLK_KP_DIVIDE: c = '/'; break;
            case SDLK_KP_MULTIPLY: c = '*'; break;
            case SDLK_KP_MINUS: c = '-'; break;
            case SDLK_KP_PERIOD: c = '.'; break;

            case SDLK_KP1:
            case SDLK_KP2:
            case SDLK_KP3:
            case SDLK_KP4:
            case SDLK_KP5:
            case SDLK_KP6:
            case SDLK_KP7:
            case SDLK_KP8:
            case SDLK_KP9:
            case SDLK_KP0:
            {
                c -= 208;
                break;
            }

            case SDLK_PAUSE:
            case SDLK_DELETE:
            case SDLK_BACKSPACE:
            case SDLK_RETURN:
            case SDLK_TAB:
            {
                // the blacklisted keys :)
                c = 0;
                break;
            }

            default:
            {
            }
        }
    }
    else
    {
        c = 0;
    }

    return c;
}

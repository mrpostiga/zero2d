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

#include "GameEngine.h"
#include "DisplayEngine.h"
#include "SoundEngine.h"
#include "TestModule.h"
#include "TextureModule.h"

#include <boost/filesystem.hpp>

#include <fstream>
using namespace std;

PropertyList GameEngine::config;

#ifndef __WIN32__
string GameEngine::mUserFolder(UNIX_HOME_FOLDER);
#else
string GameEngine::mUserFolder("data/");
#endif

void GameEngine::initialize(int inArgc, char** inArgv)
{
#ifndef __WIN32__
    {
        // UNIX home folder settings file
        using namespace boost::filesystem;
        string s(UNIX_HOME_FOLDER);
        s += "/.cyborus";
        create_directory(s.c_str());
        s += "/zero2d";
        create_directory(s.c_str());
        s += "/logs";
        create_directory(s.c_str());
        mUserFolder += "/.cyborus/zero2d/";
    }
#endif

    bool createSettingsFile = false;

    string settingsFile(mUserFolder);
    settingsFile += "settings.txt";
    const char* f = inArgc > 1 ? inArgv[1] : settingsFile.c_str();
    ifstream fin(f, ifstream::in);
    if (fin.fail())
    {
        cerr << "failed to open config file: " << f << endl;
        createSettingsFile = true;
    }
    else
    {
        fin >> config;
        fin.close();
    }

    DisplayEngine::initialize();
    //SoundEngine::initialize();

    Module* m;

    try
    {
        m = new TextureModule;
        //m = new TestModule;
    }
    catch (const CoreException& ce)
    {
        cerr << ce << endl;
        m = NULL;
    }
    catch (...)
    {
        cerr << "unknown exception" << endl;
        m = NULL;
    }

    if (m) DisplayEngine::start(m);
    //SoundEngine::cleanup();

    if (createSettingsFile)
    {
        string s(getUserFolder());
        s += "settings.txt";
        ofstream fout(s.c_str(), ofstream::out);
        if (fout.fail()) return;
        fout << config;
        fout.close();
    }
}

int GameEngine::processKey(SDLKey inSym, SDLMod inMod)
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


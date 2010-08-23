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

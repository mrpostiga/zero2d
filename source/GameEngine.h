#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "PropertyList.h"

#include <cstdlib>
#define UNIX_HOME_FOLDER getenv("HOME")

class GameEngine
{
    public:
        static void initialize(int inArgc, char** inArgv);

        static inline const std::string& getUserFolder()
        {
            return mUserFolder;
        }

        static PropertyList config;

    private:
        static std::string mUserFolder;
};

#endif

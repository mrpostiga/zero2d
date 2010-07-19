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

#include "Config.h"
#include "DisplayEngine.h"
#include "SoundEngine.h"
#include "TestModule.h"
#include "TextureModule.h"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    Config::initialize(argc, argv);
    DisplayEngine::initialize();
    //SoundEngine::initialize();
    //Config::outputSettings();

    Module* m;

    try
    {
        m = new TextureModule;
        //m = new TestModule;
    }
    catch (const Shader::Exception& se)
    {
        cerr << "shader exception -- " << se.reason << endl;
        m = NULL;
    }
    catch (...)
    {
        cerr << "unknown exception" << endl;
        m = NULL;
    }

    if (m) DisplayEngine::start(m);
    //SoundEngine::cleanup();
    Config::finalize();
    return 0;
}

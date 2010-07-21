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

#include "TestModule.h"
#include "Config.h"
#include "DisplayEngine.h"

#include <ctime>
#include <iostream>
using namespace std;

TestModule::TestModule()
{
}

TestModule::~TestModule()
{
}

void TestModule::onLoad()
{
    mTimer = 0;
    mNet.listen(Config::get<Uint16>("connect port", 45678));
    mNet.connect(Config::getRaw("connect ip"),
        Config::get<Uint16>("connect port", 45678));
}

void TestModule::onUnload()
{
}

void TestModule::onOpen()
{
}

void TestModule::onClose()
{
}

void TestModule::onLoop()
{
    Uint8 buffer[PACKET_SIZE];
    if (mNet.receiveData(buffer))
    {
        //Uint16 somePort = mNet.getPacket()->address.port;
        //SDL_Swap16(somePort);
        /*
        if (somePort != mPort)
        {
            mPort = somePort;
            cerr << "new port: " << mPort << endl;
            mNet.connect("75.174.77.210", mPort);
        }
        */
        cout << "\nreceived: " << buffer << endl;
    }
}

void TestModule::onFrame()
{
    ++mTimer;
    if (mTimer > 60)
    {
        mTimer = 0;
        mNet.sendData("WE ARE AWESOME!!!", 18);
        cout << '.';
        cout.flush();
    }
}

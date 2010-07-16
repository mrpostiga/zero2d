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

bool TestModule::onLoad()
{
    mTimer = 0;
    mNet.listen(Config::get<Uint16>("connect port", 45678));
    mNet.connect(Config::getRaw("connect ip"),
        Config::get<Uint16>("connect port", 45678));
    return true;
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
        cout << "\nreceived: " << buffer << endl;
    }
}

void TestModule::onFrame()
{
    ++mTimer;
    if (mTimer > 60)
    {
        mTimer = 0;
        mNet.sendData("TheBuzzSaw", 11);
        cout << '.';
        cout.flush();
    }
}

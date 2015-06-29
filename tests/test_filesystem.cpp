#include "tests.hpp"

#include <core/util/Log.h>
#include <core/system/file/FileWatcher.h>
#include <core/system/thread/Thread.h>
#include <core/system/gui/Keyboard.h>

void test_fileWatcher()
{
    using namespace sn;

    SN_LOG("Main thread");

    //Thread testThread([](){
    //    for (u32 i = 0; i < 10; ++i)
    //    {
    //        SN_LOG("Test thread " << i);
    //        Thread::sleep(Time::milliseconds(10));
    //    }
    //});

    //testThread.start();

    //SN_LOG("Continue main thread ");

    //while (true)
    //{
    //    Thread::sleep(Time::milliseconds(10));
    //}

    // TODO Use argv[] to get the root?
    std::string root = "D:/PROJETS/INFO/CPP/SnowfeetFramework/SnowfeetEngine/SnowfeetEngine/";

    FileWatcher watcher;
    watcher.setPath(root + "tests/test_data");
    watcher.setEnabled(true);

    bool continueWatching = true;
    while (continueWatching && watcher.isEnabled())
    {
        FileWatcher::Event ev;
        while (watcher.popEvent(ev))
        {
            SN_LOG("File change " << ev.type << " at " << ev.path);
        }

        if (isKeyPressed(SN_KEY_ESCAPE))
        {
            continueWatching = false;
        }

        Thread::sleep(Time::milliseconds(10));
    }
}



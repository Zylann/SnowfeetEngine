#include "NativeHello.hpp"
#include <core/scene/Scene.h>

namespace hello
{

void NativeHello::onReady()
{
    // Print a message
    SN_LOG("Hello from C++!");

    // Quit the application (otherwise it loops indefinitely)
    auto scene = getScene();
    scene->quit();
}

} // namespace hello


#include "FileWatcher.hpp"

// http://stackoverflow.com/questions/931093/how-do-i-make-my-program-watch-for-file-modification-in-c

namespace sn
{

FileWatcher::FileWatcher():
    m_enabled(true),
    m_isRecursive(false),
    m_impl(nullptr)
{
}

FileWatcher::~FileWatcher()
{
    destroy();
}

void FileWatcher::setPath(const std::string & path)
{
    m_watchedPath = path;

    // If a session is already running
    if (m_impl)
    {
        // Re-create the session
        create();
    }
}

void FileWatcher::setEnabled(bool e)
{
    m_enabled = e;
    if (m_enabled)
        create();
    else
        destroy();
}

bool FileWatcher::popEvent(Event & event)
{
    Lock lock(m_eventsMutex);
    if (!m_events.empty())
    {
        event = m_events.back();
        m_events.pop();
        return true;
    }
    else
    {
        return false;
    }
}

void FileWatcher::pushEvent(Event event)
{
    Lock lock(m_eventsMutex);
    m_events.push(event);
}

void FileWatcher::create()
{
    destroy();
    createImpl();
}

void FileWatcher::destroy()
{
    if (m_impl)
    {
        destroyImpl();
        m_impl = nullptr;
    }
}

} // namespace sn


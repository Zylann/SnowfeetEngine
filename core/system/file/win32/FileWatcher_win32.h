#ifndef __HEADER_SN_FILEWATCHER_WIN32__
#define __HEADER_SN_FILEWATCHER_WIN32__

#include <core/system/file/FileWatcher.h>
#include <core/system/thread/Thread.h>

#include <Windows.h>

namespace sn
{

/// \cond INTERNAL

/// \brief Represents a watching session over a defined path.
/// Must be re-created to change.
class FileWatcherImpl
{
public:
    FileWatcherImpl(FileWatcher & owner);
    ~FileWatcherImpl();

    bool isActive() const;

private:
    void startWatching(std::string pathStr, bool recursive);

private:
    FileWatcher & r_owner;
    Thread * m_watcherThread;
    std::string m_oldFileName; // There are two messages for renaming, so we need to memorize the first one
    bool m_continueWatching;
    bool m_isActive;
};

/// \endcond

} // namespace sn

#endif // __HEADER_SN_FILEWATCHER_WIN32__


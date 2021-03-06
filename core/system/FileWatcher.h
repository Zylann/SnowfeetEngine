#ifndef __HEADER_SN_FILEWATCHER__
#define __HEADER_SN_FILEWATCHER__

#include <string>
#include <queue>
#include <core/system/Lock.h>
#include <core/system/Clock.h>
#include <core/util/NonCopyable.h>

namespace sn
{

class FileWatcherImpl;

/// \brief Wraps filesystem notifications under a specified path.
/// It can detect file changes under the path like addition, deletion or modification.
/// Events are asynchronous and can be retrieved through a queue.
class SN_API FileWatcher : public NonCopyable
{
public:

    enum EventType
    {
        FILE_ADDED,
        FILE_MODIFIED,
        FILE_REMOVED,
        FILE_RENAMED,
        FILE_UNKNOWN_CHANGE
    };

    struct Event
    {
        /// \brief File event type
        EventType type;
        /// \brief Path to the concerned file, relative to the watched directory
        std::string path;
        /// \brief New path of the concerned file if it changed
        std::string newPath;
        /// \brief Is the concerned file node a directory?
        bool isDirectory;

        Event() :
            type(FILE_UNKNOWN_CHANGE),
            isDirectory(false)
        {}

        bool operator==(const Event & other)
        {
            return type == other.type
                && path == other.path
                && newPath == other.newPath
                && isDirectory == isDirectory;
        }
    };

    /// \brief Just creates an inactive, uninitialized FileWatcher.
    FileWatcher();

    /// \brief Stops and destroys the watcher
    ~FileWatcher();

    /// \brief Sets the path to be watched. The path has to be absolute.
    void setPath(const std::string & path);

    /// \brief Gets the path to be watched
    const std::string & getPath() const { return m_watchedPath; }

    /// \brief Enables or disables watching.
    /// \note If no path is set yet, you'll get no results until you set one.
    void setEnabled(bool e);

    /// \brief Tells if the watcher is active or not
    bool isEnabled() const { return m_enabled; }

    /// \brief Sets wether to watch changes in sub-directories or not
    void setRecursive(bool r);

    /// \brief Tells if sub-directories are watched or not
    bool isRecursive() const { return m_isRecursive; }

    /// \brief Pops a file system change event.
    /// \return true if an event has been popped, false if there is no event
    bool popEvent(Event & event);

    /// \brief Pushes a file system event to the internal queue.
    /// \warning This method is reserved for internal use.
    void pushEvent(Event event);

    void setFilterDuplicateEvents(bool enable);
    void setDuplicateEventsFilterTimeThreshold(Time threshold);

private:
    void create();
    void destroy();

    // Platform-specific
    void createImpl();
    void destroyImpl();

private:
    bool m_enabled;
    bool m_isRecursive;
    bool m_filterDuplicateEvents;
    Time m_duplicateEventTimeThreshold;
    std::string m_watchedPath;
    std::queue<Event> m_events;
    Mutex m_eventsMutex;
    FileWatcherImpl * m_impl;
    Clock m_lastEventTime;

};

} // namespace sn

#endif // __HEADER_SN_FILEWATCHER__


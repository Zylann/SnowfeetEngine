#include <core/util/Log.h>
#include <core/system/FilePath.h>
#include "FileWatcher_win32.h"
#include "../win32/helpers_win32.h"

// Based on this code
// https://developersarea.wordpress.com/2014/09/26/win32-file-watcher-api-to-monitor-directory-changes/

namespace sn
{

//=============================================================================
// Helpers
//=============================================================================
    
template <typename T>
class AutoArrayDeleter
{
public:
    AutoArrayDeleter(T * a): m_aptr(a) {}
    ~AutoArrayDeleter() { if(m_aptr) delete[] m_aptr; }
private:
    T * m_aptr;
};

//=============================================================================
// FileWatcher
//=============================================================================

void FileWatcher::createImpl()
{
    m_impl = new FileWatcherImpl(*this);
}

void FileWatcher::destroyImpl()
{
    delete m_impl;
}

//=============================================================================
// Impl
//=============================================================================

FileWatcherImpl::FileWatcherImpl(FileWatcher & owner):
    r_owner(owner),
    m_continueWatching(true)
{
    m_watcherThread = new Thread([this](){
        startWatching(r_owner.getPath(), r_owner.isRecursive());
    });
    m_watcherThread->start();
}

FileWatcherImpl::~FileWatcherImpl()
{
    if (m_watcherThread)
    {
        m_continueWatching = false;
        delete m_watcherThread;
    }
}

bool FileWatcherImpl::isActive() const
{
    return m_isActive;
}

void FileWatcherImpl::startWatching(std::string pathStr, bool recursive)
{
    SN_LOG("FileWatcher thread begin at '" << pathStr << "'");

    // Convert input path into the win32 format
    pathStr = FilePath::platformize(pathStr);
    LPTSTR path = new TCHAR[pathStr.size() + 1];
#ifdef UNICODE
    std::wstring wpathStr = sn::toWideString(pathStr);
    _tcscpy(path, wpathStr.c_str());
#else
    strcpy_s(path, pathStr.size()+1, pathStr.c_str());
    //path[pathStr.size()] = '\0';
#endif
    AutoArrayDeleter<TCHAR> pathDeleter(path);

    // Open the directory
    HANDLE hDir = ::CreateFile (path, 
        GENERIC_READ | FILE_LIST_DIRECTORY, 
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL, 
        OPEN_EXISTING, 
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE)
    {
        SN_ERROR("An error occurred while creating a FileWatcher handle: " << win32::getLastError());
        return;
    }
    if (hDir == nullptr)
    {
        SN_ERROR("Unexpected NULL from CreateFile: " << win32::getLastError());
        return;
    }

    // Which changes we want to be notified for
    DWORD dwNotifyFilter = 
          FILE_NOTIFY_CHANGE_FILE_NAME
        | FILE_NOTIFY_CHANGE_DIR_NAME
        | FILE_NOTIFY_CHANGE_SIZE
        | FILE_NOTIFY_CHANGE_LAST_WRITE
        | FILE_NOTIFY_CHANGE_LAST_ACCESS
        | FILE_NOTIFY_CHANGE_CREATION
    ;

    char resultsBuffer[2048] = { 0 };

    // Used for asynchronous I/O
    OVERLAPPED pollingOverlap;
    pollingOverlap.OffsetHigh = 0;
    pollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    DWORD waitTimeoutMilliseconds = 100;

    while (m_continueWatching)
    {
        DWORD nRet = 0;
        BOOL result = ::ReadDirectoryChangesW(
            hDir, // handle to the directory to be watched
            &resultsBuffer, // pointer to the buffer to receive the read results
            sizeof(resultsBuffer), // length of resultsBuffer
            recursive ? TRUE : FALSE, // flag for monitoring directory or directory tree
            dwNotifyFilter,
            &nRet, // number of bytes returned
            &pollingOverlap,// pointer to structure needed for overlapped I/O
            NULL
        );

        if (result == 0)
        {
            SN_ERROR("Failed to ReadDirectoryChangesW: " << win32::getLastError());
            break;
        }

        // Wait for events
        DWORD waitResult = ::WaitForSingleObject(pollingOverlap.hEvent, waitTimeoutMilliseconds);

        if (waitResult == WAIT_TIMEOUT)
        {
            continue;
        }

        if (waitResult == WAIT_FAILED)
        {
            // An error occurred
            SN_ERROR("FileWatcher WaitForSingleObject failed: " << win32::getLastError());
            break;
        }

        if (waitResult != WAIT_OBJECT_0)
        {
            continue;
        }

        int offset = 0;
        FILE_NOTIFY_INFORMATION * pNotification = nullptr;

        // Iterate over notifications
        do
        {
            pNotification = (FILE_NOTIFY_INFORMATION*)((char*)resultsBuffer + offset);

            char filename[MAX_PATH] = { 0 };
            //strcpy(filename, "");

            // Note: the filename is relative to the watched directory
            int filenameLength = WideCharToMultiByte(
                CP_ACP, 
                0, 
                pNotification->FileName, 
                pNotification->FileNameLength/2,
                filename,
                sizeof(filename),
                NULL,
                NULL
            );

            filename[pNotification->FileNameLength/2] = '\0';

            FileWatcher::Event ev;

            switch(pNotification->Action)
            {
            case FILE_ACTION_ADDED:
                ev.type = FileWatcher::FILE_ADDED;
                ev.path = filename;
                r_owner.pushEvent(ev);
                //SN_LOG("The file is added to the directory: " << filename);
                break;

            case FILE_ACTION_REMOVED:
                ev.type = FileWatcher::FILE_REMOVED;
                ev.path = filename;
                r_owner.pushEvent(ev);
                //SN_LOG("The file is removed from the directory: " << filename);
                break;

            case FILE_ACTION_MODIFIED:
                ev.type = FileWatcher::FILE_MODIFIED;
                ev.path = filename;
                r_owner.pushEvent(ev);
                //SN_LOG("The file is modified. This can be a change in the time stamp or attributes: " << filename);
                break;

            case FILE_ACTION_RENAMED_OLD_NAME:
                m_oldFileName = filename;
                //SN_LOG("The file was renamed and this is the old name: " << filename);
                break;

            case FILE_ACTION_RENAMED_NEW_NAME:
                ev.type = FileWatcher::FILE_RENAMED;
                ev.path = m_oldFileName;
                ev.newPath = filename;
                r_owner.pushEvent(ev);
                //SN_LOG("The file was renamed and this is the new name: " << filename);
                break;

            default:
                SN_ERROR("Unknown file notification (" << pNotification->Action << ')');
                break;
            }

            offset += pNotification->NextEntryOffset;
       
        } while(pNotification->NextEntryOffset); //(offset != 0);

    } // while m_continueWatching

    CloseHandle(hDir);

    SN_LOG("FileWatcher thread end");
}

} // namespace sn


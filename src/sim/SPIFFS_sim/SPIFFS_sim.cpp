#include "SPIFFS.h"

#include "vfs.h"

#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


const char *spiffs_storage_dir = ".";

static std::string storage_dir_path;

SPIFFSFS::SPIFFSFS(FSImplPtr impl) :
    FS(impl)
{
}

static bool path_exists(const char *path)
{
    struct stat sb;
    return stat(path, &sb) == 0;
}

static bool dir_exists(const char *path)
{
    struct stat sb;
    int ret = stat(path, &sb);
    if (ret != 0 || (sb.st_mode & S_IFMT) != S_IFDIR)
    {
        return false;
    }

    return true;
}

bool SPIFFSFS::begin(bool formatOnFail, const char * basePath, uint8_t maxOpenFiles)
{
    // Make sure the "mount point" directory exits
    storage_dir_path = spiffs_storage_dir;
    if (basePath[0] != '/')
    {
        storage_dir_path += "/";
    }
    storage_dir_path += basePath;
    _impl->mountpoint(storage_dir_path.c_str());
    if (!dir_exists(storage_dir_path.c_str()))
    {
        if (!formatOnFail)
        {
            _impl->mountpoint(NULL);
            return false;
        }

        if (!format())
        {
            _impl->mountpoint(NULL);
            return false;
        }
    }

    return true;
}

bool SPIFFSFS::format()
{
    // Make sure no file or directory with the name of the mount point already exists.
    if (path_exists(_impl->mountpoint()))
    {
        if (dir_exists(_impl->mountpoint()))
        {
            if (rmdir(_impl->mountpoint()) != 0)
            {
                return false;
            }
        }
        else
        {
            if (unlink(_impl->mountpoint()) != 0)
            {
                return false;
            }
        }
    }

    return ::mkdir(_impl->mountpoint(), S_IRWXU) == 0;
}

size_t SPIFFSFS::totalBytes()
{
    return 0;
}

size_t SPIFFSFS::usedBytes()
{
    return 0;
}

void SPIFFSFS::end()
{
}

bool SPIFFSFS::exists(const char* path)
{
    return false;
}

bool SPIFFSFS::exists(const String& path)
{
    return false;
}


SPIFFSFS SPIFFS = SPIFFSFS(FSImplPtr(new VFSImpl()));

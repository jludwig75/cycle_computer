#include "SPIFFS.h"

#include "vfs.h"


SPIFFSFS::SPIFFSFS(FSImplPtr impl) :
    FS(impl)
{
}

bool SPIFFSFS::begin(bool formatOnFail, const char * basePath, uint8_t maxOpenFiles)
{
    return false;
}

bool SPIFFSFS::format()
{
    return false;
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

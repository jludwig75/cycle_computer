#include "vfs.h"


FileImplPtr VFSImpl::open(const char* path, const char* mode)
{
    return FileImplPtr();
}

bool        VFSImpl::exists(const char* path)
{
    return false;
}

bool        VFSImpl::rename(const char* pathFrom, const char* pathTo)
{
    return false;
}

bool        VFSImpl::remove(const char* path)
{
    return false;
}

bool        VFSImpl::mkdir(const char *path)
{
    return false;
}

bool        VFSImpl::rmdir(const char *path)
{
    return false;
}



    VFSFileImpl::VFSFileImpl(VFSImpl* fs, const char* path, const char* mode)
    {
    }

    VFSFileImpl::~VFSFileImpl()
    {
    }

    size_t      VFSFileImpl::write(const uint8_t *buf, size_t size)
    {
        return 0;
    }

    size_t      VFSFileImpl::read(uint8_t* buf, size_t size)
    {
        return 0;
    }

    void        VFSFileImpl::flush()
    {
    }

    bool        VFSFileImpl::seek(uint32_t pos, SeekMode mode)
    {
        return false;
    }

    size_t      VFSFileImpl::position() const
    {
        return 0;
    }

    size_t      VFSFileImpl::size() const
    {
        return 0;
    }

    void        VFSFileImpl::close()
    {
    }

    const char* VFSFileImpl::name() const
    {
        return NULL;
    }

    time_t VFSFileImpl::getLastWrite()
    {
        return 0;
    }

    boolean     VFSFileImpl::isDirectory(void)
    {
        return false;
    }

    FileImplPtr VFSFileImpl::openNextFile(const char* mode)
    {
        return FileImplPtr();
    }

    void        VFSFileImpl::rewindDirectory(void)
    {
    }

    VFSFileImpl::operator    bool()
    {
        return false;
    }


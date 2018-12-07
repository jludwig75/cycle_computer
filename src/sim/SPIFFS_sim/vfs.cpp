#include "vfs.h"

#include <stdio.h>
#include <string>


FileImplPtr VFSImpl::open(const char* path, const char* mode)
{
    if(!path || path[0] != '/') {
        fprintf(stderr, "%s does not start with /", path);
        return FileImplPtr();
    }

    std::string file_name = std::string("spiffs_sim_root") + path;

    FileImplPtr file = std::make_shared<VFSFileImpl>(this, file_name.c_str(), mode);
    if (!file)
    {
        return FileImplPtr();
    }

    return file;
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



    VFSFileImpl::VFSFileImpl(VFSImpl* fs, const char* path, const char* mode) :
        _fs(fs),
        _f(NULL)
    {
        _f = fopen(path, mode);
        if (!_f)
        {
            return;
        }
    }

    VFSFileImpl::~VFSFileImpl()
    {
        close();
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
        if (_f)
        {
            fclose(_f);
            _f = NULL;
        }

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


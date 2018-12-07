#include "vfs.h"

#include <stdio.h>
#include <string>
#include <string.h>


FileImplPtr VFSImpl::open(const char* path, const char* mode)
{
    if (mountpoint() == NULL)
    {
        fprintf(stderr, "File system not mounted\n");
        return FileImplPtr();
    }

    if(!path || path[0] != '/') {
        fprintf(stderr, "\"%s\" does not start with \"/\"\n", path);
        return FileImplPtr();
    }

    FileImplPtr file = std::make_shared<VFSFileImpl>(this, path, mode);
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
    _f(NULL),
    _d(NULL),
    _path(NULL),
    _isDirectory(false),
    _written(false)
{
    char * temp = (char *)malloc(strlen(path)+strlen(_fs->_mountpoint)+1);
    if(!temp) {
        return;
    }
    sprintf(temp,"%s%s", _fs->_mountpoint, path);

    _path = strdup(path);
    if(!_path) {
        fprintf(stderr, "strdup(%s) failed", path);
        free(temp);
        return;
    }

    if(!stat(temp, &_stat)) {
        //file found
        if (S_ISREG(_stat.st_mode)) {
            _isDirectory = false;
            _f = fopen(temp, mode);
            if(!_f) {
                fprintf(stderr, "fopen(%s) failed", temp);
            }
        } else if(S_ISDIR(_stat.st_mode)) {
            _isDirectory = true;
            _d = opendir(temp);
            if(!_d) {
                fprintf(stderr, "opendir(%s) failed", temp);
            }
        } else {
            fprintf(stderr, "Unknown type 0x%08X for file %s", ((_stat.st_mode)&S_IFMT), temp);
        }
    } else {
        //file not found
        if(!mode || mode[0] == 'r') {
            //try to open as directory
            _d = opendir(temp);
            if(_d) {
                _isDirectory = true;
            } else {
                _isDirectory = false;
                //log_w("stat(%s) failed", temp);
            }
        } else {
            //lets create this new file
            _isDirectory = false;
            _f = fopen(temp, mode);
            if(!_f) {
                fprintf(stderr, "fopen(%s) failed", temp);
            }
        }
    }
    free(temp);
}

VFSFileImpl::~VFSFileImpl()
{
    close();
}

size_t      VFSFileImpl::write(const uint8_t *buf, size_t size)
{
    if(_isDirectory || !_f || !buf || !size) {
        return 0;
    }
    _written = true;
    return fwrite(buf, 1, size, _f);
}

size_t      VFSFileImpl::read(uint8_t* buf, size_t size)
{
    if(_isDirectory || !_f || !buf || !size) {
        return 0;
    }

    return fread(buf, 1, size, _f);
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


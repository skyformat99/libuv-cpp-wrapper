#if !defined(UVPLUSPLUS_BUF_HPP)
#define UVPLUSPLUS_BUF_HPP

#include <uv.h>
#include <cassert>
#include <string>
#include <memory>
#include <new>

namespace uv {

class buf
{
public:
    buf(uv_buf_t* buf):
        _buf(buf) {}

    buf(const std::string& buf)
    {
       _buf = new uv_buf_t;
       _buf->base = (char*) buf.c_str();
       _buf->len = buf.size();
    }

    const uv_buf_t *get() {return _buf;}
    unsigned int size(){return _buf->len;}
    const std::string tostring() 
    {
        return std::string(_buf->base, _buf->len); 
    }

protected:
    uv_buf_t *_buf;
};

}

#endif /* UVPLUSPLUS_HANDLE_HPP */

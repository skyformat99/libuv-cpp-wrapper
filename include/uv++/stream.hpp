#if !defined(UVPLUSPLUS_STREAM_HPP)
#define UVPLUSPLUS_STREAM_HPP

#include <uv.h>
#include <cassert>
#include <string>
#include <memory>
#include <new>
#include <functional>
#include <tuple>
#include <uv++/handle.hpp>
#include <uv++/buf.hpp>
#include <uv++/loop.hpp>

namespace uv {
namespace handle {

class stream : public handle 
{
public:
    typedef std::function<void(stream*, ssize_t, buf*)> read_cb;
    typedef std::function<void(int status)> write_cb;

    stream() : _handle(new uv_stream_t), _write_req(new uv_write_t) {}
    stream(uv_stream_t *s) : _handle(s), _write_req(new uv_write_t) {}

    uv_stream_t *handler() {return _handle;}

    void read_start(read_cb cb)
    {
        _on_read = cb;
        _handle->data = static_cast<void*>(this);
        int err = uv_read_start(_handle, 
        [](uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
            *buf = uv_buf_init((char*) malloc(suggested_size), suggested_size);
        }, 
        [](uv_stream_t *s, ssize_t nread, const uv_buf_t* buf) {
            auto handle = static_cast<uv::handle::stream*>(s->data);
            auto cb = handle->_on_read;
            cb(handle, nread, new uv::buf((uv_buf_t*) buf));
        });
        if (err != 0) throw std::runtime_error("ERROR READ");
    }

    void write(const std::string &buf, write_cb cb) 
    {
        _on_write = cb;
        _handle->data = static_cast<void*>(this);
        uv::buf buffer(buf);
        uv_write(_write_req, _handle, buffer.get(),
                 1, [](uv_write_t *r, int status) {
            auto handle = static_cast<uv::handle::stream*>(r->handle->data);
            auto cb = handle->_on_write;
            cb(status);
        });
    }

    void read_stop() 
    {
        uv_read_stop(_handle);
    }

protected:
    uv_stream_t *_handle;
    uv_write_t *_write_req;
    read_cb _on_read;
    write_cb _on_write;
};

}
}

#endif /* UVPLUSPLUS_HANDLE_HPP */

#if !defined(UVPLUSPLUS_HANDLE_HPP)
#define UVPLUSPLUS_HANDLE_HPP

#include <uv.h>
#include <functional>

namespace uv {
namespace handle {

class handle
{
public:
    typedef std::function<void(handle*)> close_cb;

    handle() : _handle(new uv_handle_t) {}
    uv_handle_t *handler() { return _handle; }
    
    void close(close_cb cb) {
        _on_close = cb;
        _handle->data = static_cast<void*>(this);
        uv_close(_handle, [](uv_handle_t *h) {
            auto handle = static_cast<uv::handle::handle*>(h->data);
            auto cb = handle->_on_close;
            cb(handle);
        });
    }

protected:
	uv_handle_t *_handle;
    close_cb _on_close;
};

}
}

#endif /* UVPLUSPLUS_HANDLE_HPP */

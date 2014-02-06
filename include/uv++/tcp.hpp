#if !defined(UVPLUSPLUS_TCP_HPP)
#define UVPLUSPLUS_TCP_HPP

#include <uv.h>
#include <cassert>
#include <string>
#include <memory>
#include <new>
#include <functional>
#include <uv++/stream.hpp>
#include <uv++/handle.hpp>
#include <uv++/loop.hpp>

namespace uv {
namespace handle {

class tcp : public handle
{
public:
    class ip4
    {
    protected:
        typedef union 
        { 
            struct sockaddr addr;
            struct sockaddr_in addr4;
            struct sockaddr_in6 addr6;
        } bind_type;

        bind_type _bind;
        std::string _address;
        unsigned short _port;

        virtual void setup() 
        {
            int err = uv_ip4_addr(_address.c_str(), _port, &_bind.addr4);
            if (err != 0) {
                throw std::runtime_error("Ip4 init");
            }
        }

    public:
        bind_type &bind() { return _bind; }
        ip4(const std::string &address, unsigned short port):
           _address(address), _port(port) { setup(); }
    };

    class ip6 : public ip4
    {
    protected:
        virtual void setup() 
        { 
            int err = uv_ip6_addr(_address.c_str(), _port, &_bind.addr6);
            if (err != 0) {
                throw std::runtime_error("Ip6 init");
            }
        }

    public:
        ip6(const std::string &address, unsigned short port):
            ip4(address, port) {} 
    };

    typedef std::function<void(tcp*, int status)> connection_cb;
    typedef std::function<void(tcp*)> close_cb;

    tcp(uv::loop *loop)
        : _handle(new uv_tcp_t)
    {
        ::uv_tcp_init(loop->handler(), _handle);        
    }

    stream *copy_stream() 
    {
        uv_stream_t *s = (uv_stream_t*) _handle;
        return new stream(s);
    }

    uv_tcp_t *handler() {return _handle;}

    void bind(ip4 &ipv, int flags = 0)
    {
        int err = ::uv_tcp_bind(_handle, &ipv.bind().addr, 0);
        if (err != 0) 
            throw std::runtime_error("ERROR BIND");
    }

    void listen(int backlog, connection_cb cb) 
    {
        _on_connection = cb;
        _handle->data = static_cast<void*>(this);
        int err = uv_listen((uv_stream_t*) _handle, backlog, 
        [](uv_stream_t *s, int status) {
            auto handle = static_cast<uv::handle::tcp*>(s->data);
            auto cb = handle->_on_connection;
            cb(handle, status);
        });
        if (err != 0) 
            throw std::runtime_error("ERROR LISTEN");
    }

    void accept(tcp *client)
    {
        int err  = uv_accept((uv_stream_t*) _handle, (uv_stream_t*) client->_handle);
        if (err != 0) 
            throw std::runtime_error("ERROR ACCEPT");
    }

    void close(close_cb cb) {
        _on_close = cb;
        _handle->data = static_cast<void*>(this);
        uv_close((uv_handle_t*) _handle, [](uv_handle_t *h) {
            auto handle = static_cast<uv::handle::tcp*>(h->data);   
            auto cb = handle->_on_close;
            cb(handle);
        });
    }

protected:
    uv_tcp_t *_handle;
    connection_cb _on_connection;
    close_cb _on_close;
};

}
}

#endif /* UVPLUSPLUS_TCP_HPP */

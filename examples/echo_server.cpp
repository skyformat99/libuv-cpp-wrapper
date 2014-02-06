#include <iostream>
#include <uv++.hpp>

int
main()
{
    uv::loop default_loop;
    uv::handle::tcp tcp(&default_loop);
    uv::handle::tcp::ip4 ip_addr("0.0.0.0", 12345);
    tcp.bind(ip_addr, 0);
    tcp.listen(128, [&default_loop](uv::handle::tcp *server, int status) {
        std::cout << "Connected new client: " << status << std::endl;
        uv::handle::tcp *client = new uv::handle::tcp(&default_loop);
        server->accept(client);

        uv::handle::stream *s = client->copy_stream();
        s->read_start([&default_loop](uv::handle::stream *stream, ssize_t nread, uv::buf *buf) {
            stream->write(buf->tostring(), [](int status) {
            });
        });
    });

    std::cout << "Echo server is listening." << std::endl;
	default_loop.run();
}

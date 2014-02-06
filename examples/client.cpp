#include <iostream>
#include <uv++.hpp>

int main()
{
    uv::loop default_loop;
    uv::handle::tcp tcp(&default_loop);
    uv::handle::tcp::ip4 ip_addr("127.0.0.1", 12345);
    tcp.connect(ip_addr, [&default_loop](uv::handle::tcp *client, int status) {
        uv::handle::stream *s = client->copy_stream();
        s->write("OIOI", [](int status) {

        });
    });

    std::cout << "Echo server is listening." << std::endl;
	default_loop.run();
}

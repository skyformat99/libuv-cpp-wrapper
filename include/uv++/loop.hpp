#if !defined(UVPLUSPLUS_LOOP_HPP)
#define UVPLUSPLUS_LOOP_HPP

#include <uv.h>
#include <cassert>
#include <string>
#include <memory>
#include <new>

namespace uv {

/**
 * Event loop wrapper.
 */
class loop
{
public:
    loop(uv_loop_t *l):
        _loop(l) {}

    loop() throw(std::bad_alloc)
        : _loop(::uv_loop_new())
    {
        if (!_loop) throw std::bad_alloc();
    }

    void run(uv_run_mode mode = UV_RUN_DEFAULT) throw()
    {
        assert(_loop && "Unable to run event loop.");
        ::uv_run(_loop, mode);
    }

	uv_loop_t *handler() { return _loop; }

protected:
	uv_loop_t *_loop;
};

}

#endif /* UVPLUSPLUS_LOOP_HPP */

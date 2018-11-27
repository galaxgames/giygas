#include <giygasutil/GameLoopRunner.hpp>
#include <ratio>
#include <chrono>
#include <thread>

#include <iostream>

using namespace giygas;
using namespace std;
using namespace chrono;

GameLoopRunner::GameLoopRunner(
    Context *context,
    GameLoopDelegate *updatable
) {
    _context = context;
    _updatable = updatable;
}

void GameLoopRunner::set_context(Context *context) {
    _context = context;
}

void GameLoopRunner::set_updatable(GameLoopDelegate *updatable) {
    _updatable = updatable;
}

void GameLoopRunner::run() {
    typedef duration<long long, ratio<1, 60>> Frames;
    typedef duration<float> FSeconds;
    typedef steady_clock clock_t;

    clock_t::time_point frame_start = clock_t::now();
    clock_t::time_point previous_frame_start = frame_start - duration_cast<clock_t::duration>(Frames(1));

    for (;;) {
        float elapsed_seconds = duration_cast<FSeconds>(frame_start - previous_frame_start).count();
        _context->update();
        _updatable->update_logic(elapsed_seconds);
        _updatable->update_graphics();
        if(_context->should_close() || _updatable->should_close()) {
            break;
        }

        // NOTE: Sleeping no longer necesary, as we are currently only using the VK_PRESENT_MODE_FIFO_KHR
        // present mode now, blocking on next image acquisition. This is currently the most reliable way
        // to maintain a consistent framerate, however it is probably not the best. But it's better than
        // the previous sleep code, as the system makes no promises to when we will get waken back up again. :/

        previous_frame_start = frame_start;
        frame_start = clock_t::now();
    }
}

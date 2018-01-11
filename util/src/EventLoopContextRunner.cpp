#include <giygasutil/EventLoopContextRunner.hpp>
#include <ratio>
#include <chrono>
#include <thread>

using namespace giygas;
using namespace std;
using namespace chrono;

EventLoopContextRunner::EventLoopContextRunner(
    EventLoopContext *context,
    EventLoopUpdatable *updatable
) {
    _context = context;
    _updatable = updatable;
}

void EventLoopContextRunner::set_context(EventLoopContext *context) {
    _context = context;
}

void EventLoopContextRunner::set_updatable(EventLoopUpdatable *updatable) {
    _updatable = updatable;
}

void EventLoopContextRunner::run() {
    typedef duration<long long, ratio<1, 60>> Frames;
    typedef duration<float> FSeconds;
    typedef high_resolution_clock clock_t;

    clock_t::time_point frame_start = clock_t::now();
    clock_t::time_point previous_frame_start = frame_start - duration_cast<clock_t::duration>(Frames(1));

    for (;;) {
        float elapsed_seconds = duration_cast<FSeconds>(frame_start - previous_frame_start).count();
        _updatable->update(elapsed_seconds);
        _context->update();

        if(_context->should_close()) {
            break;
        }

        // Figure out how much to sleep
        clock_t::time_point next_frame_start = frame_start + duration_cast<clock_t::duration>(Frames(1));
        clock_t::time_point current_time = clock_t::now();
        this_thread::sleep_for(next_frame_start - current_time);
        previous_frame_start = frame_start;
        frame_start = clock_t::now();
    }
}
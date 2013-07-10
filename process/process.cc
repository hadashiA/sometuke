#include "skidarak/process/process.h"

#include "skidarak/process/repeat.h"
#include "skidarak/process/sequence.h"
#include "skidarak/process/process_timer.h"

#include <functional>

namespace skidarak {

shared_ptr<Process> Process::Repeat(int num) {
    shared_ptr<Process> repeat(new class Repeat(shared_from_this(), num));
    return repeat;
}

shared_ptr<Process> Process::Delay(const ii_time duration) {
    shared_ptr<Process> timer(new class ProcessTimer(shared_from_this(),
                                                     0, ProcessTimer::REPEAT_FOREVER, duration));
    return timer;
}

shared_ptr<Process> Process::Interval(const ii_time interval) {
    shared_ptr<Process> timer(new class ProcessTimer(shared_from_this(), interval));
    return timer;
}

shared_ptr<Process> Process::Timer(const ii_time interval,
                                   const unsigned int repeat, const ii_time delay) {
    shared_ptr<Process> timer(new class ProcessTimer(shared_from_this(),
                                                     interval, repeat, delay));
    return timer;
}


template<class T>
shared_ptr<Sequence> Process::Chain() {
    shared_ptr<Process> next(new T);
    shared_ptr<Sequence> sequence(new Sequence(shared_from_this(), next));

    return sequence;
}
    
template<class T, class Arg1, class... Args>
shared_ptr<Sequence> Process::Chain(Arg1&& arg1, Args&& ... args) {
    shared_ptr<Process> next(new T(std::forward<Arg1>(arg1), std::forward<Args>(args)...));
    shared_ptr<Sequence> sequence(new Sequence(shared_from_this(), next));

    return sequence;
}

}

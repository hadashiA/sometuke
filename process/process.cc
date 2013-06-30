#include "kawaii/process/process.h"

#include "kawaii/process/repeat.h"
#include "kawaii/process/sequence.h"
#include "kawaii/process/delay.h"

#include <functional>

namespace kawaii {

shared_ptr<Process> Process::Repeat(int num) {
    shared_ptr<Process> repeat(new class Repeat(shared_from_this(), num));
    return repeat;
}

shared_ptr<Sequence> Process::Delay(const ii_time duration) {
    shared_ptr<Process> delay(new class Delay(duration));
    shared_ptr<Sequence> sequence(new Sequence(delay, shared_from_this()));

    return sequence;
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

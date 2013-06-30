#include "kawaii/process/process.h"

#include "kawaii/process/repeat.h"
#include "kawaii/process/sequence.h"

namespace kawaii {

shared_ptr<Process> Process::Repeat(int num) {
    shared_ptr<Process> repeat(new class Repeat(shared_from_this(), num));
    return repeat;
}

template<class T>
shared_ptr<Process> Process::Chain() {
    return shared_from_this();
}
    
template<class T, class Arg1, class... Args>
shared_ptr<Process> Process::Chain(Arg1&& arg1, Args&& ... args) {
    return shared_from_this();
}

}

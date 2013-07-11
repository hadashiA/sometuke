#ifndef __skidarake__process_sequence__
#define __skidarake__process_sequence__

#include "skidarake/process/process.h"

#include <memory>
#include <vector>

namespace skidarake {
using namespace std;

class Sequence : public Process {
public:
    static const HashedString TYPE;

    Sequence()
        : processes_(),
          current_(0) {
    }

    Sequence(vector<shared_ptr<Process> > list)
        : processes_(list),
          current_(0) {
    }

    Sequence(shared_ptr<Process> one, shared_ptr<Process> two)
        : current_(0) {
        processes_.push_back(one);
        processes_.push_back(two);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three)
        : current_(0) {
        processes_.push_back(one);
        processes_.push_back(two);
        processes_.push_back(three);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three,
             shared_ptr<Process> four)
        : current_(0) {
        processes_.push_back(one);
        processes_.push_back(two);
        processes_.push_back(three);
        processes_.push_back(four);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three,
             shared_ptr<Process> four,
             shared_ptr<Process> five)
        : current_(0) {
        processes_.push_back(one);
        processes_.push_back(two);
        processes_.push_back(three);
        processes_.push_back(four);
        processes_.push_back(five);
    }

    virtual const HashedString& type() const {
        return Sequence::TYPE;
    }

    virtual void OnEnter() {
        current_ = 0;
        processes_.front()->Enter();
    }

    virtual void OnExit() {
        shared_ptr<Process> process = processes_[current_];        
        process->Exit();
    }

    virtual bool Update(const ii_time delta_time);

    shared_ptr<Sequence> Push(shared_ptr<Process> process) {
        processes_.push_back(process);
        return static_pointer_cast<Sequence>(shared_from_this());
    }

    shared_ptr<Process> Pop() {
        shared_ptr<Process> last = processes_.back();
        processes_.pop_back();

        return last;
    }

    template<class T>
    shared_ptr<Sequence> Chain() {
        shared_ptr<Process> next(new T);
        return Push(next);
    }
    
    template<class T, class Arg1, class... Args>
    shared_ptr<Sequence> Chain(Arg1&& arg1, Args&& ... args) {
        shared_ptr<Process> next(new T(std::forward<Arg1>(arg1), std::forward<Args>(args)...));
        return Push(next);
    }

    shared_ptr<Sequence> operator<<(shared_ptr<Process> process) {
        return Push(process);
    }

private:
    unsigned int current_;
    vector<shared_ptr<Process> > processes_;
};

}

#endif /* defined(__skidarake__process_sequence__) */

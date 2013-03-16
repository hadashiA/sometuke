#ifndef __kawaii__process_sequence__
#define __kawaii__process_sequence__

#include "process.h"

#include <memory>
#include <list>
#include <cstdarg>

namespace kawaii {
using namespace std;

class Sequence : public Process {
public:
    static const HashedString TYPE;

    Sequence(list<shared_ptr<Process> > list)
        : initial_processes_(processes_),
          processes_(list) {
    }

    Sequence(shared_ptr<Process> one, shared_ptr<Process> two) {
        initial_processes_.push_back(one);
        initial_processes_.push_back(two);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three) {
        initial_processes_.push_back(one);
        initial_processes_.push_back(two);
        initial_processes_.push_back(three);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three,
             shared_ptr<Process> four) {
        initial_processes_.push_back(one);
        initial_processes_.push_back(two);
        initial_processes_.push_back(three);
        initial_processes_.push_back(four);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three,
             shared_ptr<Process> four,
             shared_ptr<Process> five) {
        initial_processes_.push_back(one);
        initial_processes_.push_back(two);
        initial_processes_.push_back(three);
        initial_processes_.push_back(four);
        initial_processes_.push_back(five);
    }

    virtual const HashedString& type() {
        return Sequence::TYPE;
    }


    virtual void OnEnter() {
        processes_ = initial_processes_;

        shared_ptr<Process> process = processes_.front();
        process->OnEnter();
    }

    virtual void Update(const ii_time delta_time);

private:
    list<shared_ptr<Process> > processes_;
    list<shared_ptr<Process> > initial_processes_;
};

}

#endif /* defined(__kawaii__process_sequence__) */

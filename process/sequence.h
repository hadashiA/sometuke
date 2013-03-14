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
        : processes_(list) {
    }

    Sequence(shared_ptr<Process> one, shared_ptr<Process> two) {
        processes_.push_back(one);
        processes_.push_back(two);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three) {
        processes_.push_back(one);
        processes_.push_back(two);
        processes_.push_back(three);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three,
             shared_ptr<Process> four) {
        processes_.push_back(one);
        processes_.push_back(two);
        processes_.push_back(three);
        processes_.push_back(four);
    }

    Sequence(shared_ptr<Process> one,
             shared_ptr<Process> two,
             shared_ptr<Process> three,
             shared_ptr<Process> four,
             shared_ptr<Process> five) {
        processes_.push_back(one);
        processes_.push_back(two);
        processes_.push_back(three);
        processes_.push_back(four);
        processes_.push_back(five);
    }

    virtual const HashedString& type() {
        return Sequence::TYPE;
    }

    virtual void OnEnter();
    virtual void Update(const ii_time delta_time);

private:
    list<shared_ptr<Process> > processes_;
};

}

#endif /* defined(__kawaii__process_sequence__) */

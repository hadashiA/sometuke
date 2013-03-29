#include "process.h"

#include "application.h"

namespace kawaii {

ProcessScheduler& Process::scheduler(){
    return Application::Instance().director().scheduler();
}
    
}

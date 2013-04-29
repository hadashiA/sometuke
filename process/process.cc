#include "kawaii/process/process.h"

#include "kawaii/application.h"

namespace kawaii {

ProcessScheduler& Process::scheduler(){
    return Application::Instance().director().scheduler();
}
    
}

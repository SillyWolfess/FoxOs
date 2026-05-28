#include <managers/taskManager.h>
#include <gdt.h>

Task::Task(GlobalDescriptorTable* gdt, void entrypoint()) {
    _cpuState = (CPUState*) (_stack + STACK_SIZE - sizeof(CPUState));
    _cpuState->eax = 0;
    _cpuState->ebx = 0;
    _cpuState->ecx = 0;
    _cpuState->edx = 0;

    _cpuState->esi = 0;
    _cpuState->edi = 0;
    _cpuState->ebp = 0;
/*
    _cpuState->gs = 0;
    _cpuState->fs = 0;
    _cpuState->es = 0;
    _cpuState->ds = 0;
*/
    _cpuState->error = 0;

//    _cpuState->esp = 0; // user space /security levels
    _cpuState->eip = (uint32_t) entrypoint;
    _cpuState->cs = gdt->getCodeSegment();
//    _cpuState->ss = 0; // security levels
    _cpuState->eflags = 0x202;
}

Task::~Task() {

}

TaskManager::TaskManager() {
    _nTasks = 0;
    _currentTask = -1;
}
TaskManager::~TaskManager() {

}
bool TaskManager::add(Task* task) {
    if (_nTasks >= MAX_TASKS) {
        return false;
    }
    _tasks[_nTasks++] = task;
    return true; 
}
CPUState* TaskManager::schedule(CPUState* cpuState) {
    if (_nTasks <= 0) {
        return cpuState;
    }

    if (_currentTask >= 0) {
        _tasks[_currentTask]->_cpuState = cpuState;
    }

    if (++_currentTask >= _nTasks) {
        _currentTask %= _nTasks;
    }
    return _tasks[_currentTask]->_cpuState;
}
#ifndef _TASK_MANAGER_H
#define _TASK_MANAGER_H

#include <types.h>

#define MAX_TASKS 256
#define STACK_SIZE 4096
class GlobalDescriptorTable;
/**
 * eax, ebx, ecx, edx
 * esi, edi, ebp,
 * gs, fs, es, ds, error,
 * eip, cs, eflags, esp, ss
 */
struct CPUState {
    /**
     * register accumulated
     */
    uint32_t eax;
    /**
     * base register
     */
    uint32_t ebx;
    /***
     * counting register
     */
    uint32_t ecx;
    /**
     * data register
     */
    uint32_t edx;
    /**
     * stack index
     */
    uint32_t esi;
    /**
     * data index
     */
    uint32_t edi;
    /**
     * stack base pointer
     */
    uint32_t ebp;
/*
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
*/
    uint32_t error;

    /***
     * instruction pointer
     */
    uint32_t eip;
    /**
     * code segment
     */
    uint32_t cs;
    /**
     * flags
     */
    uint32_t eflags;
    /**
     * stack pointer
     */
    uint32_t esp;
    /**
     * stack segment
     */
    uint32_t ss;

} __attribute__((packed));

class Task {
    friend class TaskManager;
    private:
        uint8_t _stack[STACK_SIZE]; // 4Kib
        CPUState* _cpuState;
    public:
        Task(GlobalDescriptorTable* gdt, void entrypoint());
        ~Task();
};

class TaskManager {
    private:
        Task* _tasks[MAX_TASKS];
        int _nTasks;
        int _currentTask;
    public:
        TaskManager();
        ~TaskManager();
        bool add(Task* task);
        CPUState* schedule(CPUState* cpuState);
};

#endif
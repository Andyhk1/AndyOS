#include "sync.h"
#include "Process/scheduler.h"

Event::Event(bool set, bool auto_reset)
{
    this->set = set;
    this->auto_reset = auto_reset;
}

void Event::Wait()
{
    Scheduler::Disable();

    if (!this->set)
    {
        THREAD *thread = Scheduler::CurrentThread();
        waiting.Enqueue(thread);
        Scheduler::BlockThread(thread, false);
        Scheduler::Enable();
        Scheduler::Switch();
    }
    else
    {
        Scheduler::Enable();
    }
}

void Event::Set()
{
    Scheduler::Disable();

    while (waiting.Count() > 0)
    {
        THREAD *thread = waiting.Dequeue();
        Scheduler::WakeThread(thread);
    }

    if (!auto_reset)
        this->set = true;

    Scheduler::Enable();
}

void Event::Clear()
{
    this->set = false;
}

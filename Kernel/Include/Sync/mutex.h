#pragma once
#include <queue.h>
#include <Process/thread.h>

class Mutex
{
private:
    Queue<THREAD *> waiting;
    THREAD *thread;
    int aquire_count;

public:
    Mutex();

    void Aquire();
    void Release();
};
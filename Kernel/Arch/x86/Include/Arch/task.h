#pragma once
#include "Process/thread.h"

namespace Task::Arch
{
    THREAD* CreateKernelThread(void(*entry)());
	THREAD* CreateUserThread(void(*entry)(), void* stack);

    void Switch();
    void Start(void(*entry)());
}
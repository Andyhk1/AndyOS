#include "timer.h"
#include "Arch/pit.h"

namespace Timer
{
    size_t Ticks()
    {
        return PIT::Ticks();
    }

	void Sleep(size_t time)
    {
        return PIT::Sleep(time);
    }
}
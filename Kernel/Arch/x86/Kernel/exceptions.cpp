#include "exceptions.h"
#include <Arch/idt.h>
#include <Arch/nmfault.h>
#include <Arch/scheduler.h>
#include <debug.h>
#include <panic.h>

namespace Exceptions {
uint32 error_code;

void ISR0(REGS *regs)
{
    panic("Division by zero");
}

void ISR1(REGS *regs)
{
    panic("Debug");
}

void ISR2(REGS *regs)
{
    panic("Non-Maskable interrupt");
}

void ISR3(REGS *regs)
{
    panic("Breakpoint");
}

void ISR4(REGS *regs)
{
    panic("Overflow");
}

void ISR5(REGS *regs)
{
    panic("Bound range exceeded");
}

void ISR6(REGS *regs)
{
    debug_pos(0, 1);
    debug_color(0xFFFF0000);

    kprintf("0x%X\t0x%X\t0x%X\t0x%X\n", regs->esp, regs->cs, regs->eip, regs->eflags);
    debug_dump((void *)regs->esp, 256);

    kprintf("Instruction: %p\n", *(uint32 *)regs->eip);
    panic("Invalid opcode");
}

void ISR8(REGS *regs)
{
    panic("Double fault");
}

void ISR9(REGS *regs)
{
    panic("Coprocessor segment overrun");
}

void ISR10(REGS *regs)
{
    panic("Invalid TSS");
}

void ISR11(REGS *regs)
{
    panic("Segment not present");
}

void ISR12(REGS *regs)
{
    panic("Stack-segment fault");
}

void ISR13(REGS *regs)
{
    regs = (REGS *)((char *)regs + 4);

    panic("General protection fault",
          "EFLAGS:%X  CS:%X  EIP:%X  ESP:%X  EBP:%X\nEAX:%X  EBX:%X  ECX:%X  "
          "EDX:%X  ESI:%X  EDI:%X\nUser ESP:%X",
          regs->eflags, regs->cs, regs->eip, regs->esp, regs->ebp, regs->eax, regs->ebx, regs->ecx,
          regs->edx, regs->esi, regs->edi, regs->user_stack);
}

void ISR15(REGS *regs)
{
    panic("15 (Reserved)");
}

void ISR16(REGS *regs)
{
    panic("x87 Floating-point exception");
}

void ISR17(REGS *regs)
{
    panic("Alignment check");
}

void ISR18(REGS *regs)
{
    panic("Machine check");
}

void ISR19(REGS *regs)
{
    panic("SIMD floating-point exception");
}

void INTERRUPT NM_ISR()
{
    asm volatile("cli\n"

                 // save registers
                 "pusha\n"
                 "push %%ds\n"
                 "push %%es\n"
                 "push %%fs\n"
                 "push %%gs\n"
                 "mov %%esp, %%ebx\n"

                 // call handler
                 "and $0xFFFFFFF0, %%esp\n"
                 "call %P0\n"

                 // restore registers
                 "mov %%ebx, %%esp\n"
                 "pop %%gs\n"
                 "pop %%fs\n"
                 "pop %%es\n"
                 "pop %%ds\n"
                 "popa\n"

                 "iret"
                 :
                 : "i"(&NMFault::Arch::HandleNMFault));
}

void INTERRUPT PageFault_ISR()
{
    asm volatile("cli\n"

                 // save error code
                 "pop %P0\n"

                 // save registers
                 "pusha\n"
                 "push %%ds\n"
                 "push %%es\n"
                 "push %%fs\n"
                 "push %%gs\n"

                 "mov %%esp, %1\n"

                 // schedule
                 "and $0xFFFFFFF0, %%esp\n"
                 "sub $12, %%esp\n"
                 "push %2\n"
                 "call %P3\n"

                 // load registers
                 "mov %4, %%esp\n"

                 "pop %%gs\n"
                 "pop %%fs\n"
                 "pop %%es\n"
                 "pop %%ds\n"
                 "popa\n"

                 "iret"
                 : "=m"(error_code), "=m"(Scheduler::Arch::tmp_stack)
                 : "N"(14), "i"(&Scheduler::Arch::ScheduleTask), "m"(Scheduler::Arch::tmp_stack));
}

bool Init()
{
    NMFault::Arch::Init();

    IDT::InstallIRQ(0, ISR0);
    IDT::InstallIRQ(1, ISR1);
    IDT::InstallIRQ(2, ISR2);
    IDT::InstallIRQ(3, ISR3);
    IDT::InstallIRQ(4, ISR4);
    IDT::InstallIRQ(5, ISR5);
    IDT::InstallIRQ(6, ISR6);
    IDT::SetISR(7, NM_ISR, 0);
    IDT::InstallIRQ(8, ISR8);
    IDT::InstallIRQ(9, ISR9);
    IDT::InstallIRQ(10, ISR10);
    IDT::InstallIRQ(11, ISR11);
    IDT::InstallIRQ(12, ISR12);
    IDT::InstallIRQ(13, ISR13);
    IDT::SetISR(14, PageFault_ISR, 0);
    IDT::InstallIRQ(15, ISR15);
    IDT::InstallIRQ(16, ISR16);
    IDT::InstallIRQ(17, ISR17);
    IDT::InstallIRQ(18, ISR18);
    IDT::InstallIRQ(19, ISR19);

    return true;
}
} // namespace Exceptions

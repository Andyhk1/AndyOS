#include <Arch/fpu.h>
#include <Arch/idt.h>
#include <Arch/pic.h>
#include <Process/scheduler.h>
#include <string.h>

namespace IDT {

struct ISR_DESC
{
    void *handler;
    bool pass_regs;
};

IDT_DESCRIPTOR idt[MAX_INTERRUPTS];
ISR_DESC handlers[MAX_INTERRUPTS];
IDT_REG idt_reg;

/*push eax; push irq; mov eax, addr; jmp eax*/
const uint8 irs_code[] = {0x50, 0x6A, 0x00, 0xB8, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0};
char isrs[MAX_INTERRUPTS * sizeof(irs_code)];

IDT_DESCRIPTOR *GetIR(uint32 i)
{
    return &idt[i];
}

void CommonHandler(int i, REGS *regs)
{
    FPU::SetTS();
    Scheduler::InterruptEnter();

    ISR_DESC &desc = handlers[i];

    if (desc.handler) {
        if (desc.pass_regs)
            ((void (*)(REGS *))desc.handler)(regs);
        else
            ((void (*)())desc.handler)();
    }

    PIC::InterruptDone(i);

    Scheduler::InterruptExit();
    FPU::SetTS();
}

void INTERRUPT CommonISR()
{
    asm volatile("cli\n"
                 "pop %%eax\n" // pop irq
                 "and $0xFF, %%eax\n"
                 "mov %%eax, -48(%%esp)\n"
                 "pop %%eax\n" // restore eax

                 "pusha\n"
                 "push %%ds\n"
                 "push %%es\n"
                 "push %%fs\n"
                 "push %%gs\n"

                 // pop irq argument into eax
                 "mov %%esp, %%ebx\n"
                 "sub $4, %%esp\n"
                 "pop %%eax\n"

                 // save esp
                 "and $0xFFFFFFF0, %%esp\n"
                 "sub $8, %%esp\n"

                 // call
                 "push %%ebx\n"
                 "push %%eax\n"
                 "call %P0\n"

                 // restore esp
                 "mov %%ebx, %%esp\n"

                 "pop %%gs\n"
                 "pop %%fs\n"
                 "pop %%es\n"
                 "pop %%ds\n"
                 "popa\n"

                 "iret" ::"i"(CommonHandler));
}

bool SetISR(uint32 i, ISR isr, int flags)
{
    if (i > MAX_INTERRUPTS || !isr)
        return false;

    uint32 uiBase = (uint32)isr;

    idt[i].low = uint16(uiBase & 0xffff);
    idt[i].high = uint16((uiBase >> 16) & 0xffff);
    idt[i].reserved = 0;
    idt[i].flags = IDT_DESC_PRESENT | IDT_DESC_BIT32 | flags;
    idt[i].sel = KERNEL_CS;

    return true;
}

bool InstallIRQ(uint32 i, void (*handler)())
{
    if (i > MAX_INTERRUPTS || !handler)
        return false;

    handlers[i].handler = (void *)handler;
    handlers[i].pass_regs = false;
    return true;
}

bool InstallIRQ(uint32 i, void (*handler)(REGS *))
{
    if (i > MAX_INTERRUPTS || !handler)
        return false;

    handlers[i].handler = (void *)handler;
    handlers[i].pass_regs = true;
    return true;
}

bool Init()
{
    idt_reg.base = (uint32)&idt;
    idt_reg.limit = sizeof(IDT_DESCRIPTOR) * MAX_INTERRUPTS - 1;

    memset(idt, 0, sizeof(IDT_DESCRIPTOR) * MAX_INTERRUPTS - 1);
    memset(handlers, 0, sizeof(handlers));

    for (int i = 0; i < MAX_INTERRUPTS; i++) {
        // Copy irq code
        char *ptr = (char *)&isrs[i * sizeof(irs_code)];
        memcpy(ptr, (char *)irs_code, sizeof(irs_code));

        // Set irq values
        uint32 target_addr = (uint32)CommonISR;

        ptr[2] = i;
        ptr[4] = target_addr & 0xFF;
        ptr[5] = (target_addr >> 8) & 0xFF;
        ptr[6] = (target_addr >> 16) & 0xFF;
        ptr[7] = (target_addr >> 24) & 0xFF;

        SetISR(i, (ISR)ptr, 0);
    }

    return true;
}

bool Enable()
{
    asm volatile("lidt (%0)" ::"r"(&idt_reg));
    return true;
}

} // namespace IDT

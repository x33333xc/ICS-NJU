#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

// Trigger an exception/interrupt with 'intr_no'
// 'intr_no' is the index to the IDT

// Push EFLAGS, CS, and EIP
// Find the IDT entry using 'intr_no'
// Clear IF if it is an interrupt
// Set CS:EIP to the entry of the interrupt handler
// need to reload CS with load_sreg()


void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
    // Push EFLAGS, CS, and EIP
    cpu.esp -= 4;
    vaddr_write(cpu.esp,2,4,cpu.eflags.val);
    cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,(cpu.cs.val&0x0000ffff));
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.eip);
	GateDesc gate;
	gate.val[0]=laddr_read((cpu.idtr.base+intr_no*8),4);
	gate.val[1]=laddr_read(((cpu.idtr.base+intr_no*8)+4),4);
	cpu.eip=gate.offset_15_0|(gate.offset_31_16<<16);
	cpu.cs.val=gate.selector;
	if(gate.type==6){
		cpu.eflags.IF=0;
	}
	
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}

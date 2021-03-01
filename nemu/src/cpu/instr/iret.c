#include "cpu/instr.h"
/*
Put the implementations of `iret' instructions here.
*/
make_instr_func(iret){
    cpu.eip=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	cpu.cs.val=(uint16_t)vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	cpu.eflags.val=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	return 0;
}
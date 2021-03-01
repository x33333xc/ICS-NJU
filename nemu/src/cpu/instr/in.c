#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `in' instructions here.
*/
//DX to AL
make_instr_func(in_b){
	print_asm_0("in","b",1);
	cpu.eax = pio_read(cpu.edx,1);
	return 1;
}

//DX to AX or EAX
make_instr_func(in_v){
	if(data_size==16){
		print_asm_0("in","w",1);
		cpu.eax = pio_read(cpu.edx,2);
	}
	else{
		print_asm_0("in","l",1);
		cpu.eax = pio_read(cpu.edx,4);
	}
	return 1;
}

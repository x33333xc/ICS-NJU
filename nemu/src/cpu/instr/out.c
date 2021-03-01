#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `out' instructions here.
*/
make_instr_func(out_b){
	print_asm_0("out","b",1);
	pio_write(cpu.edx,1,cpu.eax);
	return 1;
}

make_instr_func(out_v){
	if(data_size==16){
		print_asm_0("out","w",1);
		pio_write(cpu.edx,2,cpu.eax);
	}
	else{
		print_asm_0("out","l",1);
		pio_write(cpu.edx,4,cpu.eax);
	}
	return 1;

}

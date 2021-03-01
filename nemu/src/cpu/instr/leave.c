#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/

make_instr_func(leave){
    cpu.esp=cpu.ebp;
    OPERAND mem;
    mem.data_size=data_size;
    mem.type=OPR_MEM;
    mem.sreg=SREG_SS;
    mem.addr=cpu.esp;
    operand_read(&mem);
    //mem.val=sign_ext(mem.val,mem.data_size);
    cpu.ebp=mem.val;
    cpu.esp+=4;
    return 1;
}

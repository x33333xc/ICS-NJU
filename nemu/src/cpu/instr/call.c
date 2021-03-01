#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
make_instr_func(call_near){
    OPERAND im;
    im.type=OPR_MEM;
    im.data_size=data_size;
    im.val=eip+data_size/8+1;//next operation address
    cpu.esp-=4;
    im.addr=cpu.esp;
    im.sreg = SREG_SS;
    operand_write(&im);
    
    OPERAND imm;
    imm.type=OPR_IMM;
    imm.sreg=SREG_SS;
    imm.data_size=data_size;
    imm.addr=eip+1;
    operand_read(&imm);
    
    int offset=sign_ext(imm.val,data_size);
    cpu.eip+=offset;
    return data_size/8+1;
}

make_instr_func(call_near_indirect){
    int len=1;
	OPERAND rel,mem;
	rel.data_size=data_size;
    len+=modrm_rm(eip+1, &rel);
    operand_read(&rel);
	cpu.esp-=data_size/8;
	
	mem.data_size=data_size;
	mem.type=OPR_MEM;
	mem.sreg=SREG_DS;
	mem.addr=cpu.esp;
	mem.val=cpu.eip+len;
	operand_write(&mem);
	
	if(data_size==16)
		cpu.eip=rel.val&0xFFFF;
	else
		cpu.eip=rel.val;
	return 0;
}
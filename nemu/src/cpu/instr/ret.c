#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/

make_instr_func(ret_near){
    OPERAND im;
    im.type=OPR_MEM;
    im.sreg=SREG_DS;
    im.data_size=data_size;
    im.addr=cpu.esp;
    operand_read(&im);
    
    //pop stack
    cpu.esp+=data_size/8;
    //back to the caller
    cpu.eip=im.val;
    //printf("%x\n",im.val);
    return 0;
}

make_instr_func(ret_near_imm16){
    OPERAND im;
    im.type=OPR_MEM;
    im.data_size=data_size;
    im.addr=cpu.esp;
    operand_read(&im);
    
    //back to the caller
    cpu.eip=im.val;
    
    OPERAND imm;
	imm.type = OPR_IMM;
	imm.data_size = 16;
	imm.addr = eip + 1;
	operand_read(&imm);

	cpu.esp += data_size/8 + sign_ext(imm.val,16);
	return 0;
}

#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/

//to ebp
static void instr_execute_1op(){
    operand_read(&opr_src);
	opr_src.val=sign_ext(opr_src.val,opr_src.data_size);
	opr_dest.val=opr_src.val;
	cpu.esp-=(data_size/8);
    opr_dest.addr=cpu.esp;
	opr_dest.type=OPR_MEM;
	opr_dest.sreg=SREG_DS;
	opr_dest.data_size=data_size;
    operand_write(&opr_dest);
}

make_instr_impl_1op(push,r,v);
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, v)

make_instr_func(pusha){
    uint32_t addr = cpu.esp;
    if(data_size == 16){
        addr = addr&0xffff;
        for(int i = 0;i < 8;++i){
            if(i != 4){
                cpu.esp = cpu.esp-4;
            	OPERAND rmm;
            	rmm.type = OPR_MEM;
            	rmm.sreg = SREG_SS;
            	rmm.data_size =	16;
            	rmm.addr = cpu.esp;
            	rmm.val = cpu.gpr[i].val&0xffff;
            	operand_write(&rmm);
            }
            else{
                cpu.esp = cpu.esp-4;
            	OPERAND rmm;
            	rmm.type = OPR_MEM;
            	rmm.sreg = SREG_SS;
            	rmm.data_size =	16;
            	rmm.addr = cpu.esp;
            	rmm.val = addr;
            	operand_write(&rmm);
            }
        }
    
    }
    else if(data_size==32){
        for(int i = 0;i < 8;++i){
            if(i != 4){
                cpu.esp = cpu.esp-4;
            	OPERAND rmm;
            	rmm.type = OPR_MEM;
            	rmm.sreg = SREG_SS;
            	rmm.data_size =	32;
            	rmm.addr = cpu.esp;
            	rmm.val = cpu.gpr[i].val;
            	operand_write(&rmm);
            }
            else{
                cpu.esp = cpu.esp-4;
            	OPERAND rmm;
            	rmm.type = OPR_MEM;
            	rmm.sreg = SREG_SS;
            	rmm.data_size =	32;
            	rmm.addr = cpu.esp;
            	rmm.val = addr;
            	operand_write(&rmm);
            }
        }
    }
    return 1;
}

make_instr_func(push_i_b){
    OPERAND a,imm;
    //uint8_t imm=instr_fetch(eip+1,1);
    a.data_size=data_size;
    a.sreg=SREG_SS;
    a.type=OPR_MEM;
    
    cpu.esp-=data_size/8;
    a.addr=cpu.esp;
    
    imm.data_size=8;
    imm.type=OPR_IMM;
	imm.addr=eip+1;
	imm.sreg=SREG_SS;
	
	operand_read(&imm);
	imm.val=sign_ext(imm.val,32);
	
	a.val=imm.val;
    
    operand_write(&a);
    return 2;
    /*
    opr_src.type=OPR_IMM;
    opr_src.sreg=SREG_CS;
    opr_src.data_size=8;
    opr_src.addr=eip+1;
    //len+=opr_src.data_size/8;
    print_asm_1("push", "b", 2, &opr_src);
    operand_read(&opr_src);
    cpu.esp-=data_size/8;
    opr_src.val=sign_ext(opr_src.val,8);
    opr_dest.type=OPR_MEM;
    opr_dest.sreg=SREG_DS;
    opr_dest.addr=cpu.esp;
    opr_dest.val=opr_src.val;
    operand_write(&opr_dest);
    
    return 2;*/
    
}
#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op(){
    opr_dest.type=OPR_MEM;
	opr_dest.sreg=SREG_DS;
	opr_dest.addr=cpu.esp;
	cpu.esp+=(data_size/8);
    operand_read(&opr_dest);
	opr_dest.val=sign_ext(opr_dest.val,opr_dest.data_size);
	opr_src.val=opr_dest.val;
    operand_write(&opr_src);
}

make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_impl_1op(pop, i, b)
make_instr_impl_1op(pop, i, v)

//eax, ecx, edx, ebx, esp, ebp, esi, edi
make_instr_func(popa){
    if(data_size==16){
        for(int i=7;i>=0;--i){
            if(i!=4){
                OPERAND rmm;
            	rmm.type = OPR_MEM;
            	rmm.addr = cpu.esp;
            	rmm.sreg = SREG_SS;
            	rmm.data_size = 16;
            	
            	operand_read(&rmm);
            	cpu.esp += 4;
                cpu.gpr[i].val=rmm.val;
            }
            else{
                cpu.esp += 4;
            }
        }
    }
    else if(data_size==32){
        for(int i=7;i>=0;--i){
            if(i!=4){
                OPERAND rmm;
            	rmm.type = OPR_MEM;
            	rmm.addr = cpu.esp;
            	rmm.sreg = SREG_SS;
            	rmm.data_size = 32;
            	
            	operand_read(&rmm);
            	cpu.esp += 4;
                cpu.gpr[i].val=rmm.val;
            }
            else{
                cpu.esp += 4;
            }
        }
    }
    
    return 1;
    
    
}
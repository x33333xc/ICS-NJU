#include "cpu/instr.h"
/*
Put the implementations of `sub' instructions here.
*/
static void instr_execute_2op(){
    operand_read(&opr_src);
	operand_read(&opr_dest);
    uint32_t a=opr_src.val;
	uint32_t b=opr_dest.val;
	b=sign_ext(b,opr_dest.data_size);
	a=sign_ext(a,opr_src.data_size);
	opr_dest.val=alu_sub(a,b,opr_dest.data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(sub,i,rm,v);
make_instr_impl_2op(sub,r,rm,b);
make_instr_impl_2op(sub,rm,r,b);
make_instr_impl_2op(sub,i,a,b);
make_instr_impl_2op(sub,i,a,v);
make_instr_impl_2op(sub,i,rm,bv);
make_instr_impl_2op(sub,r,rm,v);
make_instr_impl_2op(sub,rm,r,v);
make_instr_impl_2op(sub,i,rm,b);
#include "cpu/instr.h"
/*
Put the implementations of `cmp' instructions here.
*/
static void instr_execute_2op(){
    operand_read(&opr_src);
	operand_read(&opr_dest);
    uint32_t a=opr_src.val;
	uint32_t b=opr_dest.val;
	b=sign_ext(b,opr_dest.data_size);
	a=sign_ext(a,opr_src.data_size);
	alu_sub(a,b,opr_dest.data_size);
}


make_instr_impl_2op(cmp,i,rm,bv);
make_instr_impl_2op(cmp,i,rm,v);
make_instr_impl_2op(cmp,r,rm,v);
make_instr_impl_2op(cmp,r,rm,b);
make_instr_impl_2op(cmp,i,rm,b);
make_instr_impl_2op(cmp,rm,r,v);
make_instr_impl_2op(cmp,i,a,b);
make_instr_impl_2op(cmp,i,a,v);
make_instr_impl_2op(cmp,rm,r,b);
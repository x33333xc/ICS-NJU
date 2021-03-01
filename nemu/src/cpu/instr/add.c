#include "cpu/instr.h"
/*
Put the implementations of `add' instructions here.
*/

static void instr_execute_2op(){
    operand_read(&opr_src);
	operand_read(&opr_dest);
    uint32_t a=opr_src.val;
	uint32_t b=opr_dest.val;
	b=sign_ext(b,opr_dest.data_size);
	a=sign_ext(a,opr_src.data_size);
	opr_dest.val=alu_add(a,b,opr_dest.data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(add,i,rm,v);
make_instr_impl_2op(add,r,rm,b);
make_instr_impl_2op(add,rm,r,b);
make_instr_impl_2op(add,i,a,b);
make_instr_impl_2op(add,i,a,v);
make_instr_impl_2op(add,i,rm,bv);
make_instr_impl_2op(add,r,rm,v);
make_instr_impl_2op(add,rm,r,v);
make_instr_impl_2op(add,i,rm,b);

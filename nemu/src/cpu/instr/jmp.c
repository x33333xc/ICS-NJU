#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_short){
        opr_src.val=instr_fetch(eip+1,1);
        cpu.eip+=sign_ext(opr_src.val,8);
        return 2;
        /*
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = 8;//size of short
        rel.addr = eip + 1;
        operand_read(&rel);
        int offset = sign_ext(rel.val, data_size);
        print_asm_1("jmp", "", 2, &rel);
        cpu.eip += offset;
        return 2;*/
}

make_instr_func(jmp_near_indirect){
        OPERAND rel;
        //rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        //rel.addr = eip + 1;
        modrm_rm(eip + 1, &rel);
        operand_read(&rel);
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);
        cpu.eip=sign_ext(rel.val, data_size);
        return 0;
}

make_instr_func(jmp_far_imm){
    OPERAND rel;
	rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
    rel.data_size = 32;
    rel.addr = eip + 1;
	operand_read(&rel);
	print_asm_1("jmp", "", 7, &rel);
	if(data_size == 16)
		cpu.eip = rel.val & 0xFFFF;
	else
		cpu.eip = rel.val;
	cpu.cs.val = instr_fetch(eip + 5, 2);
	load_sreg(SREG_CS);
    return 0;
}

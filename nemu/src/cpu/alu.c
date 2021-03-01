#include "cpu/cpu.h"

void set_CF_add(uint32_t result, uint32_t src, size_t data_size){
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.CF = result < src;
}

void set_CF_adc(uint32_t result, uint32_t src, size_t data_size){
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    if(cpu.eflags.CF==0)
        cpu.eflags.CF=(result<src);//same as set_CF_add
    else{
        if(result<=src)
            cpu.eflags.CF=1;
        else cpu.eflags.CF=0;
    }
}

void set_CF_mul(uint64_t res, size_t data_size){
    switch(data_size){
        case 8: 
            cpu.eflags.CF=res>(0xFF);
            break;
        case 16: 
            cpu.eflags.CF=res>(0xFFFF);
            break;
        default: 
            cpu.eflags.CF=res>(0xFFFFFFFF);
            break;
    }
}

void set_CF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    cpu.eflags.CF = result > dest;
}

void set_CF_sbb(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
    src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
    if(cpu.eflags.CF==0){
        cpu.eflags.CF = result > dest;
    }
    else{
        if(result>=dest)
            cpu.eflags.CF=1;
        else cpu.eflags.CF=0;
    }
}

void set_PF(uint32_t result){
    result = result & 0xFF;//compare low 8 bit
    result = result ^ (result>>1);
    result = result ^ (result>>2);
    result = result ^ (result>>4);
    result = result ^ (result>>8);
    result = result ^ (result>>16);
    cpu.eflags.PF = !(result & 0x1);
    /*
    int count = 0;
    uint32_t pf = (result & 0xFF);
    while(pf != 0){
        count += (pf & 0x1);
        pf = pf >> 1;
    }
    cpu.eflags.PF = ((count%2) == 0);*/
    
}

void set_ZF(uint32_t result, size_t data_size){
    result = result & (0xFFFFFFFF>>(32 - data_size));
    cpu.eflags.ZF = (result == 0);
}

void set_SF(uint32_t result, size_t data_size){
    result = sign_ext(result & (0xFFFFFFFF>>(32 - data_size)), data_size);
    cpu.eflags.SF = sign(result);
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    switch(data_size){
        case 8:
            result = sign_ext(result & 0xFF, 8);
            src = sign_ext(src & 0xFF, 8);
            dest = sign_ext(dest & 0xFF, 8);
            break;
        case 16:
            result = sign_ext(result&0xFFFF, 16);
            src = sign_ext(src & 0xFFFF, 16);
            dest = sign_ext(dest & 0xFFFF, 16);
            break;
        default: break;
    }
    if(sign(src) == sign(dest)){
        if(sign(src) != sign(result))
            cpu.eflags.OF = 1;
        else
            cpu.eflags.OF = 0;
    }
    else{
        cpu.eflags.OF = 0;
    }
}

void set_OF_adc(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    set_OF_add(result,src,dest,data_size);
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    switch(data_size){
        case 8:
            result = sign_ext(result & 0xFF, 8);
            src = sign_ext(src & 0xFF, 8);
            dest = sign_ext(dest & 0xFF, 8);
            break;
        case 16:
            result = sign_ext(result&0xFFFF, 16);
            src = sign_ext(src & 0xFFFF, 16);
            dest = sign_ext(dest & 0xFFFF, 16);
            break;
        default: break;
    }
    if(sign(src) != sign(dest)){
        if(sign(dest) != sign(result))
            cpu.eflags.OF = 1;
        else
            cpu.eflags.OF = 0;
    }
    else{
        cpu.eflags.OF = 0;
    }
}

void set_OF_sbb(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
    switch(data_size){
        case 8:
            result = sign_ext(result & 0xFF, 8);
            src = sign_ext(src & 0xFF, 8);
            dest = sign_ext(dest & 0xFF, 8);
            break;
        case 16:
            result = sign_ext(result&0xFFFF, 16);
            src = sign_ext(src & 0xFFFF, 16);
            dest = sign_ext(dest & 0xFFFF, 16);
            break;
        default: break;
    }
    if(sign(src) != sign(dest)){
        if(sign(dest) != sign(result))
            cpu.eflags.OF = 1;
        else
            cpu.eflags.OF = 0;
    }
    else{
        cpu.eflags.OF = 0;
    }
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint32_t res = 0;
	res = src + dest;
	
	set_CF_add(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res,src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
	
	
	
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint32_t res = 0;
	res = src + dest + cpu.eflags.CF;
	
	set_CF_adc(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_adc(res,src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
    /*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint32_t res = 0;
	switch(data_size){
	    case 16: 
	        res=sign_ext(res&0xFFFF,16);
	        break;
	    case 32: 
	        res=sign_ext(res&0xFFFFFFFF,32);
	        break;
	}
    res=dest-src;
	set_CF_sub(res, src, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sub(res, src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
    /*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint32_t res = 0;
	switch(data_size){
	    case 16: 
	        res=sign_ext(res&0xFFFF,16);
	        break;
	    case 32: 
	        res=sign_ext(res&0xFFFFFFFF,32);
	        break;
	}
	res=dest-(src+cpu.eflags.CF);
	set_CF_sbb(res, src, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sbb(res, src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
	
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
    /*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint64_t res = 0;
	src=src&(0xFFFFFFFF >> (32 - data_size));
	dest=dest&(0xFFFFFFFF >> (32 - data_size));
	res=(uint64_t)src*(uint64_t)dest;
	//printf("src = %u; dest = %u; res= %llu",src,dest,res);
	set_CF_mul(res,data_size);
	cpu.eflags.OF=cpu.eflags.CF;
	return res&(0xFFFFFFFFFFFFFFFF>>(64-2*data_size));
	
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	int64_t res=(int64_t)src*(int64_t)dest;
	return res;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	//src=src&(0xFFFFFFFFFFFFFFFF>>(64-2*data_size));
	//dest=dest&(0xFFFFFFFFFFFFFFFF>>(64-2*data_size));
	uint64_t res=dest/src;
	return res;
	
	
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	int32_t res=dest/src;
	return res;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint64_t res=dest%src;
	return res;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	int32_t res=dest%src;
	return res;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint32_t res=src&dest;
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint32_t res=src^dest;
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
    /*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint32_t res=src|dest;
	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	
	uint32_t res=dest;
	while(src>0){
	    cpu.eflags.CF=res>>(data_size-1);//high-order bit
	    res=res<<1;
	    src--;
	}
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
	
#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	uint32_t res=dest& (0xFFFFFFFF >> (32 - data_size));
	while(src>0){
	    cpu.eflags.CF=res&1;//low-order bit
	    res=res>>1;
	    src--;
	}
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	/*printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	int32_t res=dest;
	res=sign_ext(res,data_size);
	while(src>0){
	    cpu.eflags.CF=res&1;//low-order bit
	    res=res>>1;
	    src--;
	}
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	
	return res & (0xFFFFFFFF >> (32 - data_size));
	
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
/*
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;*/
	return alu_shl(src,dest,data_size);
#endif
}

#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];//128mb

uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data)
{
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	int NO=is_mmio(paddr);
	if(NO==-1){
#ifdef CACHE_ENABLED
		ret=cache_read(paddr,len);
#else
		ret=hw_mem_read(paddr,len);
#endif
	}
	else ret=mmio_read(paddr,len,NO);
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data)
{
	int NO=is_mmio(paddr);
	if(NO==-1){
#ifdef CACHE_ENABLED
		cache_write(paddr,len,data);
#else
		hw_mem_write(paddr,len,data);
#endif
	}
	else{
		mmio_write(paddr,len,data,NO);
	}
}

uint32_t laddr_read(laddr_t laddr, size_t len)
{
#ifndef IA32_PAGE
    return paddr_read(laddr, len);
#else
    if( cpu.cr0.pg==1 ) {
		if ((laddr>>12)!=((laddr+len-1)>>12)) {
		    
			///* this is a special case, you can handle it later. */
			//printf("cross page\n");
			//assert(0);
			uint32_t ret=0;
			uint32_t num=laddr&0xfff;
			uint32_t t=4096-num;
			ret=paddr_read(page_translate(laddr),t);
			ret+=((paddr_read(page_translate(laddr+t),len-t))<<(8*t));
			return ret;
		} 
		else {
			paddr_t paddr = page_translate(laddr);
			return paddr_read(paddr, len);	
		}
	} 
	else { return paddr_read(laddr, len); }
#endif
	
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data)
{
#ifndef IA32_PAGE
	paddr_write(laddr, len, data);
#else
    if( cpu.cr0.pg==1 ) {
		if ((laddr>>12)!=((laddr+len-1)>>12)) {
			/* this is a special case, you can handle it later. */
			//printf("cross page\n");
			//assert(0);
			uint32_t num = laddr & 0xfff;
			uint32_t t = 4096 - num;
			paddr_write(page_translate(laddr), t, data);
			paddr_write(page_translate(laddr + t), len - t, data >> (8 * t));
		} else {
			paddr_t paddr = page_translate(laddr);
			paddr_write(paddr, len, data);	
		}
	} 
	else { paddr_write(laddr, len, data); }
#endif
}

//顶层读接口
uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	//return laddr_read(vaddr, len);
#ifndef IA32_SEG
	return laddr_read(vaddr, len);
#else
	uint32_t laddr = vaddr;
	if(  cpu.cr0.pe ) {
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_read(laddr, len);
#endif

}

//顶层写接口
void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	//laddr_write(vaddr, len, data);
#ifndef IA32_SEG
	laddr_write(vaddr, len, data);
#else
	if(cpu.cr0.pe)
	{
		vaddr=segment_translate(vaddr,sreg);
		laddr_write(vaddr,len,data);
	}
	else
		laddr_write(vaddr,len,data);
#endif
}

void init_mem()
{
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);

#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr()
{
	return hw_mem;
}

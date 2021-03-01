#include "cpu/cpu.h"
#include <memory.h>
#include "memory/memory.h"

uint32_t hw_mem_read(paddr_t paddr, size_t len);
void hw_mem_write(paddr_t paddr, size_t len, uint32_t data);

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	/*printf("\nPlease implement page_translate()\n");
	fflush(stdout);
	assert(0);*/
	/*  DIR  10   位？
        PAGE  10   位？
        OFFSET  12   位？*/
	uint32_t dir=(laddr>>22)&0x3ff;
	uint32_t page=(laddr>>12)&0x3ff;
	uint32_t offset=laddr&0xfff;
	PDE page_dir;
	PTE page_table;
	page_dir.val=hw_mem_read((cpu.cr3.pdtr<<12)+(dir*4),4);
	page_table.val=hw_mem_read((page_dir.page_frame<<12)+(page*4),4);
	assert(page_dir.present==1);
	assert(page_table.present==1);
	uint32_t paddr=(page_table.page_frame<<12)+offset;
	return paddr;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}

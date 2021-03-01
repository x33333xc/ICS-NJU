#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	//uint32_t base=cpu.segReg[sreg].base;
	//return base+offset;
	return cpu.segReg[sreg].base+offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	//printf("%x\n",sreg);
	//assert (cpu.segReg[sreg].ti==0);
	SegDesc new_seg;
	memcpy(&new_seg,hw_mem+cpu.gdtr.base+cpu.segReg[sreg].index*8,sizeof(new_seg));
	
	//paste the limit and base
	uint32_t LIMIT=(new_seg.limit_19_16<<16) + new_seg.limit_15_0;
	uint32_t BASE=(new_seg.base_31_24<<24) + (new_seg.base_23_16<<16) + new_seg.base_15_0;
	
	//uint32_t LIMIT=0xffffffff;
	//uint32_t BASE=0;
	
	assert(BASE==0);
	assert(LIMIT<=0xffffffff);
	assert(new_seg.granularity==1);
	assert(new_seg.present==1);
	
	//load the hidden part
	cpu.segReg[sreg].base=BASE;
	cpu.segReg[sreg].limit=LIMIT;
	cpu.segReg[sreg].type=(new_seg.segment_type<<4)+new_seg.type;
	cpu.segReg[sreg].privilege_level=new_seg.privilege_level;
	cpu.segReg[sreg].soft_use=new_seg.soft_use;
}

/*
struct
	{
		uint32_t limit_15_0 : 16;
		uint32_t base_15_0 : 16;
		uint32_t base_23_16 : 8;
		uint32_t type : 4;
		uint32_t segment_type : 1;
		uint32_t privilege_level : 2;
		uint32_t present : 1;
		uint32_t limit_19_16 : 4;
		uint32_t soft_use : 1;
		uint32_t operation_size : 1;
		uint32_t pad0 : 1;
		uint32_t granularity : 1;
		uint32_t base_31_24 : 8;
	};
*/


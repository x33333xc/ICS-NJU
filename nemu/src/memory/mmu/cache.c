#include "memory/mmu/cache.h"
#include"nemu.h"
#include"memory/memory.h"
#include<memory.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
/*
CacheLine cache[1024];

// init the cache
void init_cache()
{
    for(int i=0;i<1024;++i) cache[i].valid=0;   //not valid
	// implement me in PA 3-1
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
    //32bit paddr:(sign) 19bit;(group number:0~127) 7bit;(addr in block:0~63) 6bit
    uint32_t sign =(paddr>>13)&0x7ffff;
	uint32_t group_num =(paddr>>6)&0x7f;
	uint32_t offset=paddr&0x3f;
	
	int i;
	for(i=0;i<8;++i){
        if(cache[group_num*8+i].sign==sign&&cache[group_num*8+i].valid==1){//hit the sign && valid
            if(offset+len<=64){
                memcpy(cache[group_num*8+i].block+offset,&data,len);
            }
            else{
                cache_write(paddr,64-offset,data);
                cache_write(paddr+64-offset,offset+len-64,data>>(8*(64-offset)));
                
            }
            break;
        }
	}
	// implement me in PA 3-1
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
    uint32_t ret;
    uint32_t sign =(paddr>>13)&0x7ffff;
	uint32_t group_num =(paddr>>6)&0x7f;
	uint32_t offset=paddr&0x3f;
    int i;
    for(i=0;i<8;++i){
        if(cache[group_num*8+i].sign==sign&&cache[group_num*8+i].valid==1){//hit the sign && valid
            if(offset+len<=64){//not cross line
                memcpy(&ret,cache[group_num*8+i].block+offset,len);
            }
            else{// (*offset+len>64)cross line 1. read 64-offset 2. read next offset+len-64 bit
                uint32_t front=0, back=0;
                memcpy(&front,cache[group_num*8+i].block+offset,64-offset);
                back=cache_read(paddr+64-offset,offset+len-64);
                back<<=(8*(64-offset));
                ret=front|back;
            }
            
            break;
        }
    }
    if(i==8){//not in cache
        //copy the data from hardware_addr
        memcpy(&ret,hw_mem+paddr,len);
        
        //look for spare block
        for(i=0;i<8;++i){
            if(cache[group_num*8+i].valid==0){
                cache[group_num*8+i].sign=sign;
                cache[group_num*8+i].valid=1;
                memcpy(&cache[group_num*8+i].block,hw_mem+paddr-offset,64);
                break;
            }
        }
        //no spare rand search
        if(i==8){
            srand((unsigned)time(0));
			i=rand()%8;
			cache[group_num*8+i].sign=sign;
            cache[group_num*8+i].valid=1;
            memcpy(&cache[group_num*8+i].block,hw_mem+paddr-offset,64);
        }
    }
	// implement me in PA 3-1
	return ret;
}
*/

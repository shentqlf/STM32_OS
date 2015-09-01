#ifndef __FLASH_H_
#define __FLASH_H_
#include <stm32f10x.h>

void WrFlash(u32 StartAddr,u32 buf[],u32 len);
int RdFlash(u32 StartAddr,u32 *p_data,u32 size) ;

#endif

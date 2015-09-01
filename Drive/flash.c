#include "flash.h"
#include <stm32f10x.h>
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
/* Define the STM32F10x FLASH Page Size depending on the used STM32 device */
#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

#define BANK1_WRITE_START_ADDR  ((uint32_t)0x0800f000)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/  
uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data = 0x3210ABCD;
uint32_t NbrOfPage = 0x00;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
volatile TestStatus MemoryProgramStatus = PASSED;

void WrFlash(u32 StartAddr,u32 buf[],u32 len)
{
	u32 i;
	u32 EndAddr;
	StartAddr |= BANK1_WRITE_START_ADDR;
	EndAddr = StartAddr + len*4;
/* Porgram FLASH Bank1 ********************************************************/       
  /* Unlock the Flash Bank1 Program Erase controller */
  FLASH_UnlockBank1();

	
	EE_Init();
  /* Define the number of page to be erased */
  NbrOfPage=((EndAddr-StartAddr)>>10)+1;

  /* Clear All pending flags */
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

  /* Erase the FLASH pages */
  for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
  {
    FLASHStatus = FLASH_ErasePage(StartAddr + (FLASH_PAGE_SIZE * EraseCounter));
  }
  
  /* Program Flash Bank1 */
  Address = StartAddr;
	i = 0;
  while((Address < EndAddr) && (FLASHStatus == FLASH_COMPLETE))
  {
    FLASHStatus = FLASH_ProgramWord(Address, buf[i++]);
    Address = Address + 4;
  }

  FLASH_LockBank1();
  
  /* Check the correctness of written data */
  Address = StartAddr;
	i=0;
  while((Address < EndAddr) && (MemoryProgramStatus != FAILED))
  {
    if((*(__IO uint32_t*) Address) != buf[i++])
    {
      MemoryProgramStatus = FAILED;
    }
    Address += 4;
  }
}

int RdFlash(u32 StartAddr,u32 *p_data,u32 size)
{
	u32 EndAddr;
	int MemoryProgramStatus=1;
	u32 Address = 0x0;
	int i=0;
	StartAddr |= BANK1_WRITE_START_ADDR;
	EndAddr=StartAddr+size*4;
	Address = StartAddr;
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{
		p_data[i++]=(*(vu32*) Address);
		Address += 4;	
	}
	return 0;	
}

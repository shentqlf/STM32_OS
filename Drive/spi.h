#include "stm32f10x.h"

/* SPI总线速度设置*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

void
SPI1_Configuration(void);
u8
SPI1_ReadWriteByte(u8 TxData);

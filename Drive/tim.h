#ifndef __TIM_H
#define __TIM_H
#include <stm32f10x.h>
void TIM2_RCC_Configuration(void);
void TIM2_GPIO_Configuration(void);
void TIM2_Configuration(void);
void TIM2PulseCfg(u8 ch,u8 pulse);

#endif
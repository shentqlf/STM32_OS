#ifndef __LED_MATRIX_H
#define __LED_MATRIX_H
#include <stm32f10x.h>

void
led(void);
void
Led_Matrix_GPIO_Init(void);

void LED_PutPoint(u16 x, u16 y,u8 color);
void MyPutChar(u16 x, char cha);
void LED_putchar(char num,char str);
void MyPutStr(char num, char *str);
void LED_putstr(char num,char *str);
void LED_putHZ(char num, char str);
void DispCarNum(char *carnum);
void DispRMB(char *RMB);
void DispInternalCar(void);
void send256(void);
#endif

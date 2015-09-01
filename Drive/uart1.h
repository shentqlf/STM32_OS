#ifndef __UART1_H
#define __UART1_H
#include "stdio.h"

void usart1_init(void);
int PutChar(char ch);
void PutStr(char *str);
#endif

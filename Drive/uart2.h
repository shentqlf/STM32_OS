#ifndef __UART2_H
#define __UART2_H
#include "stdio.h"

void Usart2Init(void);
int Usart2PutChar(char ch);
void Usart2PutStr(char *str);
#endif

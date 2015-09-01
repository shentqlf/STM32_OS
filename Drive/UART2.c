
#include "uart2.h"
#include "stm32f10x.h"
#include <stdio.h>

#include <string.h>

void Usart2Init(void)
{
  USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE );
   RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );
    
    /* Configure USARTx_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USARTx_Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);


    //USART_ClearITPendingBit(USART1, USART_IT_TC); 
    /* Enable the USART Transmoit interrupt: this interrupt is generated when 
the 
    USART1 transmit data register is empty */  
  //  USART_ITConfig(USART1, USART_IT_TC, ENABLE);
   // USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

    /* Enable the USART Receive interrupt: this interrupt is generated when 
the 
    USART1 receive data register is not empty */
   // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   // USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
      /* Enable USART1 */
//    
//    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
//    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    USART_Cmd(USART2, ENABLE);
//    DMA_Cmd(DMA1_Channel6, ENABLE);
    //DMA_Cmd(DMA1_Channel7, ENABLE);	
}

int Usart2PutChar(char ch)
{
	USART_SendData(USART2,ch);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	return ch;
}

void Usart2PutStr(char *str)
{
	while(*str!='\0')
	{
	USART_SendData(USART2,*str++);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);

	}
}	



//void USART2_IRQHandler(void)
//{
//	
//	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);

//}



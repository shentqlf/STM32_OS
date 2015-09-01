
/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include <stdio.h>
#include "macro.h"
#include "stm32f10x_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//u8 UART1RecBuffer[UART1BUFFER_MAX];
//UART1SenBuffer[0]存放函数被调用时存放的字符串；UART1SenBuffer1存放的是串口1接受的字符
static ST_UARTBUFFER g_acUART1RecBuffer;
static ST_UARTBUFFER g_acUART1SendBuffer[PRINTFBUF_NUM];
static ST_PRINTFBUFFER g_stPrintBuffer;
static u8 g_acUART2SendBuffer[UART_BUFFLEN] = {0};


void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* DMA channel1 configuration --------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;//&(USART1.DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = UART_BUFFLEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);
    DMA_ClearITPendingBit(DMA1_IT_TC4);
    DMA_ClearITPendingBit(DMA1_IT_HT4);
    /* Enable Channel5 Transfer complete interrupt */
    DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

    /* Enable the DMA Channel 1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    
}


void DMAUART1_Configuration(u8 *uiSendBuf,u32 uiLength)
{
    DMA_InitTypeDef DMA_InitStructure;
    //DMA设置：
    //设置DMA源：内存地址&串口数据寄存器地址
    //方向：内存-->外设
    //每次传输位：8bit
    //传输大小DMA_BufferSize=SENDBUFF_SIZE
    //地址自增模式：外设地址不增，内存地址自增1
    //DMA模式：一次传输，非循环
    //优先级：中
    DMA_DeInit ((DMA_Channel_TypeDef*)DMA1_Channel4_BASE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;//&(USART1.DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)uiSendBuf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = uiLength;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

     DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
     DMA_Cmd(DMA1_Channel4, ENABLE);
}


int printf(const char *format ,... )
{
    va_list arg;
    int rv;
    u8 ucIndex;
    static u8 ucCounter = 0;
    

    for (ucIndex = 0 ; ucIndex < PRINTFBUF_NUM ;ucIndex++)
    {
        if ( g_acUART1SendBuffer[ucIndex].iFullFlag == BUFF_EMPTY)
        {
            g_acUART1SendBuffer[ucIndex].iFullFlag = BUFF_FULL;
            va_start(arg, format);
            rv = vsprintf((char *)(g_acUART1SendBuffer[ucIndex].ucBuffer), format, arg);
            g_acUART1SendBuffer[ucIndex].sLength = rv;
            va_end(arg);
            break;
        }
        else
        {

        }
    }

    if (ucIndex == PRINTFBUF_NUM)
    {
        g_stPrintBuffer.iFullFlag = BUFF_FULL;
        ucCounter ++;
    }
    else
    {
        if (g_stPrintBuffer.iFullFlag != BUFF_FULL)
        {
            for (ucIndex = 0 ; ucIndex < PRINTFBUF_NUM ;ucIndex++)
            {
                if ( g_acUART1SendBuffer[ucIndex].iFullFlag == BUFF_FULL)
                {
                    g_stPrintBuffer.ucIndex = ucIndex;
                    break;
                }
            }
            if (ucIndex == PRINTFBUF_NUM)
            {
                g_stPrintBuffer.iFullFlag = BUFF_FULL;
            }
            else
            {
                DMAUART1_Configuration(g_acUART1SendBuffer[g_stPrintBuffer.ucIndex].ucBuffer,g_acUART1SendBuffer[g_stPrintBuffer.ucIndex].sLength);
                g_stPrintBuffer.iFullFlag = BUFF_HAS_DATA;
            }
            
        }
        else
        {

        }
    }
    
    
    return rv;
}

/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - Two Stop Bit
        - Odd parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the second edge 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
void UART1Init (void)
{  
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE );

    
    /* Configure USARTx_Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USARTx_Rx as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    DMA_Config();
/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);


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
    USART_Cmd(USART1, ENABLE);
    
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

    
}

/******************************************************************************
*
* Function Name  : GetPrintfBufferAddr
* Description    : 
获取g_TestMode的地址
* Input          : None
* Output         : g_TestMode的地址
* Return         : 
* History:         
  <author>		<time>		<version >		<describe>
 zhangyiming  2008.11.21        1.0           新增函数
*******************************************************************************/
ST_PRINTFBUFFER* GetPrintfBufferAddr ( void )
{
    return &g_stPrintBuffer;
}

/******************************************************************************
*
* Function Name  : GetUART1SendBufferAddr
* Description    : 
获取g_TestMode的地址
* Input          : None
* Output         : g_TestMode的地址
* Return         : 
* History:         
  <author>		<time>		<version >		<describe>
 zhangyiming  2009.2.4        1.0           新增函数
*******************************************************************************/
ST_UARTBUFFER* GetUART1SendBufferAddr( void )
{
    return g_acUART1SendBuffer;
}




    
void DMA1_6Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* DMA channel1 configuration --------------------------------------------*/
    DMA_DeInit(DMA1_Channel6);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_BASE+4;//&(USART2.DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_acUART2SendBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = UART_BUFFLEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);

    /* Enable the DMA Channel 1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}


    
void DMA1_7Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    /* DMA channel1 configuration --------------------------------------------*/
    DMA_DeInit(DMA1_Channel7);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_BASE+4;//&(USART2.DR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_acUART2SendBuffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = UART_BUFFLEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);

    
}



/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - Two Stop Bit
        - Odd parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the second edge 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
void UART2Init (void)
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
    
    DMA1_7Config();
    DMA1_6Config();
/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle 
        - USART LastBit: The clock pulse of the last data bit is not output to 
                         the SCLK pin
  */
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
    
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
    USART_Cmd(USART2, ENABLE);
    DMA_Cmd(DMA1_Channel6, ENABLE);
    //DMA_Cmd(DMA1_Channel7, ENABLE);
    
}

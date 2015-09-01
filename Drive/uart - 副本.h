/**
  ******************************************************************************
  * @file IAP/inc/platform_config.h 
  * @author   MCD Application Team
  * @version  V3.0.0
  * @date     04/27/2009
  * @brief    Evaluation board specific configuration file.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UART_H
#define _UART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "macro.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DMA_Config(void);
int printf(const char *format ,... );
void UART1Init (void);
void DMAUART1_Configuration(u8 *uiSendBuf,u32 uiLength);
ST_PRINTFBUFFER* GetPrintfBufferAddr ( void );
ST_UARTBUFFER* GetUART1SendBufferAddr( void );
void UART2Init (void);
void DMA1_7Config(void);
#endif /* _UART_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

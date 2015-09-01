/************************************************************
  Copyright (C), 2008-1,  Tech. Co., Ltd.
  FileName: VarData.c
  Author:			Version :		Date:      
  zhangyiming          1.0              2008.11.2
  Description:    涉及到的全局变量均在该文件内
  Version:         1.0
  Function List:   
	1. GetUART1RecBufferAdd()
	2. GetUART1SenBufferAdd()
    
  History:         
 	<author>		<time>		<version >		<describe>
   zhangyiming     96/10/12        1.0      build this moudle  
***********************************************************/
#ifndef __VARDATA_H__
#define __VARDATA_H__
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */



#define EOS '\0'			/*END OF STRING*/
#define KEY_SOH 1      /*标题开始*/
#define KEY_ETB 23      /*信息传输结束*/
#define KEY_CR 13
#define KEY_LF 10			
#ifndef NULL
#define NULL 0x00
#endif /* NULL */
#define OS_TICKS_PER_SEC        1000    /* Set the number of ticks in one second*/
#define NULL_PTR ((void *) NULL)//空指针
#define TESTMODE 0x55    /*测试模式的值*/
#define AD_SIZE 256
#define PLLCLK ((u32)72000000) /*CPU主频*/

#define UART_BUFFLEN 256/*串口缓冲大小*/
#define PRINTFBUF_NUM 3/*打印缓冲个数*/
#define PRINTFULL ("print buffer is full!\r\n")

typedef enum 
{
  EUQAL = 0, 
  UNEUQAL
} RESULT_CMP;

typedef struct
{
    unsigned int UART1SndFlag :1;
    unsigned int UART1RcvFlag :1;
    unsigned int BIT_VAR_RESERVE;	
}BIT_VAR;

typedef enum
{
    PRINT_LF_CR_NO,
    PRINT_LF_CR_YES
}enmPRINT_LF_CR;/*是否需要打印回车换行*/

typedef enum
{
    UART1SNDFLAG,
    UART1RCVFLAG,
    BIT_VAR_TYPE_RESERVE
}enmBIT_VAR_TYPE;

typedef enum
{
    ALLCLUTCH = 0,
    MAINCLUTCH,
    BREAKCLUTCH,
    WEFTCLUTCH,
    SLOWCLUTCH
}enmClutchType;/*刹车类型*/

/*函数错误类型*/
typedef enum 
{
    RETURN_SUCCESS = 0,
    INPUT_PARAM_IS_NULL,
    INPUT_PARAM_ERR,
    INPUT_PARAM_LESS,//输入参数太小
    INPUT_PARAM_BIGNESS,	//输入参数太大
    NUM_OF_INPUT_PARAM_MUCH,//输入参数的个数超限制
    INPUT_PARAM_OUT_OF_RANGE,//
    FUNCTION_BUFFER_IS_EMPTY,
    FUNCTION_BUFFER_IS_FULL,
    FUNCTION_BUFFER_IS_HAIF,//函数数据还未准备好
    FUNCTION_SEARCH_FAIL,//没找到指定的数据
    FUNCTION_BUFFER_IS_NOT_CHAR,
    FUNCTION_CRC_ERR,
    FUNCTION_USE_DEFAULT,
    FUNCTION_SECTOR_NOT_ERASED,
    FUNCTION_SECTOR_NOT_DEFINED,  //没有找到指定的块地址
    FUNCTION_SECTOR_WRITE_ERR,
    FUNCTION_SECTOR_ERR            //扇区擦除失败
}enmFunctionRecturnType;

/*BOOL状态类型,针对读出值的状态*/
typedef enum
{
    INEFFECTIVE = 0,/*无效*/
    EFFECTIVE
}BOOL_ST;


/*缓冲指示*/
typedef enum
{ 
    BUFF_EMPTY = 0,
    BUFF_HAS_DATA,
    BUFF_FULL
}BUFF_FLAG;
typedef struct
{
    u16 sPriority;
    u16 sLength ;
    BUFF_FLAG iFullFlag;
    u8 ucBuffer[UART_BUFFLEN];
}ST_UARTBUFFER;


typedef struct
{
    BUFF_FLAG iFullFlag;
    u8 ucIndex;
}ST_PRINTFBUFFER;




/*闪灯控制*/
typedef enum
{
    LAMP_OFF,
    LAMP_ON,
    LAMP_FLASH_TIME
}enmLampFlashStyle;

/*选色器的状态*/
typedef enum
{
    CS_OFF,
    CS_1,
    CS_2,
    CS_3,
    CS_4,
    CS_5,
    CS_6,
    CS_7,
    CS_8,
    CS_ALL_ON
}enmColorSel;


typedef union
{
    u16 cValue;
    struct
	{
        u16 bit0 :1;
        u16 bit1 :1;
        u16 bit2 :1;
        u16 bit3 :1;
        u16 bit4 :1;
        u16 bit5 :1;
        u16 bit6 :1;
        u16 bit7 :1;
        u16 bit8 :1;
        u16 bit9 :1;
        u16 bit10 :1;
        u16 bit11 :1;
        u16 bit12 :1;
        u16 bit13 :1;
        u16 bit14 :1;
        u16 bit15 :1;
    }strData;
}u32_bit;


typedef union
{
    u16 cValue;
    struct
	{
        u16 bit0 :1;/**/
        u16 bit1 :1;
        u16 bit2 :1;
        u16 bit3 :1;
        u16 bit4 :1;
        u16 BreakClutchMVVol :1;/*刹车中压*/
        u16 BreakClutchLVVol :1;/*刹车低压*/
        u16 bit7 :1;
        u16 bit8 :1;
        u16 bit9 :1;
        u16 bit10 :1;
        u16 bit11 :1;
        u16 bit12 :1;
        u16 bit13 :1;
        u16 bit14 :1;
        u16 bit15 :1;
    }strData;
}unionNewPara_bit;

#endif /*__VARDATA_H__*/


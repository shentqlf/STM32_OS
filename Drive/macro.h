/************************************************************
  Copyright (C), 2008-1,  Tech. Co., Ltd.
  FileName: VarData.c
  Author:			Version :		Date:      
  zhangyiming          1.0              2008.11.2
  Description:    �漰����ȫ�ֱ������ڸ��ļ���
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
#define KEY_SOH 1      /*���⿪ʼ*/
#define KEY_ETB 23      /*��Ϣ�������*/
#define KEY_CR 13
#define KEY_LF 10			
#ifndef NULL
#define NULL 0x00
#endif /* NULL */
#define OS_TICKS_PER_SEC        1000    /* Set the number of ticks in one second*/
#define NULL_PTR ((void *) NULL)//��ָ��
#define TESTMODE 0x55    /*����ģʽ��ֵ*/
#define AD_SIZE 256
#define PLLCLK ((u32)72000000) /*CPU��Ƶ*/

#define UART_BUFFLEN 256/*���ڻ����С*/
#define PRINTFBUF_NUM 3/*��ӡ�������*/
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
}enmPRINT_LF_CR;/*�Ƿ���Ҫ��ӡ�س�����*/

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
}enmClutchType;/*ɲ������*/

/*������������*/
typedef enum 
{
    RETURN_SUCCESS = 0,
    INPUT_PARAM_IS_NULL,
    INPUT_PARAM_ERR,
    INPUT_PARAM_LESS,//�������̫С
    INPUT_PARAM_BIGNESS,	//�������̫��
    NUM_OF_INPUT_PARAM_MUCH,//��������ĸ���������
    INPUT_PARAM_OUT_OF_RANGE,//
    FUNCTION_BUFFER_IS_EMPTY,
    FUNCTION_BUFFER_IS_FULL,
    FUNCTION_BUFFER_IS_HAIF,//�������ݻ�δ׼����
    FUNCTION_SEARCH_FAIL,//û�ҵ�ָ��������
    FUNCTION_BUFFER_IS_NOT_CHAR,
    FUNCTION_CRC_ERR,
    FUNCTION_USE_DEFAULT,
    FUNCTION_SECTOR_NOT_ERASED,
    FUNCTION_SECTOR_NOT_DEFINED,  //û���ҵ�ָ���Ŀ��ַ
    FUNCTION_SECTOR_WRITE_ERR,
    FUNCTION_SECTOR_ERR            //��������ʧ��
}enmFunctionRecturnType;

/*BOOL״̬����,��Զ���ֵ��״̬*/
typedef enum
{
    INEFFECTIVE = 0,/*��Ч*/
    EFFECTIVE
}BOOL_ST;


/*����ָʾ*/
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




/*���ƿ���*/
typedef enum
{
    LAMP_OFF,
    LAMP_ON,
    LAMP_FLASH_TIME
}enmLampFlashStyle;

/*ѡɫ����״̬*/
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
        u16 BreakClutchMVVol :1;/*ɲ����ѹ*/
        u16 BreakClutchLVVol :1;/*ɲ����ѹ*/
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


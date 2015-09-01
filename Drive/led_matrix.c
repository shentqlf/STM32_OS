/*内容：屏上连接电源，P0.0-P0.3连接A、B、C、D，OE使能端一般接VCC或者GND，用IO引脚PWM控制可以调节屏的整体亮度
 */
#include "led_matrix.h"
#include "24L01.h"
#include "delay.h"
#include "stm32f10x.h"
/*
 * PB8 LA
 * PB9 LB
 * PB10 LC
 * PB11 LD
 * PB12 G1
 * PB13 G2
 * PB14 LT
 * PB15 SK
 * PB5 EN
 * PB6 R1
 * PB7 R2
 */
#define LA(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_8):GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define LB(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_9):GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define LC(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_10):GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define LD(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_11):GPIO_ResetBits(GPIOB,GPIO_Pin_11)
#define G1(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_12):GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define G2(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_13):GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define LT(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_14):GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define SK(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_15):GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define EN(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_5):GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define R1(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_6):GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define R2(x) x? GPIO_SetBits(GPIOB,GPIO_Pin_7):GPIO_ResetBits(GPIOB,GPIO_Pin_7)

void led_delay(u8 de)
    {
    while (de--)
	;
    }
void Led_Matrix_GPIO_Init(void)
    {
    GPIO_InitTypeDef GPB;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPB.GPIO_Mode = GPIO_Mode_Out_PP;
    GPB.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8
	    | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13
	    | GPIO_Pin_14 | GPIO_Pin_15;
    GPB.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPB);
    led_delay(100);
    }
/*屏幕缓冲*/
unsigned char LED_Buf[8][32];

extern const unsigned char asc[95][16];
extern const unsigned char hz[95][16];
void LED_putHZ(char num, char str)
    {
    u8 i = 0;
    if (num > 4)
			for (i = 0; i < 16; i++)
					{
//					LED_Buf[num - 8][i + 16] = hz[str - 32][i];
//						LED_Buf[2*num][i] = hz[2*str][2*i];
//						LED_Buf[2*num+1][i] = hz[2*str][2*i+1];
					}
		else
			for (i = 0; i < 16; i++)
					{
						LED_Buf[2*num][i] = hz[2*str][2*i];
						LED_Buf[2*num+1][i] = hz[2*str][2*i+1];
					}
    }
void LED_PutPoint(u16 x, u16 y, u8 color)
    {
			if(color == 0)
			LED_Buf[x/8][y] &= ~(1<<(x%8));
			else
			LED_Buf[x/8][y] |= (1<<(x%8));
    }
//x为横坐标的单个列
//x=0~63;
void MyPutChar(u16 x, char cha)
		{
			u8 i =0,j = 0,y;
			u16 xtemp = x,ytemp = 0;
			for (i = 0; i < 16; i++)
			{
			  LED_PutPoint(xtemp,ytemp,(asc[cha - 32][i])&(0x01));
				xtemp++;
				LED_PutPoint(xtemp,ytemp,(asc[cha - 32][i])&(0x02));
				xtemp++;
				LED_PutPoint(xtemp,ytemp,(asc[cha - 32][i])&(0x04));
				xtemp++;
				LED_PutPoint(xtemp,ytemp,(asc[cha - 32][i])&(0x08));
				xtemp++;
				LED_PutPoint(xtemp,ytemp,(asc[cha - 32][i])&(0x10));
				xtemp++;
				LED_PutPoint(xtemp,ytemp,(asc[cha - 32][i])&(0x20));
				xtemp++;
				LED_PutPoint(xtemp,ytemp,(asc[cha - 32][i])&(0x40));
				xtemp++;
				LED_PutPoint(xtemp,ytemp,(asc[cha - 32][i])&(0x80));
				xtemp = x;
				ytemp++;
			}		
		}
void LED_putchar(char num, char str)
    {
    u8 i = 0;
    if (num > 7)
			for (i = 0; i < 16; i++)
					{
					LED_Buf[num - 8][i + 16] = asc[str - 32][i];
					}
		else
			for (i = 0; i < 16; i++)
					{
					LED_Buf[num][i] = asc[str - 32][i];
					}
    }
void MyPutStr(char num, char *str)
		{
			while (*str)
			MyPutChar(num+=8, *str++);
		}
void LED_putstr(char num, char *str)
    {
			while (*str)
			LED_putchar(num++, *str++);
    }
void DispCarNum(char *carnum)
		{			
			LED_putHZ(0,carnum[0]-48);
			LED_putstr(2,&carnum[1]);
			
		}
void DispRMB(char *RMB)
		{			
			
			LED_putHZ(0,30);
			LED_putstr(2,":");
			LED_putstr(3,RMB);
			LED_putHZ(3,31);
			
		}
//
void DispInternalCar()
		{			
			LED_putHZ(0,32);
			LED_putHZ(1,33);
			LED_putHZ(2,34);
			LED_putHZ(3,35);
			
		}

		
		
u8 tbuf[32];
extern unsigned char LED_Buf[8][32];
void send256(void)
{
	u8 i;
	tbuf[0] = 0x55;
	tbuf[1] = 0xaa;
	NRF24L01_TxPacket(tbuf);
	delay(10);
	for (i = 0; i < 8; i++)
		NRF24L01_TxPacket(LED_Buf[i]);
}


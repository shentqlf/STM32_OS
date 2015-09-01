#ifndef _24L01_H_
#define _24L01_H_
#include "stm32f10x.h"
#include "spi.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01�Ĵ�����������
#define READ_REG1       0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define WRITE_REG1      0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,��״̬�Ĵ���
//�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;
//bit0PRIM_RX:1����ģʽ,0����ģʽ;
//bit1PWR_UP:��ѡ��;
//bit2:0 1byte 1.2byte;
//bit3:CRCʹ��;
//bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��
//bit5:�ж�TX_DS 0ʹ�ܣ�1�ر�;
//bit6:�ж�RX_DR 0ʹ�ܣ�1�رգ�
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��)
//:bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04
//bit3:0 �Զ��ط�����;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;
//bit7: ʹ�ܺ��ز�����ģʽ
//bit[5.3] 00:1M 01:2M 10:250K 11:����
//bit4������
//bit[2:0],���书�� ��111��7db 110��4db 101��3db 011��0db 000��-12db
#define STATUS          0x07  //״̬�Ĵ���;
//bit0:TX FIFO����־;
//bit3:1,��������ͨ����0-5�����ݹܵ�0-5��110�������� 111��rx fifo��;
//bit4,�ﵽ����ط������ж�λ��д1���
//bit5:���ݷ�������ж�;
//bit6:����������Ч��д1���;
#define RSSI            0x09
//bit 0��0С��-60dbm
#define MAX_TX  	0x10  //�ﵽ����ʹ����ж�
#define TX_OK   	0x20  //TX��������ж�
#define RX_OK   	0x40  //���յ������ж�
#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
/*ͨ����ַ*/
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;

#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define FIFO_STATUS     0x17  //FIFO״̬�Ĵ���;
//bit0,RX FIFO�Ĵ����ձ�־ 1��0������;
//bit1,RX FIFO����־1��0��д;
//bit2,3,����
//bit4,TX FIFO�ձ�־ 1��0������;
//bit5,TX FIFO����־ 1��0��д;
//bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TX_ADR_WIDTH    5   //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  //20�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  //20�ֽڵ��û����ݿ��
/************************************************************
 PA12--->IRQ		IN
 PA8---->CE		OUT
 PA11--->CS		OUT
 ************************************************************/
#define CE_CLR GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define CE_SET GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define CS_CLR GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define CS_SET GPIO_SetBits(GPIOA, GPIO_Pin_4)

void
NRF24L01_Init(void);
//void NRF24L01R_Init(void);

u8
NRF24L01_Check(void); //���24L01�Ƿ����

//void Waiting_Mode(void);

void
TX_Mode(void); //����Ϊ����ģʽ
u8
NRF24L01_TxPacket(u8 *txbuf);
void
RX_Mode(void); //����Ϊ����ģʽ
u8
NRF24L01_RxPacket(u8 *rxbuf);
#endif

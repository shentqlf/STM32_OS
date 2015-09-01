#include "24L01.h"
#include "spi.h"
#include "delay.h"
#include "led_matrix.h"
#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//ʹ�õ���2401A
////////////////////////////////////////////////////////////////////////////////
unsigned char TX_ADDRESS[TX_ADR_WIDTH] =
  { 0x34, 0x43, 0x10, 0x10, 0x01 }; //���͵�ַ
unsigned char RX_ADDRESS[RX_ADR_WIDTH] =
  { 0x34, 0x43, 0x10, 0x10, 0x01 }; //���͵�ַ

/************************************************************
 24L01A�������
 PA5---->SCK		SPI
 PA6---->MISO	SPI
 PA7---->MOSI	SPI
 PA2--->IRQ �������ж��źţ��͵�ƽ��Ч
 PA3---->CE оƬ�����źţ�����TX��RXģʽ
 PA4--->CS  оƬƬѡ�ź�
 ************************************************************/
void
NRF24L01_IOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��APB2��GPIOD��ʱ��Դ

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//�ϵ���ʱ100ms����ض�ģʽ

}
u8
Read_IRQ(void)
{
  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
}

void
NRF24L01_Init(void)
{
  SPI1_Configuration();
  NRF24L01_IOInit();
  CE_CLR;
  CS_SET; //�ر�SPI����
}

u8
NRF24L01_Write_Reg(u8 reg, u8 value)
{
  u8 status;
  CS_CLR; //ʹ��SPI����
  status = SPI1_ReadWriteByte(reg); //���ͼĴ�����
  SPI1_ReadWriteByte(value); //д��Ĵ�����ֵ
  CS_SET; //�ر�SPI����
  return (status); //����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8
NRF24L01_Read_Reg(u8 reg)
{
  u8 reg_val;
  CS_CLR; //ʹ��SPI����
  SPI1_ReadWriteByte(reg); //���ͼĴ�����
  reg_val = SPI1_ReadWriteByte(0XFF); //��ȡ�Ĵ�������
  CS_SET; //�ر�SPI����
  return (reg_val); //����״ֵ̬
}
//��ָ��λ��дָ�����ȵ����
//reg:�Ĵ���(λ��)
//*pBuf:���ָ��
//len:��ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8
NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
  u8 status, u8_ctr;
  CS_CLR; //ʹ��SPI����
  status = SPI1_ReadWriteByte(reg); //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
    SPI1_ReadWriteByte(*pBuf++); //д�����
  CS_SET; //�ر�SPI����
  return status; //���ض�����״ֵ̬
}

//��ָ��λ�ö���ָ�����ȵ����
//reg:�Ĵ���(λ��)
//*pBuf:���ָ��
//len:��ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8
NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 len)
{
  u8 status, u8_ctr;
  CS_CLR; //ʹ��SPI����
  status = SPI1_ReadWriteByte(reg); //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for (u8_ctr = 0; u8_ctr < len; u8_ctr++)
    pBuf[u8_ctr] = SPI1_ReadWriteByte(0XFF); //�������
  CS_SET; //�ر�SPI����
  return status; //���ض�����״ֵ̬
}

u8
NRF24L01_Check(void)
{
  u8 buf[5] =
    { 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
  u8 i;
  NRF24L01_Write_Buf(WRITE_REG1 + TX_ADDR, buf, 5); //д��5���ֽڵĵ�ַ.
  NRF24L01_Read_Buf(TX_ADDR, buf, 5); //����д��ĵ�ַ
  for (i = 0; i < 5; i++)
    if (buf[i] != 0XA5)
      break;
  if (i != 5)
    return 1; //���24L01����
  return 0; //��⵽24L01
}

void
TX_Mode(void)
{
  CS_CLR; //ʹ��SPI����
  NRF24L01_Write_Buf(WRITE_REG1 + TX_ADDR, (u8*) TX_ADDRESS, TX_ADR_WIDTH); //дTX�ڵ��ַ
  NRF24L01_Write_Buf(WRITE_REG1 + RX_ADDR_P0, (u8*) RX_ADDRESS, RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK

  NRF24L01_Write_Reg(WRITE_REG1 + EN_AA, 0x01); //ʹ��ͨ��0���Զ�Ӧ��
  NRF24L01_Write_Reg(WRITE_REG1 + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ
  NRF24L01_Write_Reg(WRITE_REG1 + SETUP_RETR, 0x1a); //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  NRF24L01_Write_Reg(WRITE_REG1 + RF_CH, 0x00); //����RFͨ��Ϊ40
  NRF24L01_Write_Reg(WRITE_REG1 + RF_SETUP, 0x07); //����TX�������,0db����,2Mbps,���������濪��
  NRF24L01_Write_Reg(WRITE_REG1 + CONFIG, 0x0e); //���û���ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
  CS_SET; //�ر�SPI����
}

void
Waiting_Mode(void)
{

  CE_CLR;
  NRF24L01_Write_Reg(FLUSH_TX, 0xff); //���TX FIFO�Ĵ���
  NRF24L01_Write_Reg(WRITE_REG1 + CONFIG, 0x0f);
  CS_SET; //�ر�SPI����
}
//����NRF24L01����һ�����
//txbuf:��������׵�ַ
//����ֵ:�������״��
u8
NRF24L01_TxPacket(u8 *txbuf)
{
  u8 sta;
  CE_CLR;
  NRF24L01_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); //д��ݵ�TX BUF  32���ֽ�
  CE_SET; //��������
  while (Read_IRQ() != 0)
    ; //�ȴ������
  sta = NRF24L01_Read_Reg(STATUS); //��ȡ״̬�Ĵ�����ֵ
  NRF24L01_Write_Reg(WRITE_REG1 + STATUS, sta); //Çå³ýTX_DS»òMAX_RTÖÐ¶Ï±êÖ¾
  if (sta & MAX_TX) //�ﵽ����ط�����
    {
      NRF24L01_Write_Reg(FLUSH_TX, 0xff); //���TX FIFO�Ĵ���
      return MAX_TX;
    }
  if (sta & TX_OK) //�������
    {
      return TX_OK;
    }
  return 0xff; //����ԭ����ʧ��
}

void
RX_Mode(void)
{
  CE_CLR;
  NRF24L01_Write_Buf(WRITE_REG1 + RX_ADDR_P0, (u8*) RX_ADDRESS, RX_ADR_WIDTH); //дRX�ڵ��ַ

  NRF24L01_Write_Reg(WRITE_REG1 + EN_AA, 0x01); //ʹ��ͨ��0���Զ�Ӧ��
  NRF24L01_Write_Reg(WRITE_REG1 + EN_RXADDR, 0x01); //ʹ��ͨ��0�Ľ��յ�ַ
  NRF24L01_Write_Reg(WRITE_REG1 + RF_CH, 40); //����RFͨ��Ƶ��
  NRF24L01_Write_Reg(WRITE_REG1 + RX_PW_P0, RX_PLOAD_WIDTH); //ѡ��ͨ��0����Ч��ݿ��
  NRF24L01_Write_Reg(WRITE_REG1 + RF_SETUP, 0x27); //����TX�������,0db����,2Mbps,���������濪��
  NRF24L01_Write_Reg(WRITE_REG1 + CONFIG, 0x0f); //���û���ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ
  CE_SET; //CEΪ��,�������ģʽ
  delay_us(5);
}

//����NRF24L01R����һ�����
//txbuf:��������׵�ַ
//����ֵ:0��������ɣ�����������
u8
NRF24L01_RxPacket(u8 *rxbuf)
{
  u8 sta;
  sta = NRF24L01_Read_Reg(STATUS); //��ȡ״̬�Ĵ�����ֵ
  NRF24L01_Write_Reg(WRITE_REG1 + STATUS, sta); //���TX_DS��MAX_RT�жϱ�־
  if (sta & RX_OK) //���յ����
    {
      NRF24L01_Read_Buf(RD_RX_PLOAD, rxbuf, RX_PLOAD_WIDTH); //��ȡ���
      NRF24L01_Write_Reg(FLUSH_RX, 0xff); //���RX FIFO�Ĵ���
      return 0;
    }
  return 1; //û�յ��κ����
}


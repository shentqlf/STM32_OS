#include "spi.h"
#include "stm32f10x.h"
/*******************************************************************************
 * Function Name  : SPI_ReadWriteByte
 * Description    : SPI��дһ���ֽڣ�������ɺ󷵻ر���ͨѶ��ȡ�����ݣ�
 * Input          : u8 TxData �����͵���
 * Output         : None
 * Return         : u8 RxData �յ�����
 *******************************************************************************/
void
SPI1_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
  RCC_APB2PeriphClockCmd(
      RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); //ʹ��SPI1ʱ��
  /*  ����SPI1�ĸ���GPIO  */
  //PA5--CLK
  //PA6--MISO
  //PA7--MOSI  �����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  SPI_I2S_DeInit(SPI1);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ����ģʽ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //����ģʽ��SCKΪ1
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //���ݲ����ӵ�2��ʱ����ؿ�ʼ
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; //������
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //���ģʽ
  SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC����ʽ
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //����ģʽ
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);
} //SPI1_Config()

u8
SPI1_ReadWriteByte(u8 TxData)
{
  while ((SPI1->SR & SPI_I2S_FLAG_TXE) == RESET)
    ;
  SPI1->DR = TxData;
  while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == RESET)
    ;
  return (SPI1->DR);
}


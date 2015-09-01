#include "spi.h"
#include "stm32f10x.h"
/*******************************************************************************
 * Function Name  : SPI_ReadWriteByte
 * Description    : SPI读写一个字节（发送完成后返回本次通讯读取的数据）
 * Input          : u8 TxData 待发送的数
 * Output         : None
 * Return         : u8 RxData 收到的数
 *******************************************************************************/
void
SPI1_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
  RCC_APB2PeriphClockCmd(
      RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); //使能SPI1时钟
  /*  配置SPI1的复用GPIO  */
  //PA5--CLK
  //PA6--MISO
  //PA7--MOSI  复用推挽输出
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  SPI_I2S_DeInit(SPI1);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //空闲模式下SCK为1
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //数据采样从第2个时间边沿开始
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS软件管理
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; //波特率
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //大端模式
  SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主机模式
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


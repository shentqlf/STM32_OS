#include "adc.h"

u16  AD_Value[N][CH];   //�������ADCת�������Ҳ��DMA��Ŀ���ַ
u16  After_filter[CH];    //���������ƽ��ֵ֮��Ľ��
int	 i;

/*GPIO�ܽŵ�����
ѡ��ADC��ͨ��0  1  2  8  9  10  11  12  13  14  15���ֱ��Ӧ�Ĺܽ�ΪPA0  PA1  PA2  PB0  PB1  PC0  PC1  PC2  PC3  PC4  PC5
����ʹ��USART1����TXΪPA9��RXΪPA10 */
void GPIO_Configuration(void)
	{
	GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	}

void RCC_Configuration(void)
	{

     /*ʹ�ܸ�������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
	     |RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE );	  //ʹ��ADC1ͨ��ʱ�ӣ������ܽ�ʱ��
	/* Configure ADCCLK such as ADCCLK = PCLK2/6 */ 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC���ʱ�䲻�ܳ���14M
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	 
	}
  

/*����ADC1*/
void ADC1_Configuration(void)
    {
	 ADC_InitTypeDef  ADC_InitStructure;

	ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode =ENABLE;	//ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ����ת���ر�
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = CH;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
	
	/* ADC1 regular channel11 configuration */ 
	//����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����

	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_239Cycles5 );		
	
	 // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_DMACmd(ADC1, ENABLE);	
	
	/* Enable ADC1 */
	 ADC_Cmd(ADC1, ENABLE);	   //ʹ��ָ����ADC1
	 /* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);	  //��λָ����ADC1��У׼�Ĵ���
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�
	
	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);		//��ʼָ��ADC1��У׼״̬
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�
	
	
}



/*����DMA*/
void DMA_Configuration(void)
	{
	/* ADC1  DMA1 Channel Config */
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
	DMA_InitStructure.DMA_BufferSize = CH*N;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

	}


//������������
void Init_ADC1(void)
	{
	
	RCC_Configuration();	
		
	GPIO_Configuration();

	ADC1_Configuration();

	DMA_Configuration();
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		DMA_Cmd(DMA1_Channel1, ENABLE);	 //����DMAͨ��

	 
	}
	///*��ƽ��ֵ����*/
void filter(void)
{
 	u32  sum = 0;
	u8  count;    
 	for(i=0;i<CH;i++)

     {

        for ( count=0;count<N;count++)

         {

          sum += AD_Value[count][i];

         }

         After_filter[i]=sum/N;

         sum=0;
     }

}
/*��ȡADC��ֵ���������ƻ���Ϊʮ����*/
//0 1
//4 5
//v i
u16 GetVolt(u16 chx)   
  { 
   return (u16)(After_filter[chx]*3330/4096);   //��Ľ��������100���������������С��
  }

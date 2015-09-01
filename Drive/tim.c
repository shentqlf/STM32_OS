#include "tim.h"
/***********************************************************
TIM2
CH1-4:PA15,PB3,PB10,PB11
***********************************************************/

void TIM2_RCC_Configuration(void)
{
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                         RCC_APB2Periph_AFIO, ENABLE);}

void TIM2_GPIO_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;

 /* GPIOA Configuration:TIM3 Channel1, 2, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t TimerPeriod = 0;
	uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;


	/* Compute the value to be set in ARR regiter to generate signal frequency at 100 Khz */
  TimerPeriod = (SystemCoreClock / 100000 ) - 1;
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
  Channel1Pulse = (uint16_t) (((uint32_t) 5000 * (TimerPeriod - 1)) / 10000);
  /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
  Channel2Pulse = (uint16_t) (((uint32_t) 5000 * (TimerPeriod - 1)) / 10000);
  /* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
  Channel3Pulse = (uint16_t) (((uint32_t) 5000 * (TimerPeriod - 1)) / 10000);
  /* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
  Channel4Pulse = (uint16_t) (((uint32_t) 5000 * (TimerPeriod- 1)) / 10000);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;

  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);

  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);

  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM2, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}
void TIM2PulseCfg(u8 ch,u8 pulse)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t TimerPeriod = 0;
	uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;


	/* Compute the value to be set in ARR regiter to generate signal frequency at 100 Khz */
  TimerPeriod = (SystemCoreClock / 100000 ) - 1;
	  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);


	switch(ch)
	{
		case 3: 
					{
						/* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
						Channel3Pulse = (uint16_t) (((uint32_t) pulse*100 * (TimerPeriod - 1)) / 10000);
						/* PWM1 Mode configuration: Channel3 */
						TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
						TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
						TIM_OC3Init(TIM2, &TIM_OCInitStructure);
						TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
					  TIM_ARRPreloadConfig(TIM2, ENABLE);
						  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);

						break;
					}
		case 4: 
					{
						/* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
						Channel4Pulse = (uint16_t) (((uint32_t) pulse*100  * (TimerPeriod- 1)) / 10000);
						/* PWM1 Mode configuration: Channel4 */
						TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
						TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
						TIM_OC4Init(TIM2, &TIM_OCInitStructure);
						TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
						TIM_ARRPreloadConfig(TIM2, ENABLE);
						  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);
						break;


					}

	}

}
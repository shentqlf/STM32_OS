#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

#define  N   50	     //ÿͨ����50��
#define  CH  2	     //Ϊ12��ͨ��

void Init_ADC1(void);
u16 GetVolt(u16 advalue);  
void filter(void);


#endif /* __ADC_H */


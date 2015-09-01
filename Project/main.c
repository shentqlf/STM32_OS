#include "os.h"
#include "uart1.h"
#include "stdio.h"
#include "delay.h"

#define TASK_1_STK_SIZE 128
#define TASK_2_STK_SIZE 128
#define TASK_3_STK_SIZE 128
#define TASK_4_STK_SIZE 128

static STACK_TypeDef TASK_1_STK[TASK_1_STK_SIZE];
static STACK_TypeDef TASK_2_STK[TASK_2_STK_SIZE];
static STACK_TypeDef TASK_3_STK[TASK_1_STK_SIZE];
static STACK_TypeDef TASK_4_STK[TASK_2_STK_SIZE];

#define TASK1_PRIO 0
#define TASK2_PRIO 1
#define TASK3_PRIO 2
#define TASK4_PRIO 3

float cpu;
INT16U mem;
void task_1()
{
	while(1)
	{
		cpu = OS_GetCPU();
		mem = OS_GetStackMaxUsage(TASK_1_STK,TASK_1_STK_SIZE);
		OS_ENTER_CRITICAL();
		printf("cpu = %0.1f%%\r\n",cpu);
		printf("mem = %02d%%\r\n",mem);
		printf("Task 1 Running!!!\r\n");
		OS_EXIT_CRITICAL();
		OS_TimeDelay(1000);
	}
}
		u8 task2count = 0;
void task_2()
{
  while(1)
	{
		task2count++;
		OS_ENTER_CRITICAL();
			printf("Task 2 Running,count = %d!!!\r\n",task2count);
		OS_EXIT_CRITICAL();
		delay(1000);
		if(task2count==3)
		{
			OS_TaskSuspend(TASK2_PRIO);
			task2count = 0;
		}
		OS_TimeDelay(1000);
	}

}
void task_3()
{
  while(1)
	{
		OS_ENTER_CRITICAL();
			printf("Task 3 Running!!!\r\n");
		OS_EXIT_CRITICAL();
		if(TCB[TASK2_PRIO].State == TASK_SUSPEND)
		{
			printf("任务2挂起\r\n");
			OS_TaskResume(1);
			printf("任务2重新启动\r\n");
		}
		
		delay(100);
		OS_TimeDelay(1000);
	}

}
void task_4()
{
  while(1)
	{
		OS_ENTER_CRITICAL();
  //printf("Task 4 Running!!!\r\n");
		OS_EXIT_CRITICAL();
		delay(100);
		OS_TimeDelay(1000);
	}

}





int main()
{
  usart1_init();
  

	OS_Init();
	OS_TaskCreate(task_1,&TASK_1_STK[TASK_1_STK_SIZE-1],TASK1_PRIO);
	OS_TaskCreate(task_2,&TASK_2_STK[TASK_2_STK_SIZE-1],TASK2_PRIO);
	OS_TaskCreate(task_3,&TASK_3_STK[TASK_3_STK_SIZE-1],TASK3_PRIO);
	OS_TaskCreate(task_4,&TASK_4_STK[TASK_4_STK_SIZE-1],TASK4_PRIO);
  OS_Start();
  
  return 0;
}

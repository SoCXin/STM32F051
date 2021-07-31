/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-November-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "os.h"

/** @addtogroup STM32F0xx_StdPeriph_Templates
  * @{
  */

#define STARTUP_TASK_STK_SIZE 			128
#define TASK1_STK_SIZE 							128
#define TASK2_STK_SIZE 							128

#define STARTUP_TASK_PRIO 4
#define TASK1_PRIO 5
#define TASK2_PRIO 6


static volatile OS_ERR err;

static void task1(void *p_arg);
static void task2(void *p_arg);

void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |  GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, GPIO_Pin_8);
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
}

static OS_TCB task1TCB;
static CPU_STK task1_stk[TASK1_STK_SIZE];
static void task1(void *p_arg)
{

    while (1)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_8);
        OSTimeDly(    (OS_TICK    )500, 
                    (OS_OPT        )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);

        GPIO_SetBits(GPIOC, GPIO_Pin_8);
        OSTimeDly(    (OS_TICK    )500, 
                    (OS_OPT        )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
    }
}

static OS_TCB task2TCB;
static CPU_STK task2_stk[TASK2_STK_SIZE];
static void task2(void *p_arg)
{
    while (1)
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_9);
        OSTimeDly(    (OS_TICK    )500, 
                    (OS_OPT        )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);

        GPIO_SetBits(GPIOC, GPIO_Pin_9);
        OSTimeDly(    (OS_TICK    )500, 
                    (OS_OPT        )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
        
    }
}

static OS_TCB taskStartTCB;
static CPU_STK startTaskStk[STARTUP_TASK_STK_SIZE];
void StartTask(void)
{
    
    led_init();
   OS_CPU_SysTickInit(SystemCoreClock / 1000);
    OSTaskCreate(    (OS_TCB     *)&task1TCB,
                    (CPU_CHAR     *)"task1",
                    (OS_TASK_PTR)task1,
                    (void         *)0,
                    (OS_PRIO     )TASK1_PRIO,
                    (CPU_STK    *)&task1_stk[0],
                    (CPU_STK_SIZE)TASK1_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK1_STK_SIZE,
                    (OS_MSG_QTY    )0,
                    (OS_TICK    )0,
                    (void        *)0,
                    (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR        *)&err);
	
    OSTaskCreate(    (OS_TCB     *)&task2TCB,
                    (CPU_CHAR     *)"task2",
                    (OS_TASK_PTR)task2,
                    (void         *)0,
                    (OS_PRIO    ) TASK2_PRIO,
                    (CPU_STK    *)&task2_stk[0],
                    (CPU_STK_SIZE)TASK2_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK2_STK_SIZE,
                    (OS_MSG_QTY)0,
                    (OS_TICK    )0,
                    (void        *)0,
                    (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR        *)&err);    

                    
    
   OSTaskDel(        (OS_TCB     *)&taskStartTCB, 
                     (OS_ERR     *)&err);
}



int main(void)
{
		OS_ERR  err;
       
    CPU_Init();
    OSInit((OS_ERR *)&err);    
    
    OSTaskCreate(    (OS_TCB     *)&taskStartTCB,
                    (CPU_CHAR     *)"task_start",
                    (OS_TASK_PTR)StartTask,
                    (void         *)0,
                    (OS_PRIO    ) STARTUP_TASK_PRIO,
                    (CPU_STK    *)&startTaskStk[0],
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE,
                    (OS_MSG_QTY)0,
                    (OS_TICK    )0,
                    (void        *)0,
                    (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR        *)&err);
                    
    OSStart((OS_ERR *)&err);

}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

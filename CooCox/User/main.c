/***************************************************************************//**
  * @file    main.c
  * @author  
  * @version V1.0.0
  * @date    
  * @brief   CooCox主程序
  ******************************************************************************

/* 包含的头文件---------------------------------------------------------------*/
#include "stm32f0xx.h"
#include <CoOS.h>			 			/*!< CooCox RTOS header file.		  */

/* 私有类型定义---------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define TASK_STK_SIZE		128	 		/*!< Define stack size.				  */
#define LED3            GPIO_Pin_9
#define LED4            GPIO_Pin_8

#define LED_PORT        GPIOC
#define LED_GPIO_CLK    RCC_AHBPeriph_GPIOC

/* 私有变量 ------------------------------------------------------------------*/
OS_STK   LEDA_Task_Stk[TASK_STK_SIZE];	 	  /*!< Stack of 'LEDA_Task' task. */
OS_STK   LEDB_Task_Stk[TASK_STK_SIZE];	 	  /*!< Stack of 'LEDB_Task' task. */

/* 私有函数 ------------------------------------------------------------------*/
void LED_Configuration(void);
void LEDA_Task (void* pdata) ;
void LEDB_Task (void* pdata) ;

/***************************************************************************//**
  * @brief  主函数，硬件初始化，实现LED1-LED4闪烁
  * @note   无
  * @param  无
  * @retval 无
*******************************************************************************/
int main(void)
{
    CoInitOS();							/*!< Initial CooCox RTOS 			  */

    CoCreateTask(LEDA_Task, (void *)0, 10,&LEDA_Task_Stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
    CoCreateTask(LEDB_Task, (void *)0, 11,&LEDB_Task_Stk[TASK_STK_SIZE-1], TASK_STK_SIZE);
    
    CoStartOS();

    while (1)
    {
    }
}

/***************************************************************************//**
 * @brief   闪灯任务A
 * @param   无
 * @retval  无
 * @par     详细描述：
 * @details 创建 任务A，该任务以200ms为周期点亮LED3，实现LED3闪烁。
*******************************************************************************/
void LEDA_Task (void* pdata) 
{
    //初始化LED
    LED_Configuration();

    for (;;) 
    {
        GPIO_SetBits(LED_PORT,LED3);  //点亮LED1
        //延时100 ms
        CoTickDelay (100);
        GPIO_ResetBits(LED_PORT,LED3);//熄灭LED1
        //延时100 ms
        CoTickDelay (100);
    }
}

/***************************************************************************//**
 * @brief   闪灯任务B
 * @param   无
 * @retval  无
 * @par     详细描述：
 * @details 创建 任务B，该任务以400ms为周期点亮LED4，实现LED4闪烁。
*******************************************************************************/
void LEDB_Task (void* pdata) 
{
    //初始化LED
    LED_Configuration();

    for (;;) 
    {
        GPIO_SetBits(LED_PORT,LED4);  //点亮LED2
        //延时200 ms
        CoTickDelay (200);
        GPIO_ResetBits(LED_PORT,LED4);//熄灭LED2
        //延时200 ms
        CoTickDelay (200);
    }
}

/***************************************************************************//**
  * @brief  配置使用LED
  * @note   LED相关的定义需要根据需求用宏定义来修改
  * @param  无
  * @retval 无
*******************************************************************************/
void LED_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //使能LED所在GPIO的时钟
    RCC_AHBPeriphClockCmd(LED_GPIO_CLK, ENABLE);

    //初始化LED的GPIO
    GPIO_InitStructure.GPIO_Pin = LED3 | LED4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(LED_PORT,LED3 | LED4);  //熄灭LED3-4
}


#ifdef  USE_FULL_ASSERT
/***************************************************************************//**
  * @brief  报告在检查参数发生错误时的源文件名和错误行数
  * @param  file: 指向错误文件的源文件名
  * @param  line: 错误的源代码所在行数
  * @retval 无
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
    /* 用户可以增加自己的代码用于报告错误的文件名和所在行数,
       例如：printf("错误参数值: 文件名 %s 在 %d行\r\n", file, line) */

    /* 死循环 */
    while (1)
    {
    }
}
#endif

/******************* (C) COPYRIGHT wuguoyana ***************文件结束***********/

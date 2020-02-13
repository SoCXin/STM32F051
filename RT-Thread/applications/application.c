/***************************************************************************//**
  * @file    application.c
  * @author  吴国炎
  * @version V1.0.0
  * @date    2012年6月1号
  * @brief   This file is part of RT-Thread RTOS
  ******************************************************************************
  @verbatim
  主程序
  吴国炎制作
  技术博客网址：http://hi.baidu.com/wuguoyana/home
  @endverbatim
  * @attention
  *
  * 郑重声明：
  * 此文件只用于提供开发参考，如果因此文件而产生的任何问题纠纷，我概不负责。
  *
  * <h2><center> &copy; COPYRIGHT 2011 wuguoyana </center></h2>
*******************************************************************************/

/* 包含的头文件---------------------------------------------------------------*/
#include <stdio.h>

#include <board.h>
#include <rtthread.h>

/* 私有类型定义---------------------------------------------------------------*/
/* 私有定义 ------------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define rt_hw_led1_on()   GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define rt_hw_led1_off()  GPIO_ResetBits(GPIOC, GPIO_Pin_9)

#define rt_hw_led2_on()   GPIO_SetBits(GPIOC, GPIO_Pin_8)
#define rt_hw_led2_off()  GPIO_ResetBits(GPIOC, GPIO_Pin_8)
/* 私有变量 ------------------------------------------------------------------*/
ALIGN(RT_ALIGN_SIZE)
static char led1_stack[384];
static struct rt_thread led1_thread;

static char led2_stack[384];
static struct rt_thread led2_thread;
/* 私有函数 ------------------------------------------------------------------*/

/***************************************************************************//**
  * @brief  led硬件初始
  * @note   无
  * @param  无
  * @retval 无
*******************************************************************************/
static void rt_hw_led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    //使能LED的GPIO时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    //配置GPIO引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/***************************************************************************//**
  * @brief  led1任务函数入口
  * @note   无
  * @param  无
  * @retval 无
*******************************************************************************/
static void led1_thread_entry(void* parameter)
{
    rt_hw_led_init();

    while (1)
    {
        rt_hw_led1_on();
        rt_thread_delay(RT_TICK_PER_SECOND/4);

        rt_hw_led1_off();
        rt_thread_delay(RT_TICK_PER_SECOND/4);
    }
}

/***************************************************************************//**
  * @brief  led2任务函数入口
  * @note   无
  * @param  无
  * @retval 无
*******************************************************************************/
static void led2_thread_entry(void* parameter)
{
    while (1)
    {
        rt_hw_led2_on();
        rt_thread_delay(RT_TICK_PER_SECOND/4);

        rt_hw_led2_off();
        rt_thread_delay(RT_TICK_PER_SECOND/4);
    }
}
/***************************************************************************//**
  * @brief  应用初始化任务
  * @note   无
  * @param  无
  * @retval 无
*******************************************************************************/
int rt_application_init()
{
    rt_err_t result;

    result = rt_thread_init(&led1_thread,
                            "led1",
                            led1_thread_entry,
                            RT_NULL,
                            &led1_stack[0],
                            sizeof(led1_stack),
                            4,
                            2);

    if (result == RT_EOK)
        rt_thread_startup(&led1_thread);

    result = rt_thread_init(&led2_thread,
                            "led2",
                            led2_thread_entry,
                            RT_NULL,
                            &led2_stack[0],
                            sizeof(led2_stack),
                            5,
                            2);

    if (result == RT_EOK)
        rt_thread_startup(&led2_thread);

    return 0;
}


/******************* (C) COPYRIGHT wuguoyana ***************文件结束***********/

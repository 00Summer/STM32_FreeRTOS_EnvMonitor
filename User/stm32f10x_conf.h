#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

/* Includes ------------------------------------------------------------------*/
/* 下面这些是你开启的底层硬件驱动库头文件,由keil自动配置标准库，注释调不用模块的原因是加快编译速度*/
#include "stm32f10x_rcc.h"   // 时钟控制（必须有）
#include "stm32f10x_gpio.h"  // 引脚控制（必须有）
#include "stm32f10x_usart.h" // 串口通信（咱们现在的代码用到了）
#include "stm32f10x_exti.h"  // 外部中断（以后按键会用到）
#include "misc.h"            // NVIC中断控制器（必须有）
#include "stm32f10x_fsmc.h" 

// 如果你以后还要用到别的外设（比如定时器、I2C），就在这里继续 #include 进来
// #include "stm32f10x_tim.h"
// #include "stm32f10x_i2c.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM32F10x_CONF_H */

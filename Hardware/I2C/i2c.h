#ifndef _I2C_H
#define _I2C_H

#include"stm32f10x.h"
//宏定义 SCL和SDA，方便后续函数开关SCL和SDA的引脚。
#define I2C_SCL(x)  (x ? GPIO_SetBits(GPIOA, GPIO_Pin_6) : GPIO_ResetBits(GPIOA, GPIO_Pin_6))
#define I2C_SDA(x)  (x ? GPIO_SetBits(GPIOA, GPIO_Pin_7) : GPIO_ResetBits(GPIOA, GPIO_Pin_7))
//宏定义 读取SDA的电平
#define READ_SDA GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);

void I2C_Send_Byte(uint8_t st);
uint8_t I2C_Read_Byte(uint8_t ack);

uint8_t I2C_Wait_Ack(void);
void I2C_Ack(void);
void I2C_NAck(void);

void I2C_Ping_Test(void);

#endif

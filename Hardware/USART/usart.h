#ifndef _USART_H
#define _USART_H

#include"stm32f10x.h"//""代表在工程文件夹中寻找；stm32f103所有的引脚、时钟、寄存器等地址。
#include<stdio.h>//<>代表在keil安装目录中寻找；C语言标准输入输出库，后面要用到printf。

void USART1_Init(uint32_t bound);//uint32_t 就是typedef unsigned int。
//_t意思是typedef。32为int，16为short int，8为char。

#endif

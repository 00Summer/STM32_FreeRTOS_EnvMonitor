#include "usart.h"

void USART1_Init(uint32_t bound){
    //命名两个结构体类型
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;

    //1.设置时钟，即通电。APB2是高速时钟总线，USART1被挂在APB2，但是USART2被挂在APB1。
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    //2.配置GPIO参数，并开启PA9(TX发送引脚)和PA10（RX接收引脚）。
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出：PA9是ODR和TX复用的。
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入：浮空导线，随着电脑发来的高低电平变化。
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //3.配置USART参数，并开启USART1
    USART_InitStruct.USART_BaudRate = bound;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1,&USART_InitStruct);

    //4.配置表设置好USART_InitStruct.XX，配入寄存器USART_Init，开启总闸USART_Cmd。
    USART_Cmd(USART1, ENABLE);
}

//printf重定向。因为printf是为电脑设计的，单片机没有显卡和显示器，printf无法打印，改成发送单个字符。
int fputc(int ch, FILE *f) {
    //字符ch传入USART1的数据寄存器DR中
    USART_SendData(USART1, (uint8_t)ch); 
    //等待发送完成。因为CPU速度太快，防止DR被快速覆盖
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
    return ch;
}

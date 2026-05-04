#include"stm32f10x.h"
#include"i2c.h"
#include"delay.h"
#include"usart.h"
//I2C的标准通讯速率为10Khz，也就是10us，半个时钟周期为us。
#define I2C_Delay() delay_us(5)

void I2C_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    //开启GPIOA的时钟。
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    //配置GPIO引脚、模式、速度，并写入寄存器。
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&GPIO_InitStruct);
    //设置SCL和SDA为高电平，开漏模式下代表高阻抗。
    I2C_SCL(1);
    I2C_SDA(1);
}

void I2C_Start(void){
    //防御性编程，重复初始条件
    I2C_SCL(1);
    I2C_SDA(1);

    I2C_Delay();
    I2C_SDA(0);//从1到0，制造下降沿。
    I2C_Delay();
    I2C_SCL(0);//收尾。SCL低电平，准备发送数据。
}

void I2C_Stop(void){
    //防御性编程，重复初始条件
    I2C_SCL(0);
    I2C_SDA(0);
    I2C_Delay();

    I2C_SCL(1);
    I2C_Delay();
    I2C_SDA(1);//从0到1，制造上升沿。
    I2C_Delay();
    
    I2C_SCL(1);//收尾。SCL高电平，停止发送数据。
}

//单片机作为发送方，读取从机是否应答
uint8_t I2C_Wait_Ack(void){
    uint8_t ucErrTime = 0; // 定义一个超时计数器
    I2C_SDA(1);I2C_Delay();//主机释放SDA
    I2C_SCL(1);I2C_Delay();//拉高SCL，保证数据有效

    //反复抓取SDA引脚电平250次
    while(READ_SDA){
        ucErrTime++;
        if(ucErrTime > 250){
            I2C_Stop(); // 超时了，放弃等待，释放总线
            return 1;   // 返回 1，表示彻底失败 (NACK)
        }
    }

    //读取成功，拉低时钟线。
    I2C_SCL(0);
    return 0;
}

//单片机作为接受方的应答
void I2C_Ack(void) {
    //释放时钟线，让数据线可变。
    I2C_SCL(0);

    //拉低数据线，表示应答
    I2C_SDA(0);
    I2C_Delay();

    //制造第9个脉冲，让发送发读取
    I2C_SCL(1);
    I2C_Delay();

    //应答成功，继续接受下一个字符
    I2C_SCL(0);
}

//单片机作为接受方的非应答
void I2C_NAck(void) {
    //释放时钟线，让数据线可变。
    I2C_SCL(0);

    //拉高数据线，表示非应答
    I2C_SDA(1);
    I2C_Delay();

    //制造第9个脉冲，让发送方读取
    I2C_SCL(1);
    I2C_Delay();

    //应答成功，继续接受下一个字符
    I2C_SCL(0);
}

//I2C发送一个字节
void I2C_Send_Byte(uint8_t txd) {
    uint8_t t;   
    I2C_SCL(0); // 拉低时钟开始数据传输

    //将数据由高位到低位，全部发送出去
    for(t=0; t<8; t++) {
        I2C_SDA((txd & 0x80) >> 7);
        txd <<= 1;
        I2C_Delay();//确保SDA反转稳定

        I2C_SCL(1);//保证数据有效性
        I2C_Delay();//给足存储时间
        I2C_SCL(0);//传输正式结束
        I2C_Delay();
    }
}

// I2C 读取一个字节
uint8_t I2C_Read_Byte(uint8_t ack) {
    uint8_t i, receive = 0;
    I2C_SDA(1); // 释放SDA线，准备读取
    for(i=0; i<8; i++ ) {
        I2C_SCL(0);
        I2C_Delay();
        I2C_SCL(1);
        receive <<= 1;
        if(READ_SDA) receive++;   
        I2C_Delay(); 
    }
    if (!ack) I2C_NAck(); // 不继续读数据，发送nACK
    else I2C_Ack();       // 继续读数据，发送ACK
    return receive;
}


void I2C_Ping_Test(void){
    I2C_Start();
    //SHT20的初始默认地址为7位的0x40，需要补齐一位R/W位，写操作补0
    I2C_Send_Byte(0x40<<1 | 0x00);
    uint8_t ack_status = I2C_Wait_Ack();
    I2C_Stop();

    if(ack_status == 0){
        printf("I2C Ping Successing,SHT20 is working");
    }else{
        printf("I2C Ping failed");
    }
}


#include"sht20.h"
#include"i2c.h"
#include"delay.h"

// 引入 FreeRTOS 核心组件
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//SHT20的读写地址
#define SHT20_ADDRESS_W 0x80
#define SHT20_ADDRESS_R 0x81

//// 核心优化：采用 FreeRTOS 非阻塞式延时
#define SHT20_Delay() vTaskDelay(pdMS_TO_TICKS(85))

extern SemaphoreHandle_t I2C_Mutex;


void SHT20_Init(void){
    I2C_Init();
}

float SHT20_Read_Temp(void){
    uint16_t temp_raw;

    //// ======= 第一阶段：申请总线发指令 =======
    //获取I2C锁，被占用就死等
    xSemaphoreTake(I2C_Mutex,portMAX_DELAY);

    //发送指令
    I2C_Start();
    I2C_Send_Byte(SHT20_ADDRESS_W);//主机通过I2C向SHT20发送写指令
    I2C_Wait_Ack();
    I2C_Send_Byte(0Xf3);//非主机测量温度
    I2C_Wait_Ack();
    I2C_Stop();

    //指令发完后，释放锁，无需占用总线
    xSemaphoreGive(I2C_Mutex);
    //挂起任务，等待转换
    SHT20_Delay();

    // ======= 第二阶段：重新申请总线读数据 =======
    //读取数据
    I2C_Start();
    I2C_Send_Byte(SHT20_ADDRESS_R);
    I2C_Wait_Ack();

    temp_raw = I2C_Read_Byte(1);//读高8位，回复ack
    temp_raw <<= 8;//左移8位，继续低8位，回复NACK
    temp_raw |= I2C_Read_Byte(0);
    I2C_Stop();

    //数据清洗
    temp_raw &= ~0x0003;//清除状态位，最后两位置0；
    float temp = -46.85 + 175.72 * ((float)temp_raw / 65536.0);
    return temp;
}

float SHT20_Read_Hum(void){
    uint16_t hum_raw;

    
    xSemaphoreTake(I2C_Mutex, portMAX_DELAY);//加锁
    //1.发送指令
    I2C_Start();
    I2C_Send_Byte(SHT20_ADDRESS_W);//主机通过I2C向SHT20发送写指令
    I2C_Wait_Ack();
    I2C_Send_Byte(0Xf5);//非主机测量温度
    I2C_Wait_Ack();
    I2C_Stop();
    xSemaphoreGive(I2C_Mutex);//释放锁

    //2.等待转换
    SHT20_Delay();

    //3.读取数据
    xSemaphoreTake(I2C_Mutex, portMAX_DELAY);
    I2C_Start();
    I2C_Send_Byte(SHT20_ADDRESS_R);
    I2C_Wait_Ack();

    hum_raw = I2C_Read_Byte(1);//读高8位，回复ack
    hum_raw <<= 8;//左移8位，继续低8位，回复NACK
    hum_raw |= I2C_Read_Byte(0);
    I2C_Stop();
    xSemaphoreGive(I2C_Mutex);

    //4.数据清洗
    hum_raw &= ~0x0003;//清除状态位，最后两位置0；
    float hum = -6.0 + 125.0 * ((float)hum_raw / 65536.0);
    return hum;
}


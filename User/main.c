//STM32核心组件
#include "stm32f10x.h"
#include "usart.h"
#include "i2c.h"
#include "sht20.h"
#include "delay.h"
#include "lcd.h"
#include <stdio.h>

//FreeRTOS核心组件
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//声明互斥锁与SHT20的句柄
SemaphoreHandle_t printf_Mutex;
SemaphoreHandle_t I2C_Mutex;
TaskHandle_t SHT20_Task_Handle;

// 浮点数转字符串函数,以解决sprinf占用大量内存(保留两位小数)
// 参数：num为要转换的浮点数, str为存放结果的字符数组
void Float_To_Str(float num, char *str) {
    int pos = 0;
    
    // 1. 处理负数
    if (num < 0) {
        str[pos++] = '-';
        num = -num;
    }
    
    // 2. 提取整数部分和小数部分
    int int_part = (int)num; // 整数部分 (如 26)
    int dec_part = (int)((num - int_part) * 100 + 0.5f); // 小数部分，+0.5f是为了四舍五入
    
    // 3. 转换十位和个位 (假设温湿度不会超过百位数，如超过可加百位逻辑)
    str[pos++] = (int_part / 10) + '0'; // 十位转 ASCII
    str[pos++] = (int_part % 10) + '0'; // 个位转 ASCII
    
    // 4. 加上小数点
    str[pos++] = '.';
    
    // 5. 转换小数第一位和第二位
    str[pos++] = (dec_part / 10) + '0'; 
    str[pos++] = (dec_part % 10) + '0'; 
    
    // 6. 加上结束符
    str[pos] = '\0'; 
}

void SHT20_Task(void *pvParameters){
    float temp,humi;
    char temp_str[10];
    char humi_str[10];

    // ======= 静态 UI 框架提前渲染 =======
    LCD_ShowString(30, 50, (uint8_t *)"FreeRTOS Env Monitor", RED, WHITE, 16, 0);
    LCD_ShowString(30, 90, (uint8_t *)"Temp: ", BLUE, WHITE, 16, 0);
    LCD_ShowString(30, 110, (uint8_t *)"Humi: ", BLUE, WHITE, 16, 0);

    while(1){
        temp = SHT20_Read_Temp();
        humi = SHT20_Read_Hum();

        //串口打印（受锁保护）
        xSemaphoreTake(printf_Mutex,portMAX_DELAY);
        printf("Temp:%.2f C,Humi:%.2f %%\r\n",temp,humi);
        xSemaphoreGive(printf_Mutex);

        // ========== LCD 屏幕刷新部分=======
        //温度显示
        Float_To_Str(temp, temp_str); 
        LCD_ShowString(78, 90, (uint8_t *)temp_str, BLUE, WHITE, 16, 0);
        LCD_ShowString(118, 90, (uint8_t *)" C", BLUE, WHITE, 16, 0);//单位摄氏度C
        //湿度显示
        Float_To_Str(humi, humi_str); 
        LCD_ShowString(78, 110, (uint8_t *)humi_str, BLUE, WHITE, 16, 0);
        LCD_ShowString(118, 110, (uint8_t *)" %RH", BLUE, WHITE, 16, 0);

        //让出CPU1000ms
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main(){
    //初始化
    delay_init();
    USART1_Init(115200);
    LCD_Init();
    printf("LCD Init OK!\r\n");
    SHT20_Init();

    //创建互斥信号量。即长度为1，数据项为0的队列。
    I2C_Mutex = xSemaphoreCreateMutex();
    printf_Mutex = xSemaphoreCreateMutex();

    xTaskCreate(SHT20_Task,"SHT20_Task",128,NULL,1,&SHT20_Task_Handle);

    vTaskStartScheduler();

    //防止调度器分配内存失败后，指针跑飞导致死机。
    while(1);
    
}

// FreeRTOS 堆栈溢出函数
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    pcTaskName = "StackOverflow";
    // 内存爆了，跳进死循环，并显示错误名
    printf("Fatal Error: Task [%s] Stack Overflow!\r\n", pcTaskName);
    while(1); 
}



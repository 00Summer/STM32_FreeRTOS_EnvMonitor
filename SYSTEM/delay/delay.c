#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"

// DWT 寄存器地址宏定义
#define DWT_CR      *(volatile uint32_t *)0xE0001000
#define DWT_CYCCNT  *(volatile uint32_t *)0xE0001004
#define DEM_CR      *(volatile uint32_t *)0xE000EDFC
#define DEM_CR_TRCENA       (1 << 24)
#define DWT_CR_CYCCNTENA    (1 << 0)

static uint32_t cpu_freq_mhz = 0;

void delay_init(void) {
    cpu_freq_mhz = SystemCoreClock / 1000000; // 记录当前主频 
    
    // 开启 DWT 周期计数器
    DEM_CR |= DEM_CR_TRCENA; 
    DWT_CYCCNT = 0;           // 计数器清零
    DWT_CR |= DWT_CR_CYCCNTENA; // 启动计数器
}

// 微秒级延时 
void delay_us(uint32_t nus) {
    uint32_t ticks = nus * cpu_freq_mhz;
    uint32_t t0 = DWT_CYCCNT; // 记录开始时的时钟节拍
    
    // 死等，直到经过了足够的 CPU 周期
    while ((DWT_CYCCNT - t0) < ticks); 
}

// 毫秒级延时 
void delay_ms(uint32_t nms) {
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        vTaskDelay(pdMS_TO_TICKS(nms));
    } else {
        delay_us(nms * 1000);
    }
}

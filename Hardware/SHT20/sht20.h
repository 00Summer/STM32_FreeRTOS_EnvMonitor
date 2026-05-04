#if !defined(_SHT20_H)
#define _SHT20_H

#include"stm32f10x.h"

void SHT20_Init(void);
float SHT20_Read_Temp(void);
float SHT20_Read_Hum(void);


#endif 

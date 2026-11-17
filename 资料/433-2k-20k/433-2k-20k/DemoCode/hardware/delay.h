

#ifndef __DELAY_H__
#define __DELAY_H__


#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"


void Delay_Init(void);
void delay_ns(u32 ns);
void delay_us(u32 Nus);
void delay_ms(u16 nms);

#endif


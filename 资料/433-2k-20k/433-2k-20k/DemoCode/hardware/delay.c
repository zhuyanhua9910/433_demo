

#include "delay.h"

static u8  fac_us=0;//us Delay multiple multiplier
static u16 fac_ms=0;//ms Delay multiple multiplier

//////////////////////////////////////////////////////////////////////////////delay
void delay_ns(u32 ns)
{
  u32 i;
  for(i=0;i<ns;i++)
  {
    __nop();
    __nop();
    __nop();
  }
}

void Delay_Init(void)		//unit:MHz
{
	SysTick->CTRL =0;//&= 0xfffffffb;//select internal clk: HCLK/8
	fac_us = 72/8;      
	fac_ms = (u16)fac_us*1000;
}            

void delay_us(u32 Nus)
{ 
	SysTick->LOAD=Nus*fac_us;       //load time   
	SysTick->VAL=	0x00;
	SysTick->CTRL|=0x01;            //start count   
	while(!(SysTick->CTRL&(1<<16)));//wait time out 
	SysTick->CTRL&=0xFFFFFFFE;  //close counter
	SysTick->VAL=0x00;        //clear counter    
}

void delay_ms(u16 nms)	//nms <= 0xffffff*8/SYSCLK; for 72M, Nms<=1864 
{    
	SysTick->LOAD=(u32)nms*fac_ms; 
	SysTick->VAL=	0x00;
	SysTick->CTRL|=0x01;               
	while(!(SysTick->CTRL&(1<<16)));  
	SysTick->CTRL&=0XFFFFFFFE;   
	SysTick->VAL=	0x00;          
}

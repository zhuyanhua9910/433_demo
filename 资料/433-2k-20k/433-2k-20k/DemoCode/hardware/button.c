
#include "button.h"
#include "delay.h"
#include "LCD.h"
#include "QX4463.h"

#define PressTime 200	// 0.1ms*200=20ms
#define BuzzerStopNum  100
u8 switch_flag = 0;
u8 flag_IRQ = 0;
//button
volatile u8 TimerUs=0;
volatile u32 buttontime = 0; 
//led
static LED_Info leds[4] = {0};
static uint16_t blinkInterval = 5000; //2000*100us=200ms 
static uint8_t maxBlinkTimes = 1;  
//buzzer
static u8 beepEnabled = 0;
static u16 beepCounter = 0;
static u16 LevelInversionTime = 50; //100us*50=5ms
static u16 LevelInversionNums = 0; //40*5ms=200ms


//============== funtion ==================//
void Key_Update(KeyStatus *key) 
{
    u8 new_state = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10) == Bit_RESET ? 1:0;  
    if (new_state != key->raw_state) 
	{
        key->raw_state = new_state;
        key->change_time = buttontime;
    }
    if((buttontime - key->change_time) > PressTime) 
	{
        key->last_state = key->current_state;
        key->current_state = key->raw_state; 
    }
}

void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseStructure.TIM_Period=100-1;  // 100us
	TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);  
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);  
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6, ENABLE);	
	
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void LED_StartBlink(LED_Type led)
{
    if(led == LED_ALL) 
	{
        for(int i = 0; i < 4; i++) 
		{
            leds[i].blinking = 1;
            leds[i].blinkCounter = 0;
            leds[i].blinkTimes = 0;
            leds[i].ledState = 1;
            GPIO_SetBits(leds[i].GPIOx, leds[i].GPIO_Pin);
        }
    } 
	else if(led < 4) 
	{
        leds[led].blinking = 1;
        leds[led].blinkCounter = 0;
        leds[led].blinkTimes = 0;
        leds[led].ledState = 1;
        GPIO_SetBits(leds[led].GPIOx, leds[led].GPIO_Pin);
    }
}

void LED_StopBlink(LED_Type led)
{
    if(led == LED_ALL) 
	{
        for(int i=0;i<4;i++) 
		{
            leds[i].blinking = 0;
            GPIO_ResetBits(leds[i].GPIOx, leds[i].GPIO_Pin);
        }
    } 
	else if(led<4) 
	{
        leds[led].blinking = 0;
        GPIO_ResetBits(leds[led].GPIOx, leds[led].GPIO_Pin);
    }
}

void LED_StopAll(void)
{
    LED_StopBlink(LED_ALL);
}

void Button_Init( void )
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PC10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init( GPIOC, &GPIO_InitStructure );
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PD2
	GPIO_Init( GPIOD, &GPIO_InitStructure );
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);	//PD2
	
	EXTI_InitTypeDef EXTI_InitStructure;	
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0A;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Led_Init(void)
{
	GPIO_InitTypeDef	GpioInitStructer;
	GpioInitStructer.GPIO_Mode = GPIO_Mode_Out_PP;						
	GpioInitStructer.GPIO_Speed = GPIO_Speed_50MHz;		
	//beer 
	GpioInitStructer.GPIO_Pin = BUZZER_GPIO_PIN;		
	GPIO_Init( BUZZER_GPIO_PORT, &GpioInitStructer );
	//led
	GpioInitStructer.GPIO_Pin = LED1_GPIO_PIN;		
	GPIO_Init( LED1_GPIO_PORT, &GpioInitStructer );	
	GpioInitStructer.GPIO_Pin = LED2_GPIO_PIN;		
	GPIO_Init( LED2_GPIO_PORT, &GpioInitStructer );
	GpioInitStructer.GPIO_Pin = LED3_GPIO_PIN;		
	GPIO_Init( LED3_GPIO_PORT, &GpioInitStructer );	
	GpioInitStructer.GPIO_Pin = LED4_GPIO_PIN;		
	GPIO_Init( LED4_GPIO_PORT, &GpioInitStructer );
	
	Led1=Led2=Led3=Led4=0;
	/*  Led Struct GPIO Init  */
	// LED1 -GPIOB1
	leds[0].GPIOx = GPIOB;
    leds[0].GPIO_Pin = GPIO_Pin_0;  
    // LED1 -GPIOB1
    leds[1].GPIOx = GPIOB;
    leds[1].GPIO_Pin = GPIO_Pin_1;  
    // LED2 -GPIOC4
    leds[2].GPIOx = GPIOC;
    leds[2].GPIO_Pin = GPIO_Pin_4;
    // LED3 -GPIOC5
    leds[3].GPIOx = GPIOC;
    leds[3].GPIO_Pin = GPIO_Pin_5;
}

void Flashing_LED(u8 times)
{
	Led1=Led2=Led3=Led4=0;
	for(int i=0;i<times*2;i++)
	{
		Led1 = ~Led1;Led2 = ~Led2;
		Led3 = ~Led3;Led4 = ~Led4;
		delay_ms(200);
	}
	Led1=Led2=Led3=Led4=0;
}

void BEEP_SetState(BEEP_State state)
{
    if(state == BEEP_ON) {
        beepEnabled = 1;
        beepCounter = 0;
    } else {
        beepEnabled = 0;
        GPIO_ResetBits(GPIOC, GPIO_Pin_0);
    }
}


//======================= interrupt ===========================//

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{		
		flag_IRQ = 1;
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{		
		switch_flag = 1;
		LCD_Clear();
		QX_SET_SDN_HIGH( );
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
//		printf("hhhhhh\n");
		DIRECT_TX_DAT = ~DIRECT_TX_DAT;	
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}

void TIM6_IRQHandler(void)	
{
	if(TIM_GetITStatus(TIM6 , TIM_IT_Update) != RESET ) 
	{
		TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);
		TimerUs = TRUE;
		buttontime++;
		if(buttontime>=0xFFFFFFFF)
		{
			buttontime=0;
		}
		
		if(beepEnabled) 
		{
			beepCounter++;
			if(beepCounter >= LevelInversionTime) 
			{
				beepCounter = 0;
				LevelInversionNums++;
				GPIOC->ODR ^= GPIO_Pin_0;
			}
			if(LevelInversionNums > BuzzerStopNum)
			{
				LevelInversionNums = 0;
				beepEnabled = 0;
			}
        }
		//////////// led blink /////////////////
		for(int i = 0; i < 4; i++) 
		{
            if(leds[i].blinking) 
			{
                leds[i].blinkCounter++;
                
                if(leds[i].blinkCounter >= blinkInterval) 
				{
                    leds[i].blinkCounter = 0;
                    
                    if(leds[i].ledState) 
					{
                        GPIO_ResetBits(leds[i].GPIOx, leds[i].GPIO_Pin);
                        leds[i].ledState = 0;
                    } 
					else 
					{
                        GPIO_SetBits(leds[i].GPIOx, leds[i].GPIO_Pin);
                        leds[i].ledState = 1;
                        leds[i].blinkTimes++; 
                        
                        if(leds[i].blinkTimes >= maxBlinkTimes) 
						{
                            leds[i].blinking = 0;
                            GPIO_ResetBits(leds[i].GPIOx, leds[i].GPIO_Pin);
                        }
                    }
                }
            }
        }// end for
	}		 	
}

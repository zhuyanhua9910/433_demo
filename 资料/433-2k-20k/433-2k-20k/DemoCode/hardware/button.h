
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "sys.h"
#include "string.h"

//led PB0 PB1 PC4 PC5
#define LED1_GPIO_PORT		GPIOB								
#define LED1_GPIO_PIN		GPIO_Pin_0
#define LED2_GPIO_PORT		GPIOB								
#define LED2_GPIO_PIN		GPIO_Pin_1
#define LED3_GPIO_PORT		GPIOC								
#define LED3_GPIO_PIN		GPIO_Pin_4
#define LED4_GPIO_PORT		GPIOC								
#define LED4_GPIO_PIN		GPIO_Pin_5
//beer PC0
#define BUZZER_GPIO_PORT	GPIOC								
#define BUZZER_GPIO_PIN		GPIO_Pin_0

#define Led1		PBout(0)								
#define Led2		PBout(1)
#define Led3		PCout(4)								
#define Led4		PCout(5)
#define Buzzer		PCout(0)

#define FALSE 	0
#define TRUE 	!FALSE 
//Direct TX IO
#define	 DIRECT_TX_CLK  PCin(7)
#define	 DIRECT_TX_DAT  PCout(9)
////////////////////////////
typedef enum {
    LED0 = 0,   // GPIOB0
    LED1 = 1,   // GPIOB1
    LED2 = 2,   // GPIOC4
    LED3 = 3,   // GPIOC5
    LED_ALL = 4 // All LED
} LED_Type;

typedef struct {
    uint8_t blinking;
    uint16_t blinkCounter;
    uint8_t ledState;
    uint8_t blinkTimes;
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
} LED_Info;

typedef enum {
    BEEP_OFF = 0,
    BEEP_ON = 1
} BEEP_State;

typedef struct {
	uint8_t current_state;  
	uint8_t last_state;   
	uint8_t raw_state;     
	uint32_t change_time; 
} KeyStatus;

void Key_Update(KeyStatus *key); 
void Timer_Init(void);
void Flashing_LED(u8 times);
void Button_Init(void);

void Led_Init(void);
void LED_StartBlink(LED_Type led);
void LED_StopBlink(LED_Type led);
void LED_StopAll(void);
void BEEP_SetState(BEEP_State state);

#endif

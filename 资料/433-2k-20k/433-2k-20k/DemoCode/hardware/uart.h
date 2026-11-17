
#ifndef __UART_H__
#define __UART_H__

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include <string.h> 

//uart define
#define UART_PORT					USART1
#define UART_PORT_CLK				RCC_APB2Periph_USART1
#define UART_PORT_AF				GPIO_Remap_USART1

//uart pin define
#define UART_TX_GPIO_PORT			GPIOA
#define UART_TX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define UART_TX_GPIO_PIN			GPIO_Pin_9

#define UART_RX_GPIO_PORT			GPIOA
#define UART_RX_GPIO_CLK			RCC_APB2Periph_GPIOA
#define UART_RX_GPIO_PIN			GPIO_Pin_10


void Uart_Init( uint32_t UartBaudRate );
void uart_tx_bytes( uint8_t* TxBuffer, uint8_t Length );
uint8_t uart_rx_bytes( uint8_t* RxBuffer );



#endif




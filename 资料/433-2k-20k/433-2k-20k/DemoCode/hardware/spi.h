

#ifndef __SPI_H__
#define __SPI_H__


#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"


//SPI pin define
#define SPI_NSS_GPIO_PORT			GPIOB
#define SPI_NSS_GPIO_CLK			RCC_APB2Periph_GPIOB
#define SPI_NSS_GPIO_PIN			GPIO_Pin_12				

#define SPI_CLK_GPIO_PORT			GPIOB
#define SPI_CLK_GPIO_CLK			RCC_APB2Periph_GPIOB
#define SPI_CLK_GPIO_PIN			GPIO_Pin_13				

#define SPI_MISO_GPIO_PORT			GPIOB
#define SPI_MISO_GPIO_CLK			RCC_APB2Periph_GPIOB	
#define SPI_MISO_GPIO_PIN			GPIO_Pin_14

#define SPI_MOSI_GPIO_PORT			GPIOB
#define SPI_MOSI_GPIO_CLK			RCC_APB2Periph_GPIOB	
#define SPI_MOSI_GPIO_PIN			GPIO_Pin_15


#define SPI_Set_NSS_High( )			SPI_NSS_GPIO_PORT->ODR |= SPI_NSS_GPIO_PIN					//high
#define SPI_Set_NSS_Low( )			SPI_NSS_GPIO_PORT->ODR &= (u32)( ~((u32)SPI_NSS_GPIO_PIN ))	//low



void QX40x_SPIInit( void );
u8 SpiReadWriteByte(u8 TxByte);

#endif



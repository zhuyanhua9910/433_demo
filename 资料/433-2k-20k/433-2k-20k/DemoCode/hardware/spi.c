
#include "spi.h"

#define SpiTimeOut			((u16)0xFFFF)


void QX40x_SPIInit(void)
{
	GPIO_InitTypeDef	SpiGpioInitStructer;
	SPI_InitTypeDef		SpiInitStructer;
	//SPI CLK configure
	RCC_APB2PeriphClockCmd( SPI_CLK_GPIO_CLK | SPI_MISO_GPIO_CLK | SPI_MOSI_GPIO_CLK | SPI_NSS_GPIO_CLK, ENABLE );	//打开端口时钟
	//SCK MOSI MISO configure to AF
	SpiGpioInitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	SpiGpioInitStructer.GPIO_Mode = GPIO_Mode_AF_PP;
	//SCLK
	SpiGpioInitStructer.GPIO_Pin = SPI_CLK_GPIO_PIN;
	GPIO_Init( SPI_CLK_GPIO_PORT, &SpiGpioInitStructer );		
	//MOSI
	SpiGpioInitStructer.GPIO_Pin = SPI_MOSI_GPIO_PIN;
	GPIO_Init( SPI_MOSI_GPIO_PORT, &SpiGpioInitStructer );		
	GPIO_SetBits( SPI_MOSI_GPIO_PORT, SPI_MOSI_GPIO_PIN );
	//MISO
	SpiGpioInitStructer.GPIO_Pin = SPI_MISO_GPIO_PIN;
	GPIO_Init( SPI_MISO_GPIO_PORT, &SpiGpioInitStructer );		
	GPIO_SetBits( SPI_MISO_GPIO_PORT, SPI_MISO_GPIO_PIN );
	//NSS configure to out_pp
	SpiGpioInitStructer.GPIO_Mode = GPIO_Mode_Out_PP;
	SpiGpioInitStructer.GPIO_Pin = SPI_NSS_GPIO_PIN;
	GPIO_Init( SPI_NSS_GPIO_PORT, &SpiGpioInitStructer );
	//Initialize the default high level
	GPIO_SetBits( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN );
	
	//SPI configure
	SPI_I2S_DeInit( SPI2 );			//rest SPI
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );		
	SPI_Cmd( SPI2, DISABLE );		//disable SPI
	
	SpiInitStructer.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//Double-line full-duplex
	SpiInitStructer.SPI_Mode = SPI_Mode_Master;							//Host mode
	SpiInitStructer.SPI_CPOL = SPI_CPOL_Low;							//The idle state is low level
	SpiInitStructer.SPI_CPHA = SPI_CPHA_1Edge;							//The first edge collects data
	SpiInitStructer.SPI_DataSize = SPI_DataSize_8b;						//8 bits of data
	SpiInitStructer.SPI_NSS = SPI_NSS_Soft;								//Slave machine software
	SpiInitStructer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	//64 Frequency division
	SpiInitStructer.SPI_FirstBit = SPI_FirstBit_MSB;					//MSB fist to be send
	SpiInitStructer.SPI_CRCPolynomial = 7;								//Do not use CRC	 
	
	SPI_Init( SPI2, &SpiInitStructer );
	SPI_Cmd( SPI2, ENABLE );
}
/*!==============================================================================
  * @brief :SPI Send and receive one byte
  * @param :
			@TxByte: byte to be send
  * @note  :Non-blocking type, the timeout function will exit
  * @retval:receive data
   =============================================================================*/
u8 SpiReadWriteByte( u8 TxByte )
{
	u8 Data = 0;
	u16 WaitTime = 0;
	//The waiting to send buffer is empty
	while( RESET == SPI_I2S_GetFlagStatus( SPI2, SPI_I2S_FLAG_TXE ) )
	{
		if( SpiTimeOut == ++WaitTime )
		{
			break;			//Exit due to timeout
		}
	}
	//Reset the receiving waiting time (Because SPI is very fast, under normal circumstances,
	//	data will be received immediately after the sending is completed, and the waiting time 
	//		does not need to be too long)
	WaitTime = SpiTimeOut / 2;	
	SPI2->DR = TxByte;	//send data
	//The waiting to receive buffer is empty
	while( RESET == SPI_I2S_GetFlagStatus( SPI2, SPI_I2S_FLAG_RXNE ) )		
	{
		if( SpiTimeOut == ++WaitTime )
		{
			break;			//Exit due to timeout
		}
	}
	Data = (u8)SPI2->DR;		//receive data
	
	return Data;		
}



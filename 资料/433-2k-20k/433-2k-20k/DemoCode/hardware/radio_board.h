#ifndef __RADIO_BOARD_H__
#define __RADIO_BOARD_H__
#include "stm32f10x.h"
#include "sys.h"

#define FALSE 	0
#define TRUE 	!FALSE 

typedef enum {
    MODE_CW = 0,         
    MODE_DIRECT_RX, 	
//	MODE_DIRECT_TX,
	MODE_BIDIRECTIONALPACKET,
    MODE_COUNT             
} OpMode;


typedef enum 
{
	CW     = 0,
	OOK	   = 1,
	FSK2   = 2,
	GFSK2  = 3,
	FSK4   = 4,
	GFSK4  = 5
}ModeTpye;	

typedef enum 
{
	SLEEP		= 1,	
	SPI_ACTIVE	= 2,
	READY		= 3,
	READY2		= 4,
	TX_TUNE		= 5,
	RX_TUNE		= 6,
	TX			= 7,
	RX			= 8
}CurrentState;	



void GetPartInfo(void);
void GetModType(void);
void GetChannelState(void);
void ShowInformation(void);
void GetRSSI(void);

void ProTxConfig(u8 select);
u8 InformationCompare(u8* String1, u8* String2, u8 length);
void Board_Init(void);

//////////////////////
void Unmodulated_Camier_CW(void);
void DirectRx(void);
void DirectTx(void);
void DirectTxPortInit(void);
void BidirectionalPacket(void);
void Polling_Handler(void);
u8 SendVariablePacket(u8 channel,u8*packet,u8 length);

#endif

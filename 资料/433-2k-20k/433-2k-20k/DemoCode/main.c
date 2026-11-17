#include "radio_board.h"
#include "button.h"
#include "QX4463.h"
#include <string.h> 

u8 CurrentMode =0;

int main(void) 
{
    Board_Init();
	while(1)
	{	
		switch(CurrentMode) 
		{
            case MODE_CW:
					Display_Asc_String(36, 11*0, 3, (u8*)"<  CW  >");
					Unmodulated_Camier_CW();
					break;
			case MODE_DIRECT_RX: 
					Display_Asc_String(15, 11*0, 3, (u8*)"<  Direct-RX  >");
					DirectRx();
					break;
//			case MODE_DIRECT_TX: 
//					Display_Asc_String(15, 11*0, 3, (u8*)"<  Direct-TX  >");
//					DirectTx();
//					break;
			case MODE_BIDIRECTIONALPACKET: 
					Display_Asc_String(4, 11*0, 3, (u8*)"< BidirectionalPkt >");
					BidirectionalPacket();
					break;
            default: 
					//CurrentMode = MODE_CW;
					break;
        }
		delay_ms(30); // delay 30 ms after shut down SDN
		CurrentMode = (CurrentMode + 1) % MODE_COUNT;
	}   
}






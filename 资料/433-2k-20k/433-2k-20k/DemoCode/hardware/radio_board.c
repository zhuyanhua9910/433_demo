
#include "radio_board.h"
#include "uart.h"
#include "lcd.h"
#include "button.h"
#include "QX4463.h"
#include "wds.h"

u8 CustomPacket[] = {'B','U','T','T','O','N','1'};
u8 AckPacket[] = {'A','C','K'};
static const char* mod_names[] =
{"CW","OOK","2FSK","2GFSK","FSK4","4GFSK"};
static const char* state_names[] = 
{"SLEEP","SPI_ACTIVE","READY","READY2","TX_TUNE","RX_TUNE","TX","RX"};
u8 PartInfo[]={0x22,0x44,0x63,0x00,0x86,0x00,0x00,0x06};

#define PACKET_SEND_INTERVAL 8000 //0.1ms*5000=500ms

u8 RxBuffer[64] = {0}; 
u8 Tx_ACk = 0;   
u8 Tx_custom = 0; 
u8 INT_Status = 0;
u16 CountInterval_100Us = 0;

extern u8 flag_IRQ;
extern u8 switch_flag;
extern volatile u8 TimerUs;
extern u8 buff[16];
extern u8 Resp[32];
extern u8 Cmd[32];
KeyStatus key = {0};


//==================== CW ==========================//
void Unmodulated_Camier_CW(void)
{
	QX40x_Reset( );				//QX446x rest
	QX40x_PowerUp(0x01,0x00,30000000 );	//powerup 30MHz
	ProTxConfig(0);
	ShowInformation( );
	//GPIO0,GPIO2:Tx(0,1) Rx(1,0)
	QX40x_GpioPinCfg(0x40|2,0x40|17,0x40|3,0x40|20,0,0,0);
	Flashing_LED(2);
	QX40x_StartTx(0,0x30,0); 
	while(!switch_flag)
	{
		Led1=1;
		Led4=1;
	}
	switch_flag = 0;
}

//==================== DirectRx ==========================//
void DirectRx(void)
{
	QX40x_Reset( );//QX4463 rest
	QX40x_PowerUp(0x01,0x00,30000000 );	//powerup 30MHz
//	ProTxConfig(2);
	QX40x_Config_Init();
	//GPIO0,GPIO2:Tx(0,1) Rx(1,0)
	QX40x_GpioPinCfg(0x40|3,0x40|17,0x40|20,0x40|2,0,0,0);
//	QX40x_GpioPinCfg(0,0x40|20,0,0,0,0,0);
	ShowInformation();
	Flashing_LED(3);
	QX40x_StartRx(0,0,0,0,8,8);
	while(!switch_flag)
	{
		Led2=1;
		Led3=1;
	}
	switch_flag = 0;
}

//==================== DirectTx ==========================//
void DirectTx(void) 
{
    QX40x_Reset( );				
	QX40x_PowerUp(0x01,0x00,30000000 );	
	ProTxConfig(1);
//	QX40x_Config_Init();
	//GPIO0,GPIO2:Tx(0,1) Rx(1,0)
	QX40x_GpioPinCfg(0x40|2,0x50,0x40|3,0x44,0,0,0);
	ShowInformation();
	DirectTxPortInit();
    Flashing_LED(3); 
	QX40x_StartTx(0,0,0);
    while(!switch_flag)
	{
		Led3=1;
	}
	switch_flag = 0;
}

void DirectTxPortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_7; //PC7 clk
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_9; //PC9 data
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	DIRECT_TX_DAT = 1;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);	 
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0B;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//////////////////////////////////////////////////////
void BidirectionalPacket(void) 
{
	QX40x_Reset( );//QX4463 rest
	QX40x_PowerUp(0x01,0x00,30000000 );	//powerup 30MHz
	ProTxConfig(3);
//	QX40x_Config_Init();
	//GPIO0,GPIO2:Tx(0,1) Rx(1,0)
	QX40x_GpioPinCfg(0x40|3,0,0x40|2,0,0,0,0);
	ShowInformation();
    Radio_StartRx(0, 0, 7, 0, 3, 8);
	QX40x_GetIntStatus(0, 0, 0);
    Flashing_LED(3); 

    while(!switch_flag) 
	{
		if(TimerUs){
			if (CountInterval_100Us < 0xFFFF){
				//Count once every 100 microseconds
				CountInterval_100Us++;	
			}	
		}	
		Key_Update(&key);
		TimerUs = 0;
		Polling_Handler();
    }
	switch_flag = 0;
}

u8 Radio_Check_Tx_Rx(void)
{
	if (flag_IRQ == 1)
	{
		flag_IRQ=0;
		/* Read ITs, clear pending ones */
		QX40x_GetIntStatus(0, 0, 0);
		if (Resp[6] & (0x01 << 3))
		{
			/* State change to */
			QX40x_ChangeStatus(1);
			/* Reset FIFO */
			QX40x_FifoInfo(0x02);
			/* State change to */
			QX40x_ChangeStatus(8);
		}
		if(Resp[2] & (0x01 << 5))
		{
			return 0x20;
		}
		if(Resp[2] & (0x01 << 4))
		{
//			GetRSSI();
			/* Packet RX */
			/* Get payload length */
			u8 len = QX40x_FifoInfo(0);
			QX40x_ReadRxFifo(RxBuffer, len);
			Display_Asc_String(30, 11 * 5, 4, (u8 *)RxBuffer);
			return 0x10;
		}
		if (Resp[2] & (0x01 << 3))
		{
			/* Reset FIFO */
			QX40x_FifoInfo(0x02);
			Display_Asc_String(0, 11*4, 3, (u8*)"CRC_ERRO!");
		}
	}
	return 0;
}

void Polling_Handler(void)
{
	static u8 PktSending = 0;
	INT_Status = Radio_Check_Tx_Rx();
	switch (INT_Status)
	{
		case 0x20:
			memset(RxBuffer, 0, sizeof(RxBuffer));
			QX40x_GpioPinCfg(0x40|3,0,0x40|2,0,0,0,0);
			if( Tx_ACk )
			{			
				// Message "ACK" sent successfully
				LED_StartBlink(LED0);
				Tx_ACk=0;
				// Start RX with radio packet length
				Radio_StartRx(0, 0, 7, 0, 3, 8);
			}
			else if(Tx_custom)
			{
				// Custom message sent successfully
				LED_StartBlink(LED1);
				Tx_custom=0;
				// Start RX waiting for "ACK" message
				Radio_StartRx(0, 0, 3, 0, 3, 8);
			}
			/* Clear Packet Sending flag */
			PktSending = 0;
			LCD_Clear();
		break;

		case 0x10:
			LCD_Clear();
			if (InformationCompare(RxBuffer,AckPacket,3) == 1)
			{
				QX40x_GpioPinCfg(0x40|3,0,0x40|2,0,0,0,0);
				// Start RX with radio packet length
				Radio_StartRx(0, 0, 7, 0, 3, 8);
				// "ACK" arrived successfully
				LED_StartBlink(LED_ALL);
//				BEEP_SetState(BEEP_ON);
				break;
			}
			else
			{
				if (InformationCompare(RxBuffer,CustomPacket,7) == 1)
				{
					QX40x_GpioPinCfg(0x40|2,0,0x40|3,0,0,0,0);
					// Blink once LED1 as payload match
					LED_StartBlink(LED2);
					// Send ACK back
					Radio_StartTx_Packet(0,AckPacket,3);
					Tx_ACk=1;
				}
			}
			break;
	} 
	if((CountInterval_100Us >= PACKET_SEND_INTERVAL) & (0 == PktSending))
	{	
		if (SendVariablePacket(0,CustomPacket,7) == TRUE)
		{
			/* Reset packet send counter */
			CountInterval_100Us = 0;
			/* Set Packet Sending flag */
			PktSending = 1;
		}
	}
}

u8 SendVariablePacket(u8 channel,u8*packet,u8 length)
{
	if(key.current_state)
	{ 
		QX40x_GpioPinCfg(0x40|2,0,0x40|3,0,0,0,0);
		Radio_StartTx_Packet(channel,packet,length);
		Tx_custom=1;
   	/* Packet sending initialized */
   	return TRUE;
    }
	QX40x_GpioPinCfg(0x40|3,0,0x40|2,0,0,0,0);
    return FALSE;
}


//////////////////////////////////////////////////////
void GetModType(void)
{
	QX40x_GetProperty(MODEM_MOD_TYPE);
	u8 modetype = Resp[0] & 0x07;
    if(modetype < sizeof(mod_names)/sizeof(mod_names[0])) 
	{
        printf("ModType: %s\n", mod_names[modetype]);
		sprintf((char*)buff,"Type:%s\n",mod_names[modetype]);
        Display_Asc_String(50, 11*2, 3, buff);
	}
}

void GetPartInfo(void)
{
	QX40x_PartInfo( );
	printf("\n");
	if( InformationCompare(Resp,PartInfo,8) ){
		sprintf((char*)buff,"QX403");
		printf("QX403\n");
	}
	else{
		sprintf((char*)buff,"XXXXXXX");
 		printf("XXXXXXX\n");
	}
	Display_Asc_String(0, 11*2, 3, buff);
		
}

void GetChannelState(void)
{
	u8 currstate=0,channel=0;
	QX40x_RequestDeviceStatus( );
	currstate = Resp[0];
    if(currstate < sizeof(state_names)/sizeof(state_names[0])) 
	{
        printf("CRR_STATE: %s\n", state_names[currstate]);
        Display_Asc_String(0, 11*4, 3, (u8*)state_names[currstate]);
	}
	channel = Resp[1];
	printf("CHANNEL: %d\n", channel);
	sprintf((char*)buff,"CHANNEL:%d\n",channel);
	Display_Asc_String(50, 11*4, 3, buff);
}

void ShowInformation(void)
{
	GetPartInfo();
	GetModType();
	GetChannelState();
}

void GetRSSI(void)
{
	QX40x_GetModemStatus(0);
	u8 rssi = Resp[3];
	s8 RF_InputLevel_dBm = ((rssi/2)-134);
	sprintf((char*)buff,"RF_InputLevel_dBm:%d\n",RF_InputLevel_dBm);
	printf("%s\r\n", buff);
}

u8 InformationCompare(u8* String1, u8* String2, u8 length)
{
	while (length-- > 0)
    {
        if (*String1++ != *String2++)
        {
            return FALSE; 
        }
    }
    return TRUE;
}


void ProTxConfig(u8 select)
{
	switch(select)
	{
		case 0:
			Radio_SetPros(CW_433_92MHz,sizeof(CW_433_92MHz)/sizeof(_PROP));
			break;
		case 1:
			Radio_SetPros(DT433_92MHz_FSK2_R2K_D20K,sizeof(DT433_92MHz_FSK2_R2K_D20K)/sizeof(_PROP));
			break;
		case 2:
			Radio_SetPros(DR433_92MHz_2FSK2_R2K_D20K,sizeof(DR433_92MHz_2FSK2_R2K_D20K)/sizeof(_PROP));
			break;
		case 3:
			Radio_SetPros(BD433_92MHz_2FSK_R10K_D20K,sizeof(BD433_92MHz_2FSK_R10K_D20K)/sizeof(_PROP));
			break;
		default:
				break;
	}
}

void Board_Init(void)
{
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC 
					| RCC_APB2Periph_GPIOD | RCC_APB2Periph_ADC1 |RCC_APB2Periph_AFIO, ENABLE);
	//RCC_ADCCLKConfig(RCC_PCLK2_Div6);//72M/6=12MHz
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	////////////////////////////////////////////////////////////////////////////////
	Uart_Init(115200);		//usart init 	
	Delay_Init( );			//delay init	
	Led_Init( );			//LED init	
	Timer_Init();
	LCD_Init( );			//LCD init 
	LCD_Clear( );			//clear LCD
	Button_Init( );			//buttom init
	QX40x_SPIInit( );		//SPI init
	QX40x_Init( );			//QX4463 init
}





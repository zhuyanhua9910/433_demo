
#include "QX4463.h"
#include "wds3.h"

u8 buff[16] = {0};
u8 Resp[32] ={0};
u8 Cmd[32]  ={0};
const static u8 Configure_array[ ] = RADIO_CONFIGURATION_DATA_ARRAY;

/*!================================
  * @brief :QX40x Empty operation
   ===============================*/
void QX40x_Nop(void)
{
    QX_SET_CSN_LOW( );			
    SpiReadWriteByte(NOP);	
    QX_SET_CSN_HIGH( );		
}

/*!===========================
  * @brief :QX4463 wait cts
   ==========================*/
void QX40x_WaitCts(void)
{
	u16 CTS_TIMEOUT = 10000;
	u8 cts = 0;
    do
    {
		QX_SET_CSN_LOW( );
		SpiReadWriteByte(READ_CMD_BUFF);
		cts = SpiReadWriteByte(0xFF);
		QX_SET_CSN_HIGH( );
	}while((cts != 0xFF)&&(CTS_TIMEOUT-- != 0));
}

/*!============================================
  * @brief :read response command
  * @param :
  *			@pRead:Return position
  *			@NumBytes:number bytes of read
   ===========================================*/
void QX40x_ReadResponse(u8 *pRead, u8 NumBytes)
{
	u8 cts=0;
    QX40x_WaitCts( );		
    QX_SET_CSN_LOW( );		
	SpiReadWriteByte(READ_CMD_BUFF);
	cts=SpiReadWriteByte(0xFF);
	if(cts==0xff)
	{
		while( NumBytes-- )
		{
			*pRead++ = SpiReadWriteByte(0xFF);	
		}
	}
    QX_SET_CSN_HIGH( );		
}

/*!==================================================
  * @brief :fast read response
  * @param :
  * 		@cmd:command 
  *			@pRead:Return position
  *			@NumBytes:number of read
   =================================================*/
void QX40x_FastRead(u8 Cmd, u8 *pRead, u8 NumBytes)
{
    QX40x_WaitCts( );		
    QX_SET_CSN_LOW( );		
	SpiReadWriteByte(Cmd);	
	while( NumBytes-- )
    {
        *pRead++ = SpiReadWriteByte(0xFF);	
    }
    QX_SET_CSN_HIGH( );		
}

/*!=========================================
  * @brief : write command
  * @param :
  *			@pCmd:command
  *			@CmdNum：Number bytes of write
   ========================================*/
void QX40x_WriteCmd(u8 *pCmd,u8 CmdNum)
{
    QX40x_WaitCts( );		
    QX_SET_CSN_LOW( );			
    while(CmdNum--)
    {
        SpiReadWriteByte(*pCmd++);
    }
    QX_SET_CSN_HIGH( );			
}

/*!============================================
  * @brief : send commends
  * @param :
  *			@Cmd：commend
  *			@CmdNum：number bytes of write
  *			@pRead：receive data
  *			@ReadNum：number bytes of receive
   ===========================================*/
void QX40x_SendCmdGetResp(u8 *Cmd,u8 CmdNum,u8 *pRead,u8 ReadNum)
{
	QX40x_WriteCmd(Cmd,CmdNum);
	if(ReadNum)
		QX40x_ReadResponse(pRead,ReadNum);
}

/*!===============================================================
  * @brief :power-up the device
  * @param :
  *		@BOOTOPTIONS: Selects the functionality of the device 
		by specifying the firmware image to be loaded upon boot-up
  *		@XTALOPTIONS: Crystal oscillator selection
  *		@XOFREQ: Crystal oscillator frequency
   ===============================================================*/
void QX40x_PowerUp(u8 BOOT_OPTIONS, u8 XTAL_OPTIONS, u32 XO_FREQ)
{
    Cmd[0] = POWER_UP;		
    Cmd[1] = BOOT_OPTIONS;
    Cmd[2] = XTAL_OPTIONS;
    Cmd[3] = XO_FREQ >> 24;
    Cmd[4] = XO_FREQ >> 16;
    Cmd[5] = XO_FREQ >> 8;
    Cmd[6] = XO_FREQ;
    QX40x_WriteCmd(Cmd,7);
}

/*!======================================================
  * @brief : Reports basic information about the device.
   ======================================================*/
void QX40x_PartInfo( void )
{
    Cmd[0] = PART_INFO;
	QX40x_SendCmdGetResp(Cmd,1,Resp,8);
}

/*!========================================================
  * @brief : Reports Factory information about the device.
   =======================================================*/
void QX40x_BootFactoryInfo(void)
	{
	//A:0x532E,B:0x2C70
	Cmd[0] = BOOT_FACT_INFO;
	Cmd[1] = 0;
	QX40x_SendCmdGetResp(Cmd,2,Resp,16);
}
	
/*!==================================================================
  * @brief : Returns the Function revision information of the device.
   =================================================================*/
void QX40x_FunInfo(void)
{
	Cmd[0] = FUNC_INFO;	
	QX40x_SendCmdGetResp(Cmd,1,Resp,6);
}

/*!========================================================================
  * @brief : Return the interrupt status or clear latched interrupt events.
  * @param :
  *		@phclrpend: clear packet pending interrupt.
  *		@modemclrpend: clear modem pending interrupt.
  *		@chipclrpend: clear chip pending interrupt.
   =======================================================================*/
void QX40x_GetIntStatus(u8 ph_clr_pend,u8 modem_clr_pend,u8 chip_clr_pend)
{
    Cmd[0] = GET_INT_STATUS;
	Cmd[1] = ph_clr_pend;
	Cmd[2] = modem_clr_pend;
	Cmd[3] = chip_clr_pend;
	QX40x_SendCmdGetResp(Cmd,4,Resp,8);
}

/*!================================================
  * @brief : set the value of one properties
  * @param :
  *			@property: property
  *			@value: value of the property
   ===============================================*/
void QX40x_SetProperty(u16 property,u8 value)
{
    Cmd[0] = SET_PROPERTY;		
    Cmd[1] = property >> 8;
    Cmd[2] = 1;
    Cmd[3] = property;
    Cmd[4] = value;
	QX40x_WriteCmd(Cmd,5);
}

/*!================================================================
  * @brief : set the value of more properties
  * @param :
  *			@Group: Selects the property group to be retrieved.
  *			@NumProps: Selects the number of contiguous properties 
					   to be retrieved, starting at START_PROP.
  *			@SartProp: Selects the offset number or index of the 
					   property within the specified group
  *			@pdata: values of the property to be written
   ===============================================================*/
void QX40x_SetPropertys(u8 Group,u8 NumProps,u8 SartProp,u8 *pdata)
{
    u8 i = 0;
    Cmd[0] = SET_PROPERTY;		
    Cmd[1] = Group;
    Cmd[2] = NumProps;
    Cmd[3] = SartProp;
    while( NumProps-- )
    {
        Cmd[4+i] = *pdata++;
    }
    QX40x_WriteCmd(Cmd,4+NumProps);		
}

/*!==============================================
  * @brief : Retrieves the value of one propertie
  * @param :
  *			@property: property
  * @retval: none
   =============================================*/
void QX40x_GetProperty(u16 property)
{
    Cmd[0] = GET_PROPERTY;
    Cmd[1] = property >> 8;
    Cmd[2] = 1;
    Cmd[3] = property;		
	QX40x_SendCmdGetResp(Cmd,4,Resp,1);
}

/*!===============================================================
  * @brief : Retrieves the value of more properties
  * @param :
  *			@Group: Selects the property group to be retrieved.
  *			@NumProps: Selects the number of contiguous properties 
					   to be retrieved, starting at START_PROP.
  *			@SartProp: Selects the offset number or index of the 
					   property within the specified group
  *			@pRead: datas of read properties
   ===============================================================*/
void QX40x_GetPropertys(u8 Group,u8 NumProps,u8 SartProp,u8 *pRead )
{
    Cmd[0] = GET_PROPERTY;
    Cmd[1] = Group >> 8;
    Cmd[2] = NumProps;
    Cmd[3] = SartProp;	
	QX40x_SendCmdGetResp(Cmd,4,pRead,NumProps);	
}

/*!================================================================
  * @brief : configure GPIOs.
   ===============================================================*/
void QX40x_GpioPinCfg(u8 GPIO0,u8 GPIO1,u8 GPIO2,u8 GPIO3,
										u8 IRQ,u8 SDO,u8 GEN_CONFIG)
{
    Cmd[0] = GPIO_PIN_CFG;
    Cmd[1] = GPIO0;
    Cmd[2] = GPIO1;
    Cmd[3] = GPIO2;
    Cmd[4] = GPIO3;
    Cmd[5] = IRQ;
    Cmd[6] = SDO;
    Cmd[7] = GEN_CONFIG;
	QX40x_SendCmdGetResp(Cmd,8,Resp,7);
}

/*!=============================================
  * @brief : Writes data byte(s) to the TX FIFO.
  * @param :
*			@pWriteData: data byte(s) of write.
*			@NumBytes: bytes number of write.
   ============================================*/
void QX40x_WriteTxFifo(u8 *pWriteData,u8 NumBytes)
{
	QX40x_WaitCts();
    QX_SET_CSN_LOW( );
    SpiReadWriteByte(WRITE_TX_FIFO);	
    while( NumBytes-- )    
	{ 
		SpiReadWriteByte(*pWriteData++); 
	}
    QX_SET_CSN_HIGH( );
}

/*!================================================================
  * @brief : Access the current byte counts in the TX and RX FIFOs, 
			 and provide for resetting the FIFOs.
  * @param :
*			@FIFO: reset tx fifo(0) or rx fifo(1).
   ================================================================*/
u8 QX40x_FifoInfo(u8 FIFO)
{	
    Cmd[0] =FIFO_INFO; 
	Cmd[1] =FIFO;
	QX40x_SendCmdGetResp(Cmd,2,Resp,2);
	return Resp[0];
}

/*!===================================================================
  * @brief : Switches to TX state and starts transmission of a packet.
  * @param :
*			@Channel: Selects the channel number on which to transmit.
*			@Condition: Selection of sending status
*			@TxLen: the length of Tx datas.
   ==================================================================*/
void QX40x_StartTx( u8 Channel, u8 Condition, u16 Tx_Len)
{
    Cmd[0] = START_TX;		
    Cmd[1] = Channel;
    Cmd[2] = Condition;
    Cmd[3] = (u8)(Tx_Len >> 8);
    Cmd[4] = (u8)Tx_Len;
	Cmd[5] = 0x00;
	Cmd[6] = 0x00;
    QX40x_WriteCmd(Cmd,7);
}

/*!=========================================================================
  * @brief : Switches to RX state and starts reception of a packet.
			 and provide for resetting the FIFOs.
  * @param :
  *		@Channel：Channel number to transmit the packet on
  *		@Condition：Start receiving status
  *		@RxLen：lendth of rx
  *		@NextState1：This parameter selects the desired operational state 
		of the chip to automatically enter upon timeout of Preamble detection.
  *		@NextState2：This parameter selects the desired operational state 
		of the chip to automatically enter upon reception of a valid packet.
  *		@NextState3：This parameter selects the desired operational state of 
		the chip to automatically enter upon reception of an invalid packet.
   =========================================================================*/
void QX40x_StartRx(u8 Channel,u8 Condition,u16 Rx_Len, 
							u8 Next_State1,u8 Next_State2,u8 Next_State3)
{
    Cmd[0] = START_RX;
    Cmd[1] = Channel;
    Cmd[2] = Condition;
    Cmd[3] = (u8)(Rx_Len >> 8);
    Cmd[4] = (u8)Rx_Len;
    Cmd[5] = Next_State1;
    Cmd[6] = Next_State2;
    Cmd[7] = Next_State3;	
    QX40x_WriteCmd(Cmd,8);
}

/*!=================================================================================
  * @brief :  Returns information about the length of the variable field in the 
			  last packet received, and (optionally) overrides field length.
			 and provide for resetting the FIFOs.
  * @param :
  *			@FieldNum：Mask domain
  *			@Length：Selects the new length (in bytes) of the data field 
			specified by the FIELD_NUM parameter.
  *			@DiffLen：Specifies the difference (in bytes) between the new length value 
			and the original length value (specified in the corresponding 
			PKT_FIELD_X_LENGTH property, or passed in the START_RX:RX_LEN parameter).
   =================================================================================*/
void QX40x_GetPacketInfo(u8 FieldNum,u16 Len,u16 Len_Diff)
{	
    Cmd[0] = PACKET_INFO;
    Cmd[1] = FieldNum;
    Cmd[2] = (u8)(Len >> 8);
    Cmd[3] = (u8)Len;
    Cmd[4] = (u8)(Len_Diff >> 8);
    Cmd[5] = (u8)Len_Diff;
	QX40x_SendCmdGetResp(Cmd,6,Resp,2);
}

/*!===========================================
  * @brief : read RX FIFO data
  * @param :
  *			@pRxData：read datas
  *			@NumBytes：bytes number of read
   ==========================================*/
void QX40x_ReadRxFifo(u8 *pRxData,u8 NumBytes)
{
    QX40x_WaitCts( );
    QX_SET_CSN_LOW( );
    SpiReadWriteByte(READ_RX_FIFO);	
    while(NumBytes--)
    {
        *pRxData++ = SpiReadWriteByte(0xFF);	
    }	
    QX_SET_CSN_HIGH( );	
}

/*!=================================================================
  * @brief : Manually switch the chip to a desired operating state.
  * @param :
*			@NextStatus: Operating state to switch to immediately.
   ================================================================*/
void QX40x_ChangeStatus(u8 NextStatus)
{
    Cmd[0] = CHANGE_STATE;
    Cmd[1] = NextStatus;
    QX40x_WriteCmd(Cmd,2);
}

/*!=====================================================================
  * @brief : Reads the fast response registers(FRR) starting with FRR_A.
  * @param : Number of Fast Response Registers to be read.
   ====================================================================*/
void QX40x_FrrARead(u8 RespByteNum)
{
    Cmd[0] = FRR_A_READ;		
	QX40x_FastRead(Cmd[0],Resp,RespByteNum);
}

/*!=====================================================================
  * @brief : Reads the fast response registers(FRR) starting with FRR_B.
  * @param : Number of Fast Response Registers to be read.
   ====================================================================*/
void QX40x_FrrBRead(u8 RespByteNum)
{
    Cmd[0] = FRR_B_READ;		
    QX40x_FastRead(Cmd[0],Resp,RespByteNum);
}

/*!=====================================================================
  * @brief : Reads the fast response registers(FRR) starting with FRR_C.
  * @param : Number of Fast Response Registers to be read.
   ====================================================================*/
void QX40x_FrrCRead(u8 RespByteNum)
{
    Cmd[0] = FRR_C_READ;		
    QX40x_FastRead(Cmd[0],Resp,RespByteNum);
}

/*!=====================================================================
  * @brief : Reads the fast response registers(FRR) starting with FRR_D.
  * @param : Number of Fast Response Registers to be read.
   ====================================================================*/
void QX40x_FrrDRead(u8 RespByteNum)
{
    Cmd[0] = FRR_D_READ;		
    QX40x_FastRead(Cmd[0],Resp,RespByteNum);
}

/*!===================================================================
  * @brief : Performs conversions using the Auxiliary ADC and returns 
				the results of those conversions.
  * @param :
   ==================================================================*/
void QX40x_GetAdcReading(u8 ADC_EN, u8 ADC_CFG)
{
    Cmd[0] = GET_ADC_READING;	
	Cmd[1] = ADC_EN;	
	Cmd[2] = ADC_CFG;
	QX40x_SendCmdGetResp(Cmd,3,Resp,6);
}

/*!=============================================================
  * @brief : Returns the interrupt status of the Packet Handler 
  * @param :
*			PH_CLR_PEND: Flags to clear.
   ============================================================*/
void QX40x_GetPhStatus(u8 PH_CLR_PEND)
{
    Cmd[0] = GET_PH_STATUS;	
	Cmd[1] = PH_CLR_PEND;	
	QX40x_SendCmdGetResp(Cmd,2,Resp,2);
}

/*!====================================================
  * @brief : Returns the interrupt status of the modem.
  * @param :
*			PH_CLR_PEND: Flags to clear.
   ===================================================*/
void QX40x_GetModemStatus(u8 MODEM_CLR_PEND)
{
    Cmd[0] = GET_MODEM_STATUS;	
	Cmd[1] = MODEM_CLR_PEND;	
	QX40x_SendCmdGetResp(Cmd,2,Resp,8);
}

/*!===================================================
  * @brief : Returns the interrupt status of the Chip.
  * @param :
*			PH_CLR_PEND: Flags to clear.
   ==================================================*/
void QX40x_GetChipStatus(u8 CHIP_CLR_PEND)
{
    Cmd[0] = GET_CHIP_STATUS;	
	Cmd[1] = CHIP_CLR_PEND;	
	QX40x_SendCmdGetResp(Cmd,2,Resp,4);
}

/*!=====================================================================
  * @brief : Performs image rejection calibration. Completion can be 
	monitored by polling CTS or waiting for CHIP_READY interrupt source.
  * @param :
   ====================================================================*/
void QX40x_Ircal(u8 STEP_SIZE,u8 RSSI_AVG,u8 RX_CHAIN_SETTING1,u8 RX_CHAIN_SETTING2)
{
    Cmd[0] = IRCAL;	
	Cmd[1] = STEP_SIZE;	
	Cmd[2] = RSSI_AVG;	
	Cmd[3] = RX_CHAIN_SETTING1;	
	Cmd[4] = RX_CHAIN_SETTING2;	
    QX40x_WriteCmd(Cmd,5);
}

/*!===========================================================
  * @brief : Forces a specific value for IR calibration, and 
	reads back calibration values from previous calibrations.
  * @param : 
   ==========================================================*/
void QX40x_IrcalManual(u8 IRCAL_AMP, u8 IRCAL_PH)
{
	Cmd[0] = IRCAL_MANUAL;
	Cmd[1] = IRCAL_AMP;
	Cmd[2] = IRCAL_PH;
	QX40x_SendCmdGetResp(Cmd,3,Resp,2);
}

/*!==========================================================
  * @brief : Manually hop to a new frequency while in RX mode.
  * @param / INTE : New INTE register value.
  * @param / FRAC2 : New FRAC2 register value.
  * @param / FRAC1 : New FRAC1 register value.
  * @param / FRAC0 : New FRAC0 register value.
  * @param / VCO_CNT1 : New VCO_CNT1 register value.
  * @param / VCO_CNT0 : New VCO_CNT0 register value.
   ==========================================================*/
void QX40x_RxHop(u8 INTE,u8 FRAC2,u8 FRAC1,u8 FRAC0,u8 VCO_CNT1,u8 VCO_CNT0)
{
    Cmd[0] = RX_HOP;	
	Cmd[1] = INTE;
	Cmd[2] = FRAC2;
	Cmd[3] = FRAC1;
	Cmd[4] = FRAC0;
	Cmd[5] = VCO_CNT1;
	Cmd[6] = VCO_CNT0;	
    QX40x_WriteCmd(Cmd,7);
}	

/*!================================================================
  * @brief : While in TX state this will ramp down the PA, 
						Retune the PLL, and ramp the PA back up.
  * @param / INTE  : INTE register value.
  * @param / FRAC2 : FRAC register value.
  * @param / FRAC1 : FRAC register value.
  * @param / FRAC0 : FRAC register value.
  * @param / VCO_CNT1 : VCO_CNT register value.
  * @param / VCO_CNT0 : VCO_CNT register value.
  * @param / PLL_SETTLE_TIME1 : Delay in us to leave PA ramped down.
  * @param / PLL_SETTLE_TIME0 : Delay in us to leave PA ramped down.
   ================================================================*/
void QX40x_TxHop(u8 INTE,u8 FRAC2,u8 FRAC1,u8 FRAC0,u8 VCO_CNT1,u8 VCO_CNT0
										,u8 PLL_SETTLE_TIME1,u8 PLL_SETTLE_TIME0)
{
    Cmd[0] = TX_HOP;	
	Cmd[1] = INTE;
	Cmd[2] = FRAC2;
	Cmd[3] = FRAC1;
	Cmd[4] = FRAC0;
	Cmd[5] = VCO_CNT1;
	Cmd[6] = VCO_CNT0;	
	Cmd[7] = PLL_SETTLE_TIME1;
	Cmd[8] = PLL_SETTLE_TIME0;
    QX40x_WriteCmd(Cmd,9);
}

/*!=================================================
  * @brief :Obtain the current status of the device.
 ==================================================*/
void QX40x_RequestDeviceStatus(void)
{ 
    Cmd[0] = REQUEST_DEVICE_STATE;
	QX40x_SendCmdGetResp(Cmd,1,Resp,2);
}

// Sends START_TX command ID to the radio with no input parameters
void QX40x_StartTxFast(void)
{
    Cmd[0] = START_TX;
    QX40x_WriteCmd(Cmd,1);
}

// Sends START_RX command ID to the radio with no input parameters
void QX40x_StartRxFast(void)
{
	Cmd[0] = START_RX;
    QX40x_WriteCmd(Cmd,1);
}

// Clear all Interrupt status/pending flags. Does NOT read back interrupt flags
void QX40x_GetIntStatusFastClear(void)
{
    Cmd[0] = GET_INT_STATUS;
    QX40x_WriteCmd(Cmd,1);
}

// Clear and read all Interrupt status/pending flags
void QX40x_GetGntStatusFastClearRead(void)
{
    Cmd[0] = GET_INT_STATUS;
    QX40x_SendCmdGetResp(Cmd,1,Resp,8);
}

// Reads back current GPIO pin configuration. Does NOT configure GPIO pins
void QX40x_GpioPinCfgFast(void)
{
    Cmd[0] = GPIO_PIN_CFG;
    QX40x_SendCmdGetResp(Cmd,1,Resp,7);
}


// Clear all Packet Handler status flags. Does NOT read back interrupt flags
void QX40x_GetPhStatusFastClear(void)
{
    Cmd[0] = GET_PH_STATUS;
    Cmd[1] = 0;
    QX40x_WriteCmd(Cmd,2);
}

// Clear and read all Packet Handler status flags.
void QX40x_GetPhStatusFastClearRead(void)
{
    Cmd[0] = GET_PH_STATUS;
    QX40x_SendCmdGetResp(Cmd,1,Resp,2);
}

// Clear all Modem status flags. Does NOT read back interrupt flags
void QX40x_GetModemStatusFastClear(void)
{
    Cmd[0] = GET_MODEM_STATUS;
    Cmd[1] = 0;
    QX40x_WriteCmd(Cmd,2);
}

// Clear and read all Modem status flags.
void QX40x_GetModemStatusFastClearRead(void)
{
    Cmd[0] = GET_MODEM_STATUS;
    QX40x_SendCmdGetResp(Cmd,1,Resp,8);
}

// Clear all Chip status flags. Does NOT read back interrupt flags
void QX40x_GetChipStatusFastClear(void)
{
    Cmd[0] = GET_CHIP_STATUS;
    Cmd[1] = 0;
    QX40x_WriteCmd(Cmd,1);
}

// Clear and read all Chip status flags.
void QX40x_GetChipStatusFastClearRead(void)
{
    Cmd[0] = GET_CHIP_STATUS;
	QX40x_SendCmdGetResp(Cmd,1,Resp,3);
}

// Resets the RX/TX FIFO. Does not read back anything from TX/RX FIFO
void QX40x_FifoInfoFastReset(u8 FIFO)
{
    Cmd[0] = FIFO_INFO;
    Cmd[1] = FIFO;
    QX40x_WriteCmd(Cmd,2);
}

// Reads RX/TX FIFO count space. Does NOT reset RX/TX FIFO
void QX40x_FifoInfoFastRead(void)
{
    Cmd[0] = FIFO_INFO;
    QX40x_SendCmdGetResp(Cmd,1,Resp,2);
}

//packet TX
void Radio_StartTxPacket(u8 channel, u8 *pRadioPacket, u8 length)
{
	/* Leave RX state */
	QX40x_ChangeStatus(3);
	/* Read ITs, clear pending ones */
	QX40x_GetIntStatus(0, 0, 0);
	/* Reset the Tx Fifo */
	QX40x_FifoInfo(0x01);
	/* Fill the TX fifo with datas */
	QX40x_WriteTxFifo(pRadioPacket,length);
	/* Start sending packet, channel 0, START immediately */
	QX40x_StartTx(channel, 0x80, length);
}

// reset the QX40x
void QX40x_Reset( void )
{
	QX_SET_SDN_HIGH( );		//shutdown SDN
	delay_us(15);			//at least 10us 
	QX_SET_SDN_LOW( );		//shutup SDN
	delay_ms(20);			//at least 10ms
}

/*!====================================
  * @brief :QX40x Pin initialization
   ===================================*/
void QX40x_GpioInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//sdn cofigure
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = QX4463_SDN_PIN;
	GPIO_Init( QX4463_SDN_PORT, &GPIO_InitStructure );
	//IRQ configure
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Pin = QX4463_IRQ_PIN;
	GPIO_Init( QX4463_IRQ_PORT, &GPIO_InitStructure );
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);//IRQ

	EXTI_InitTypeDef EXTI_InitStructure;	
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0D;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/*!================================================================
  * @brief : propertys fonfigure.
   ===============================================================*/
void _QX40x_SetProp(_PROP prop)
{
	Cmd[0] = SET_PROPERTY;
	Cmd[1] = prop.id>>8;
	Cmd[2] = 1;
	Cmd[3] = (u8)prop.id;
	Cmd[4] = prop.val;
	QX40x_WriteCmd(Cmd,5);
}

void Radio_SetPros(const _PROP* prop,u16 len)
{
	for(u8 i=0;i<len;i++)
	{
		_QX40x_SetProp(*(prop+i));
	}
}
/*!================================================================
  * @brief :QX40x init
   ================================================================*/
void QX40x_Config_Init( void )
{
    u8 i;
    uint16_t j = 0;

    while( ( i = Configure_array[j] ) != 0 )
    {
        j += 1;
        QX40x_WriteCmd( (u8 *)Configure_array + j, i );
        j += i;
    }
}

void Radio_StartTx_Packet(u8 channel, u8 *pRadioPacket, u8 length)
{
	/* Leave RX state */
	QX40x_ChangeStatus(3);	
	/* Read ITs, clear pending ones */
	QX40x_GetIntStatus(0, 0, 0);	
	/* Reset the Tx Fifo */
	QX40x_FifoInfo(0x01);
	/* Fill the TX fifo with datas */
	QX40x_WriteTxFifo(pRadioPacket,length);
	/* Start sending packet, channel 0, START immediately */
	QX40x_StartTx(channel, 0x80, length);
}

void Radio_StartRx( u8 Channel, u8 Condition, u16 Length, 
							u8 Next_State1, u8 Next_State2, u8 Next_State3 )
{
	QX40x_GetIntStatus(0,0,0);
    QX40x_FifoInfo(0x02);
    Cmd[ 0 ] = START_RX;
    Cmd[ 1 ] = Channel;
    Cmd[ 2 ] = Condition;
    Cmd[ 3 ] = Length >> 8;
    Cmd[ 4 ] = Length;
    Cmd[ 5 ] = Next_State1;
    Cmd[ 6 ] = Next_State2;
    Cmd[ 7 ] = Next_State3;	
    QX40x_WriteCmd(Cmd, 8);
}

void QX40x_Init( void )
{
	QX40x_GpioInit( );			//SDN GPIO init
//	QX40x_Reset( );				//QX40x rest
//	QX40x_PowerUp(0x01,0x00,30000000 );	//powerup 30MHz	
//	QX40x_Config_Init();
	
}



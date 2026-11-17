
#ifndef __QX4463_H__
#define __QX4463_H__

#include "spi.h"
#include "delay.h"
#include "LCD.h"
#include <stdio.h>

// SDN: PA11
#define QX4463_SDN_PORT				GPIOA							
#define QX4463_SDN_CLK				RCC_APB2Periph_GPIOA
#define QX4463_SDN_PIN				GPIO_Pin_11

// IRQ: PA12
#define QX4463_IRQ_PORT				GPIOA								
#define QX4463_IRQ_CLK				RCC_APB2Periph_GPIOA
#define QX4463_IRQ_PIN				GPIO_Pin_12

//IO Operation function definition
#define QX_SET_SDN_HIGH( )			QX4463_SDN_PORT->ODR |= QX4463_SDN_PIN
#define QX_SET_SDN_LOW( )			QX4463_SDN_PORT->ODR &= (u32)((u32)( ~QX4463_SDN_PIN ))

#define QX_SET_CSN_HIGH( )			SPI_Set_NSS_High( )
#define QX_SET_CSN_LOW( )			SPI_Set_NSS_Low( )


//BOOT COMMANDS ============================
#define   POWER_UP                 0x02 
//COMMON COMMANDS ==========================
#define   NOP                      0x00 
#define   PART_INFO                0x01 
#define	  BOOT_FACT_INFO	       0x09
#define   FUNC_INFO                0x10 
#define   GET_INT_STATUS           0x20                                                                     
#define   SET_PROPERTY             0x11 
#define   GET_PROPERTY             0x12 
#define   FIFO_INFO                0x15 
#define   GPIO_PIN_CFG             0x13 
#define   CHANGE_STATE             0x34
#define   REQUEST_DEVICE_STATE     0x33 
#define   READ_CMD_BUFF            0x44 
#define   FRR_A_READ               0x50 
#define   FRR_B_READ               0x51 
#define   FRR_C_READ               0x53 
#define   FRR_D_READ               0x57 
//IR_CAL_COMMANDS ==========================
#define   IRCAL                    0x17 
#define   IRCAL_MANUAL             0x1a
//TX_COMMANDS ==============================
#define   START_TX                 0x31 
#define   WRITE_TX_FIFO            0x66 
#define   TX_HOP                   0x37
//RX_COMMANDS ==============================
#define   GET_MODEM_STATUS         0x22                                                                      
#define   PACKET_INFO              0x16                                   
#define   START_RX                 0x32                                   
#define   RX_HOP                   0x36
#define   READ_RX_FIFO             0x77 
//ADVANCED_COMMANDS ========================
#define   GET_PH_STATUS            0x21                                    
#define   GET_CHIP_STATUS          0x23
#define   PROTOCOL_CFG             0x18
#define   GET_ADC_READING          0x14  

//QX4463 property =========================================
#define  GLOBAL_XO_TUNE							0x0000
#define  GLOBAL_CLK_CFG							0x0001
#define  GLOBAL_LOW_BATT_THRESH					0x0002
#define  GLOBAL_CONFIG							0x0003
#define  GLOBAL_WUT_CONFIG						0x0004
#define  GLOBAL_WUT_M							0x0005
#define  GLOBAL_WUT_M_L							0x0006
#define  GLOBAL_WUT_R							0x0007
#define  GLOBAL_WUT_LDC							0x0008
#define  GLOBAL_WUT_CAL							0x0009

#define  INT_CTL_ENABLE							0x0100
#define  INT_CTL_PH_ENABLE						0x0101
#define  INT_CTL_MODEM_ENABLE					0x0102
#define  INT_CTL_CHIP_ENABLE					0x0103

#define  FRR_CTL_A_MODE							0x0200
#define  FRR_CTL_B_MODE							0x0201
#define  FRR_CTL_C_MODE							0x0202
#define  FRR_CTL_D_MODE							0x0203

#define  PREAMBLE_TX_LENGTH						0x1000
#define  PREAMBLE_CONFIG_STD_1					0x1001
#define  PREAMBLE_CONFIG_NSTD					0x1002
#define  PREAMBLE_CONFIG_STD_2					0x1003
#define  PREAMBLE_CONFIG						0x1004
#define  PREAMBLE_PATTERN_31_24			0x100A
#define  PREAMBLE_PATTERN_23_16			0x100B
#define  PREAMBLE_PATTERN_15_8			0x100C
#define  PREAMBLE_PATTERN_7_0			0x100D
#define  PREAMBLE_POSTAMBLE_CONFIG				0x1009
#define  PREAMBLE_POSTAMBLE_PATTERN				0x100A
#define  PREAMBLE_POSTAMBLE_PATTERN_1			0x100B
#define  PREAMBLE_POSTAMBLE_PATTERN_2			0x100C
#define  PREAMBLE_POSTAMBLE_PATTERN_3			0x100D

#define  SYNC_CONFIG							0x1100
#define  SYNC_BITS_31_24						0x1101
#define  SYNC_BITS_23_16						0x1102
#define  SYNC_BITS_15_8							0x1103
#define  SYNC_BITS_7_0							0x1104
#define  SYNC_CONFIG2							0x1105

#define  PKT_CRC_CONFIG							0x1200
#define  PKT_WHT_POLY_15_8						0x1201
#define  PKT_WHT_POLY_7_0						0x1202
#define  PKT_WHT_SEED_15_8						0x1203
#define  PKT_WHT_SEED_7_0						0x1204
#define  PKT_WHT_BIT_NUM						0x1205
#define  PKT_CONFIG1							0x1206
#define  PKT_CONFIG2							0x1207
#define  PKT_LEN								0x1208
#define  PKT_LEN_FIELD_SOURCE					0x1209
#define  PKT_LEN_ADJUST							0x120A
#define  PKT_TX_THRESHOLD						0x120B
#define  PKT_RX_THRESHOLD						0x120C

#define  PKT_FIELD_1_LENGTH_12_8				0x120D
#define  PKT_FIELD_1_LENGTH_7_0					0x120E
#define  PKT_FIELD_1_CONFIG						0x120F
#define  PKT_FIELD_1_CRC_CONFIG					0x1210
#define  PKT_FIELD_2_LENGTH_12_8				0x1211
#define  PKT_FIELD_2_LENGTH_7_0					0x1212
#define  PKT_FIELD_2_CONFIG						0x1213
#define  PKT_FIELD_2_CRC_CONFIG					0x1214
#define  PKT_FIELD_3_LENGTH_12_8				0x1215
#define  PKT_FIELD_3_LENGTH_7_0					0x1216
#define  PKT_FIELD_3_CONFIG						0x1217
#define  PKT_FIELD_3_CRC_CONFIG					0x1218
#define  PKT_FIELD_4_LENGTH_12_8				0x1219
#define  PKT_FIELD_4_LENGTH_7_0					0x121A
#define  PKT_FIELD_4_CONFIG						0x121B
#define  PKT_FIELD_4_CRC_CONFIG					0x121C
#define  PKT_FIELD_5_LENGTH_12_8				0x121D
#define  PKT_FIELD_5_LENGTH_7_0					0x121E
#define  PKT_FIELD_5_CONFIG						0x121F
#define  PKT_FIELD_5_CRC_CONFIG					0x1220
#define  PKT_RX_FIELD_1_LENGTH_12_8				0x1221
#define  PKT_RX_FIELD_1_LENGTH_7_0				0x1222
#define  PKT_RX_FIELD_1_CONFIG					0x1223
#define  PKT_RX_FIELD_1_CRC_CONFIG				0x1224
#define  PKT_RX_FIELD_2_LENGTH_12_8				0x1225
#define  PKT_RX_FIELD_2_LENGTH_7_0				0x1226
#define  PKT_RX_FIELD_2_CONFIG					0x1227
#define  PKT_RX_FIELD_2_CRC_CONFIG				0x1228
#define  PKT_RX_FIELD_3_LENGTH_12_8				0x1229
#define  PKT_RX_FIELD_3_LENGTH_7_0				0x122A
#define  PKT_RX_FIELD_3_CONFIG					0x122B
#define  PKT_RX_FIELD_3_CRC_CONFIG				0x122C
#define  PKT_RX_FIELD_4_LENGTH_12_8				0x122D
#define  PKT_RX_FIELD_4_LENGTH_7_0				0x122E
#define  PKT_RX_FIELD_4_CONFIG					0x122F
#define  PKT_RX_FIELD_4_CRC_CONFIG				0x1230
#define  PKT_RX_FIELD_5_LENGTH_12_8				0x1231
#define  PKT_RX_FIELD_5_LENGTH_7_0				0x1232
#define  PKT_RX_FIELD_5_CONFIG					0x1233
#define  PKT_RX_FIELD_5_CRC_CONFIG				0x1234
#define  PKT_CRC_SEED_31_24						0x1236
#define  PKT_CRC_SEED_23_16						0x1237
#define  PKT_CRC_SEED_15_8						0x1238
#define  PKT_CRC_SEED_7_0						0x1239

#define  MODEM_MOD_TYPE								0x2000
#define  MODEM_MAP_CONTROL							0x2001
#define  MODEM_DSM_CTRL								0x2002
#define  MODEM_DATA_RATE_2 							0x2003
#define  MODEM_DATA_RATE_1 							0x2004
#define  MODEM_DATA_RATE_0 							0x2005
#define  MODEM_TX_NCO_MODE_3						0x2006
#define  MODEM_TX_NCO_MODE_2 						0x2007
#define  MODEM_TX_NCO_MODE_1						0x2008
#define  MODEM_TX_NCO_MODE_0 						0x2009
#define  MODEM_FREQ_DEV_2 							0x200A
#define  MODEM_FREQ_DEV_1 							0x200B
#define  MODEM_FREQ_DEV_0 							0x200C
#define  MODEM_FREQ_OFFSET 							0x200D
#define  MODEM_FREQ_OFFSET_1 						0x200E
#define  MODEM_TX_FILTER_COEFF_8					0x200F
#define  MODEM_TX_FILTER_COEFF_7					0x2010
#define  MODEM_TX_FILTER_COEFF_6					0x2011
#define  MODEM_TX_FILTER_COEFF_5					0x2012
#define  MODEM_TX_FILTER_COEFF_4					0x2013
#define  MODEM_TX_FILTER_COEFF_3					0x2014
#define  MODEM_TX_FILTER_COEFF_2					0x2015
#define  MODEM_TX_FILTER_COEFF_1					0x2016
#define  MODEM_TX_FILTER_COEFF_0					0x2017
#define  MODEM_TX_RAMP_DELAY						0x2018
#define  MODEM_MDM_CTRL								0x2019
#define  MODEM_IF_CONTROL							0x201A
#define  MODEM_IF_FREQ_2							0x201B
#define  MODEM_IF_FREQ_1							0x201C
#define  MODEM_IF_FREQ_0							0x201D
#define  MODEM_DECIMATION_CFG1						0x201E
#define  MODEM_DECIMATION_CFG0						0x201F
#define  MODEM_DECIMATION_CFG2						0x2020
#define  MODEM_IFPKD_THRESHOLDS						0x2021
#define  MODEM_BCR_OSR_1							0x2022
#define  MODEM_BCR_OSR_0							0x2023

#define  MODEM_BCR_NCO_OFFSET_2						0x2024
#define  MODEM_BCR_NCO_OFFSET_1						0x2025
#define  MODEM_BCR_NCO_OFFSET_0						0x2026
#define  MODEM_BCR_GAIN_1							0x2027
#define  MODEM_BCR_GAIN_0							0x2028
#define  MODEM_BCR_GEAR								0x2029
#define  MODEM_BCR_MISC1							0x202A
#define  MODEM_BCR_MISC0							0x202B
#define  MODEM_AFC_GEAR								0x202C
#define  MODEM_AFC_WAIT								0x202D
#define  MODEM_AFC_GAIN_1							0x202E
#define  MODEM_AFC_GAIN_0							0x202F

#define  MODEM_AFC_LIMITER_1						0x2030
#define  MODEM_AFC_LIMITER_0						0x2031
#define  MODEM_AFC_MISC								0x2032
#define  MODEM_AFC_ZIFOFF							0x2033
#define  MODEM_ADC_CTRL								0x2034
#define  MODEM_AGC_CONTROL							0x2035
#define  MODEM_AGC_WINDOW_SIZE						0x2038
#define  MODEM_AGC_RFPD_DECAY						0x2039
#define  MODEM_AGC_IFPD_DECAY						0x203A
#define  MODEM_FSK4_GAIN1							0x203B
#define  MODEM_FSK4_GAIN0							0x203C
#define  MODEM_FSK4_TH1								0x203D
#define  MODEM_FSK4_TH0								0x203E
#define  MODEM_FSK4_MAP								0x203F
#define  MODEM_OOK_PDTC								0x2040
#define  MODEM_OOK_BLOPK							0x2041
#define  MODEM_OOK_CNT1								0x2042
#define  MODEM_OOK_MISC								0x2043
#define  MODEM_RAW_CONTROL							0x2045
#define  MODEM_RAW_EYE_1							0x2046
#define  MODEM_RAW_EYE_0							0x2047
#define  MODEM_ANT_DIV_MODE							0x2048
#define  MODEM_ANT_DIV_CONTROL						0x2049
#define  MODEM_RSSI_THRESH							0x204A
#define  MODEM_RSSI_JUMP_THRESH						0x204B
#define  MODEM_RSSI_CONTROL							0x204C
#define  MODEM_RSSI_CONTROL2						0x204D
#define  MODEM_RSSI_COMP							0x204E
#define  MODEM_RAW_SEARCH2							0x2050
#define  MODEM_CLKGEN_BAND							0x2051
#define  MODEM_SPIKE_DET							0x2054
#define  MODEM_ONE_SHOT_AFC							0x2055
#define  MODEM_RSSI_HYSTERESIS						0x2056
#define  MODEM_RSSI_MUTE							0x2057
#define  MODEM_FAST_RSSI_DELAY						0x2058
#define  MODEM_PSM									0x2059
#define  MODEM_PSM_1								0x205A
#define  MODEM_DSA_CTRL1							0x205B
#define  MODEM_DSA_CTRL2							0x205C
#define  MODEM_DSA_QUAL								0x205D
#define  MODEM_DSA_RSSI								0x205E
#define  MODEM_DSA_MISC								0x205F

#define	MODEM_CHFLT_RX1_CHFLT_COE13_7_0 			0x2100
#define	MODEM_CHFLT_RX1_CHFLT_COE12_7_0 			0x2101
#define	MODEM_CHFLT_RX1_CHFLT_COE11_7_0 			0x2102
#define	MODEM_CHFLT_RX1_CHFLT_COE10_7_0 			0x2103
#define	MODEM_CHFLT_RX1_CHFLT_COE9_7_0 				0x2104
#define	MODEM_CHFLT_RX1_CHFLT_COE8_7_0 				0x2105	
#define	MODEM_CHFLT_RX1_CHFLT_COE7_7_0 				0x2106	
#define	MODEM_CHFLT_RX1_CHFLT_COE6_7_0 				0x2107
#define	MODEM_CHFLT_RX1_CHFLT_COE5_7_0				0x2108		
#define	MODEM_CHFLT_RX1_CHFLT_COE4_7_0 				0x2109						
#define	MODEM_CHFLT_RX1_CHFLT_COE3_7_0				0x210A		
#define	MODEM_CHFLT_RX1_CHFLT_COE2_7_0				0x210B	
#define	MODEM_CHFLT_RX1_CHFLT_COE1_7_0 				0x210C	
#define	MODEM_CHFLT_RX1_CHFLT_COE0_7_0 				0x210D

#define	MODEM_CHFLT_RX1_CHFLT_COEM0 				0x210E
#define	MODEM_CHFLT_RX1_CHFLT_COEM1 				0x210F
#define	MODEM_CHFLT_RX1_CHFLT_COEM2 				0x2110
#define	MODEM_CHFLT_RX1_CHFLT_COEM3 				0x2111

#define	MODEM_CHFLT_RX2_CHFLT_COE13_7_0 			0x2112	
#define	MODEM_CHFLT_RX2_CHFLT_COE12_7_0 			0x2113
#define	MODEM_CHFLT_RX2_CHFLT_COE11_7_0				0x2114	
#define	MODEM_CHFLT_RX2_CHFLT_COE10_7_0 			0x2115						
#define	MODEM_CHFLT_RX2_CHFLT_COE9_7_0				0x2116		
#define	MODEM_CHFLT_RX2_CHFLT_COE8_7_0				0x2117
#define	MODEM_CHFLT_RX2_CHFLT_COE7_7_0 				0x2118	
#define	MODEM_CHFLT_RX2_CHFLT_COE6_7_0 				0x2119
#define	MODEM_CHFLT_RX2_CHFLT_COE5_7_0				0x211A		
#define	MODEM_CHFLT_RX2_CHFLT_COE4_7_0 				0x211B						
#define	MODEM_CHFLT_RX2_CHFLT_COE3_7_0				0x211C		
#define	MODEM_CHFLT_RX2_CHFLT_COE2_7_0				0x211D
#define	MODEM_CHFLT_RX2_CHFLT_COE1_7_0 				0x211E	
#define	MODEM_CHFLT_RX2_CHFLT_COE0_7_0 				0x211F

#define	MODEM_CHFLT_RX2_CHFLT_COEM0					0x2120		
#define	MODEM_CHFLT_RX2_CHFLT_COEM1 				0x2121						
#define	MODEM_CHFLT_RX2_CHFLT_COEM2					0x2122		
#define	MODEM_CHFLT_RX2_CHFLT_COEM3					0x2123

#define  PA_MODE									0x2200
#define  PA_PWR_LVL									0x2201
#define  PA_BIAS_CLKDUTY							0x2202
#define  PA_TC										0x2203
#define  PA_RAMP_EX									0x2204
#define  PA_RAMP_DOWN_DELAY							0x2205
#define  PA_DIG_PWR_SEQ_CONFIG						0x2206

#define  SYNTH_PFDCP_CPFF							0x2300
#define  SYNTH_PFDCP_CPINT							0x2301
#define  SYNTH_VCO_KV								0x2302
#define  SYNTH_LPFILT3								0x2303
#define  SYNTH_LPFILT2								0x2304
#define  SYNTH_LPFILT1								0x2305
#define  SYNTH_LPFILT0								0x2306
#define  SYNTH_VCO_KVCAL							0x2307

#define  MATCH_VALUE_1								0x3000
#define  MATCH_MASK_1								0x3001
#define  MATCH_CTRL_1								0x3002
#define  MATCH_VALUE_2								0x3003
#define  MATCH_MASK_2								0x3004
#define  MATCH_CTRL_2								0x3005
#define  MATCH_VALUE_3								0x3006
#define  MATCH_MASK_3								0x3007
#define  MATCH_CTRL_3								0x3008
#define  MATCH_VALUE_4								0x3009
#define  MATCH_MASK_4								0x300A
#define  MATCH_CTRL_4								0x300B

#define  FREQ_CONTROL_INTE							0x4000
#define  FREQ_CONTROL_FRAC_2						0x4001
#define  FREQ_CONTROL_FRAC_1						0x4002
#define  FREQ_CONTROL_FRAC_0						0x4003
#define  FREQ_CONTROL_CHANNEL_STEP_SIZE_1			0x4004
#define  FREQ_CONTROL_CHANNEL_STEP_SIZE_0			0x4005
#define  FREQ_CONTROL_W_SIZE						0x4006
#define  FREQ_CONTROL_VCOCNT_RX_ADJ					0x4007

#define  RX_HOP_CONTROL								0x5000
#define  RX_HOP_TABLE_SIZE							0x5001
#define  RX_HOP_TABLE_ENTRY							0x5002

#define  PTI_CTL									0xF000
#define  PTI_BAUD									0xF001
#define  PTI_BAUD_1									0xF002
#define  PTI_LOG_EN									0xF003


typedef struct 
{
	u16 id;
	u8 val;
}_PROP;

void QX40x_Reset(void);
void QX40x_GpioInit(void);
void Radio_SetPros(const _PROP* prop,u16 len);
void QX40x_Config_Init( void );
void Radio_StartTx_Packet(u8 channel, u8 *pRadioPacket, u8 length);
void QX40x_Init( void );
void Radio_StartRx( u8 Channel, u8 Condition, u16 Length, 
							u8 Next_State1, u8 Next_State2, u8 Next_State3 );

//========================Function ==============================
void QX40x_WaitCts(void);
void QX40x_WriteCmd(u8 *pCmd,u8 CmdNum);
void QX40x_ReadResponse(u8 *pRead,u8 NumBytes);
void QX40x_FastRead(u8 Cmd,u8 *pRead, u8 NumBytes);
void QX40x_SendCmds(u8 *Cmd,u8 CmdNumber,u8 *Read,u8 ReadNumber);
//boot commands
void QX40x_PowerUp(u8 BOOT_OPTIONS, u8 XTAL_OPTIONS, u32 XO_FREQ);
//common commands
void QX40x_Nop(void);
void QX40x_GpioPinCfg(u8 GPIO0,u8 GPIO1,u8 GPIO2,u8 GPIO3,u8 IRQ,u8 SDO,u8 GEN_CONFIG);
void QX40x_BootFactoryInfo(void);
void QX40x_PartInfo(void);
void QX40x_FunInfo(void);
u8 QX40x_FifoInfo(u8 FIFO);
void QX40x_SetProperty(u16 property, u8 value);
void QX40x_SetPropertys(u8 Group,u8 NumProps,u8 SartProp,u8 *pdata);
void QX40x_GetProperty(u16 property);
void QX40x_GetPropertys(u8 Group,u8 NumProps,u8 SartProp,u8 *pRead);
void QX40x_ChangeStatus(u8 NextStatus);
void QX40x_GetIntStatus(u8 ph_clr_pend,u8 modem_clr_pend,u8 chip_clr_pend);
void QX40x_RequestDeviceStatus(void);
void QX40x_FrrARead(u8 RespByteNum);
void QX40x_FrrBRead(u8 RespByteNum);
void QX40x_FrrCRead(u8 RespByteNum);
void QX40x_FrrDRead(u8 RespByteNum);
//Ir cal commands
void QX40x_Ircal(u8 STEP_SIZE,u8 RSSI_AVG,u8 RX_CHAIN_SETTING1,u8 RX_CHAIN_SETTING2);
void QX40x_IrcalManual(u8 IRCAL_AMP, u8 IRCAL_PH);
//TX commands
void QX40x_WriteTxFifo(u8 *pWriteData,u8 NumBytes);
void QX40x_StartTx(u8 Channel, u8 Condition, u16 Tx_Len);
void QX40x_TxHop(u8 INTE,u8 FRAC2,u8 FRAC1,u8 FRAC0,u8 VCO_CNT1,u8 VCO_CNT0,u8 PLL_SETTLE_TIME1,u8 PLL_SETTLE_TIME0);
//RX commands
void QX40x_GetModemStatus(u8 MODEM_CLR_PEND);
void QX40x_StartRx(u8 Channel,u8 Condition,u16 Length,u8 Next_State1,u8 Next_State2,u8 Next_State3);
void QX40x_RxHop(u8 INTE,u8 FRAC2,u8 FRAC1,u8 FRAC0,u8 VCO_CNT1,u8 VCO_CNT0);
void QX40x_ReadRxFifo(u8 *pRxData ,u8 NumBytes);
void QX40x_GetPacketInfo(u8 FieldNum,u16 Len,u16 Len_Diff);
//advanced commands
void QX40x_GetAdcReading(u8 ADC_EN, u8 ADC_CFG);
void QX40x_GetPhStatus(u8 PH_CLR_PEND);
void QX40x_GetChipStatus(u8 CHIP_CLR_PEND);
//advanced commands
void QX40x_StartTxFast(void);
void QX40x_StartRxFast(void);
void QX40x_GetIntStatusFastClear(void);
void QX40x_GetGntStatusFastClearRead(void);
void QX40x_GpioPinCfgFast(void);
void QX40x_GetPhStatusFastClear(void);
void QX40x_GetPhStatusFastClearRead(void);
void QX40x_GetModemStatusFastClear(void);
void QX40x_GetModemStatusFastClearRead(void);
void QX40x_GetChipStatusFastClear(void);
void QX40x_GetChipStatusFastClearRead(void);
void QX40x_FifoInfoFastReset(u8 FIFO);
void QX40x_FifoInfoFastRead(void);

#endif

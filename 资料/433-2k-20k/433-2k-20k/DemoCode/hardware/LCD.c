#include "lcd.h"
#include "delay.h"
#include "font.h"

void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	////////////////////////////////////////////////////////////
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1| RCC_APB2Periph_GPIOA,  ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;	//PA4,PA3,PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 
	LCD_CS = 1;
	LCD_DC = 1;	 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//SPI_BaudRatePrescaler_16;	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	
	SPI_InitStructure.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStructure);  
	SPI_Cmd(SPI1, ENABLE); 
}

//////////////////////////////////////////////
u8 ST7735S_SPIWriteByte(u8 Byte)
{
	while((SPI1->SR&0x02)==0);		 
	SPI1->DR=Byte;	 	            
	while((SPI1->SR&0x01)==0);     
	return SPI1->DR;        	  
}
/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(u8 dat)
{
	LCD_CS = 0;
	ST7735S_SPIWriteByte(dat);
	LCD_CS = 1;
}
/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}
/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA16(u16 dat)
{
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}
/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC = 0;//写命令
	LCD_Writ_Bus(dat);
	LCD_DC = 1;//写数据
}
/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_WR_REG(0x2a);//CASET 列地址设置
	LCD_WR_DATA16(x1);
	LCD_WR_DATA16(x2);
	LCD_WR_REG(0x2b);//RASET 行地址设置
	LCD_WR_DATA16(y1);
	LCD_WR_DATA16(y2);
	LCD_WR_REG(0x2c);//RAMWR 储存器写
}
void LCD_Init(void)
{
	LCD_GPIO_Init();//初始化GPIO

	//板子自带复位电路  无需手动复位
//	LCD_RES_Clr();//复位
//	delay_ms(100);
//	LCD_RES_Set();
//	delay_ms(100);

	LCD_BLK = 1;//打开背光
	delay_ms(50);
	delay_ms(50);

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out
	delay_ms(60);              //Delay 120ms
	delay_ms(60);
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	//Panel Function
	LCD_WR_REG(0xB1);	//In Normal Mode (Full Colors)
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2); //In Idle Mode (8-colors)
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3); //In Partial Mode + Full Colors
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	//------------------------------------End ST7735S Frame Rate---------------------------------//
	LCD_WR_REG(0xB4); //Dot inversion
	LCD_WR_DATA8(0x03);
	//------------------------------------ST7735S Power Sequence---------------------------------//
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0XC0);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_WR_REG(0xC5); //VCOM
	LCD_WR_DATA8(0x1A);
	LCD_WR_REG(0x36); //MADCTL MX, MY, RGB mode
	/*if(USE_HORIZONTAL == 0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL == 1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL == 2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);*/
	LCD_WR_DATA8(0xC0);
	//------------------------------------ST7735S Gamma Sequence---------------------------------//
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2A);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------//
	LCD_WR_REG(0x3A); //COLMOD 65k mode
	LCD_WR_DATA8(0x05); //Interface Format
	LCD_WR_REG(0x29); //DISPON Display on
}
/******************************************************************************
      函数说明：进入SLEEP模式，工作电流约3.1mA
      入口数据：无
      返回值：  无
******************************************************************************/
void LCD_SleepIn(void)
{
	LCD_BLK = 0;//关闭背光
	LCD_WR_REG(0x10); //Sleep in
}
/******************************************************************************
      函数说明：退出SLEEP模式，工作电流约15mA
      入口数据：无
      返回值：  无
******************************************************************************/
void LCD_SleepOut(void)
{
	LCD_BLK = 1;//打开背光
	LCD_WR_REG(0x11); //Sleep out
}
/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Clear(void)
{
	u16 i, j;
	LCD_Address_Set(0, 0, LCD_W - 1, LCD_H - 1); //设置显示范围
	for(i = 0; i < LCD_H; i++)
	{
		for(j = 0; j < LCD_W; j++)
			LCD_WR_DATA16(BLACK);
	}
}

/******************************************************************************
      函数说明：读取N个数据
      入口数据：AddrHigh  写地址高字节
                AddrMid   写地址中字节
                AddrLow   写地址低字节
                *pBuff    读取的数据
                DataLen   读取数据的长度
      返回值：  无
******************************************************************************/
void get_n_bytes_data_from_ROM(u32 Addr, u8 *pBuff, u8 DataLen)
{
	u8 i;
	LCD_CS = 1; //字库片选
	ST7735S_SPIWriteByte(0x03);//写指令
	ST7735S_SPIWriteByte(Addr>>16);//写地址高字节
	ST7735S_SPIWriteByte(Addr>>8);//写地址中字节
	ST7735S_SPIWriteByte(Addr&0xFF);//写地址低字节
	for(i = 0; i < DataLen; i++)
		*(pBuff + i) = ST7735S_SPIWriteByte(0); //读一个字节数据
	LCD_CS = 0;//取消字库片选
}

static u8 FontBuf[130];//字库缓存
static u8 focus;
///////////////////////////////////////////////////////////////////////////////
void LCD_Write_Font(u8 index)
{
	u8 k, shift = 0x80;
	if(focus)
	{
		for(k = 0; k < 8; k++)
		{
			if(FontBuf[index] & shift)
				LCD_WR_DATA16(BLACK);
			else
				LCD_WR_DATA16(WHITE);
			shift >>= 1;
		}
	}
	else
	{
		for(k = 0; k < 8; k++)
		{
			if(FontBuf[index] & shift)
				LCD_WR_DATA16(WHITE);
			else
				LCD_WR_DATA16(BLACK);
			shift >>= 1;
		}
	}
}
/******************************************************************************
      函数说明：显示汉字
      入口数据：x,y      写入的坐标
                zk_num   1:12*12,  2:15*16,  3:24*24,  4:32*32
      返回值：  无
******************************************************************************/
void Display_GB2312(u16 x, u16 y, u8 zk_num)
{
	u8 i;
	switch(zk_num)
	{
		case 1:
			LCD_Address_Set(x, y, x + 15, y + 11);
			for(i = 0; i < 24; i++)
				LCD_Write_Font(i);
			break;  // 12*12
		case 2:
			LCD_Address_Set(x, y, x + 15, y + 15);
			for(i = 0; i < 32; i++)
				LCD_Write_Font(i);
			break;     // 15*16
		case 3:
			LCD_Address_Set(x, y, x + 23, y + 23);
			for(i = 0; i < 72; i++)
				LCD_Write_Font(i);
			break;     // 24*24
		case 4:
			LCD_Address_Set(x, y, x + 31, y + 31);
			for(i = 0; i < 128; i++)
				LCD_Write_Font(i);
			break;    // 32*32
	}
}
/******************************************************************************
      函数说明：显示汉字
      入口数据：zk_num    1:12*12,  2:15*16,  3:24*24,  4:32*32
                x,y       坐标
                text[]    要显示的汉字
      返回值：  无
******************************************************************************/
void Display_GB2312_String(u16 x, u16 y, u8 zk_num, u8 text[])
{
	u8 i = 0;
	u32 FontAddr = 0; //字地址
	u32 BaseAdd = 0; //字库基地址
	u8 n, d; // 不同点阵字库的计算变量
	switch(zk_num)
	{// n个数d：字间距
		case 1 :BaseAdd = 0x00;n = 24;d = 12;break;  // 12*12
		case 2 :BaseAdd = 0x2C9D0;n = 32;d = 16;break;   // 15*16
		case 3 :BaseAdd = 0x68190;n = 72;d = 24;break;   // 24*24
		case 4 :BaseAdd = 0xEDF00;n = 128;d = 32;break;   // 32*32
	}
	while((text[i] > 0x00))
	{
		if(((text[i] >= 0xA1) && (text[i] <= 0xA9)) && (text[i + 1] >= 0xA1))
		{//国标简体（GB2312）汉字在 字库IC中的地址由以下公式来计算：//
			//Address = ((MSB - 0xA1) * 94 + (LSB - 0xA1))*n+ BaseAdd; 分三部取地址///
			FontAddr = (text[i] - 0xA1) * 94;
			FontAddr += (text[i + 1] - 0xA1);
			FontAddr = (u32)((FontAddr * n) + BaseAdd);
			get_n_bytes_data_from_ROM(FontAddr, FontBuf, n ); //取一个汉字的数据，存到"FontBuf[]"
			Display_GB2312(x, y, zk_num); //显示一个汉字到LCD上/
		}
		else if(((text[i] >= 0xB0) && (text[i] <= 0xF7)) && (text[i + 1] >= 0xA1))
		{//国标简体（GB2312） 字库IC中的地址由以下公式来计算：//
			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+846)*n+ BaseAdd; 分三部取地址//
			FontAddr = (text[i] - 0xB0) * 94;
			FontAddr += (text[i + 1] - 0xA1) + 846;
			FontAddr = (u32)((FontAddr * n) + BaseAdd);
			get_n_bytes_data_from_ROM(FontAddr, FontBuf, n ); //取一个汉字的数据，存到"FontBuf[ ]"
			Display_GB2312(x, y, zk_num); //显示一个汉字到LCD上/
		}
		x += d; //下一个字坐标
		i += 2; //下个字符
	}
}

/******************************************************************************
      函数说明：显示ASCII码
      入口数据：x,y      写入的坐标
                zk_num   1:5*7   2:5*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
      返回值：  无
******************************************************************************/
void Display_Asc(u16 x, u16 y, u8 zk_num)
{
	u8 i;
	switch(zk_num)
	{
		case 1:
			LCD_Address_Set(x, y, x + 7, y + 7);
			for(i = 0; i < 7; i++)
				LCD_Write_Font(i);
			break;//5x7 ASCII
		case 2:
			LCD_Address_Set(x, y, x + 7, y + 7);
			for(i = 0; i < 8; i++)
				LCD_Write_Font(i);
			break;   //	  7x8 ASCII
		case 3:
			LCD_Address_Set(x, y, x + 7, y + 11);
			for(i = 0; i < 12; i++)
				LCD_Write_Font(i);
			break;  //  6x12 ASCII
		case 4:
			LCD_Address_Set(x, y, x + 7, y + 15);
			for(i = 0; i < 16; i++)
				LCD_Write_Font(i);
			break;     //  8x16 ASCII
		case 5:
			LCD_Address_Set(x, y, x + 15, y + 24);
			for(i = 0; i < 48; i++)
				LCD_Write_Font(i);
			break;    //  12x24 ASCII
		case 6:
			LCD_Address_Set(x, y, x + 15, y + 31);
			for(i = 0; i < 64; i++)
				LCD_Write_Font(i);
			break;   //  16x32 ASCII
	}
}
/******************************************************************************
      函数说明：显示ASCII码
      入口数据：x,y      写入的坐标
                zk_num   1:5*7   2:5*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                text[]   要显示的字符串
      返回值：  无
******************************************************************************/
void Display_Asc_String(u16 x, u16 y, u16 zk_num, u8 text[])
{
	u8 i = 0;
	u32 FontAddr = 0; //字地址
	u32 BaseAdd = 0; //字库基地址
	u8 n, d; // 不同点阵字库的计算变量
	switch(zk_num)
	{//n个数，d:字间距
		case 1:BaseAdd = 0x1DDF80;n = 8;d = 6;break;	 //	  5x7 ASCII
		case 2:BaseAdd = 0x1DE280;n = 8;d = 8;break;	 //   7x8 ASCII
		case 3:BaseAdd = 0x1DBE00;n = 12;d = 6;break;	 //  6x12 ASCII
		case 4:BaseAdd = 0x1DD780;n = 16;d = 8;break;	 //  8x16 ASCII
		case 5:BaseAdd = 0x1DFF00;n = 48;d = 12;break;	 //  12x24 ASCII
		case 6:BaseAdd = 0x1E5A50;n = 64;d = 16;break;	 //  16x32 ASCII
	}
	while((text[i] > 0x00))
	{
		if((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = 	text[i] - 0x20;
			FontAddr = (u32)((FontAddr * n) + BaseAdd);
			get_n_bytes_data_from_ROM(FontAddr, FontBuf, n ); /*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Asc(x+2, y+4, zk_num); /*显示一个ascii到LCD上 */ //add 4
		}
		i++;  //下个数据
		x += d; //下一个字坐标
	}
}
/******************************************************************************
      函数说明：显示选中ASCII码
      入口数据：x,y      写入的坐标
                zk_num   1:5*7   2:5*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                text[]   要显示的字符串
      返回值：  无
******************************************************************************/
void Display_Asc_StringF(u16 x, u16 y, u16 zk_num, u8 text[])
{
	focus = 1;
	Display_Asc_String(x,y,zk_num,text);
	focus = 0;
}
/******************************************************************************
      函数说明：显示ASCII码(Arial&Times New Roman)
      入口数据：x,y      写入的坐标
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
      返回值：  无
******************************************************************************/
void Display_Arial_TimesNewRoman(u16 x, u16 y, u8 zk_num)
{
	u8 i;
	switch(zk_num)
	{
		case 1:
			LCD_Address_Set(x, y, x + 15, y + 12);
			for(i = 2; i < 26; i++)
				LCD_Write_Font(i);
			break;  //  6x12 ASCII
		case 2:
			LCD_Address_Set(x, y, x + 15, y + 17);
			for(i = 2; i < 34; i++)
				LCD_Write_Font(i);
			break;     //  8x16 ASCII
		case 3:
			LCD_Address_Set(x, y, x + 23, y + 23);
			for(i = 2; i < 74; i++)
				LCD_Write_Font(i);
			break;    //  12x24 ASCII
		case 4:
			LCD_Address_Set(x, y, x + 31, y + 31);
			for(i = 2; i < 130; i++)
				LCD_Write_Font(i);
			break;   //  16x32 ASCII
	}
}
/******************************************************************************
      函数说明：显示ASCII(Arial类型)
      入口数据：x,y      写入的坐标
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                text[]   要显示的字符串
      返回值：  无
******************************************************************************/
void Display_Arial_String(u16 x, u16 y, u16 zk_num, u8 text[])
{
	u8 i = 0;
	u32 FontAddr = 0; //字地址
	u32 BaseAdd = 0; //字库基地址
	u8 n, d; // 不同点阵字库的计算变量
	switch(zk_num)
	{//n:个数，d:字间距
		case 1:BaseAdd = 0x1DC400;n = 26;d = 8;break;	 //  8x12 ASCII(Arial类型)
		case 2:BaseAdd = 0x1DE580;n = 34;d = 10;break;	 //12  12x16 ASCII(Arial类型)
		case 3:BaseAdd = 0x1E22D0;n = 74;d = 16;break;	 //  16x24 ASCII(Arial类型)
		case 4:BaseAdd = 0x1E99D0;n = 130;d = 24;break;	 //  24x32 ASCII(Arial类型)
	}
	while((text[i] > 0x00))
	{
		if((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = 	text[i] - 0x20;
			FontAddr = (u32)((FontAddr * n) + BaseAdd);
			get_n_bytes_data_from_ROM(FontAddr, FontBuf, n ); /*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Arial_TimesNewRoman(x+2, y+4, zk_num); /*显示一个ascii到LCD上 */
		}
		i++;  //下个数据
		x += d; //下一个字坐标
	}
}
/******************************************************************************
      函数说明：显示选中ASCII(Arial类型)
      入口数据：x,y      写入的坐标
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                text[]   要显示的字符串
      返回值：  无
******************************************************************************/
void Display_Arial_StringF(u16 x, u16 y, u16 zk_num, u8 text[])
{
	focus = 1;
	Display_Arial_String(x,y,zk_num,text);
	focus = 0;
}
/******************************************************************************
      函数说明：显示ASCII(Arial类型)
      入口数据：x,y      写入的坐标
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                text[]   要显示的字符串
      返回值：  无
******************************************************************************/
void Display_TimesNewRoman_String(u16 x, u16 y, u16 zk_num, u8 text[])
{
	u8 i = 0;
	u32 FontAddr = 0; //字地址
	u32 BaseAdd = 0; //字库基地址
	u8 n, d; // 不同点阵字库的计算变量
	switch(zk_num)
	{//n:个数，d:字间距
		case 1:BaseAdd = 0x1DCDC0;n = 26;d = 8;break;	 //  6x12 ASCII(TimesNewRoman类型)
		case 2:BaseAdd = 0x1DF240;n = 34;d = 12;break;	 //  12x16 ASCII(TimesNewRoman类型)
		case 3:BaseAdd = 0x1E3E90;n = 74;d = 16;break;	 //  12x24 ASCII(TimesNewRoman类型)
		case 4:BaseAdd = 0x1ECA90;n = 130;d = 24;break;	 //  16x32 ASCII(TimesNewRoman类型)
	}
	while((text[i] > 0x00))
	{
		if((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = 	text[i] - 0x20;
			FontAddr = (u32)((FontAddr * n) + BaseAdd);
			get_n_bytes_data_from_ROM(FontAddr, FontBuf, n ); /*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Arial_TimesNewRoman(x, y, zk_num); /*显示一个ascii到LCD上 */
		}
		i++;  //下个数据
		x += d; //下一个字坐标
	}
}


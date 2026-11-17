#ifndef __LCD_H
#define __LCD_H
#include "sys.h"

////////////////////////////////////////////////////////////////////

#define LCD_W 132
#define LCD_H 132 //160

#define	 ST7735S_BLK		GPIO_Pin_2		//PA2
#define	 LCD_BLK			PAout(2)		//PA2
#define	 ST7735S_DC			GPIO_Pin_3		//PA3
#define	 LCD_DC	 			PAout(3)		//PA3
#define	 ST7735S_CS 		GPIO_Pin_4		//PA4 SPI1_NSS
#define	 LCD_CS	 			PAout(4)		//PA4
#define	 ST7735S_SCK 		GPIO_Pin_5		//PA5 SPI1_SCK
#define	 ST7735S_SDO 		GPIO_Pin_6		//PA6 SPI1_MISO
#define	 ST7735S_SDI 		GPIO_Pin_7		//PA7 SPI1_MOSI

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define RED           	 0x001F
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define BLUE           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init(void);
void LCD_Clear(void);
void LCD_SleepIn(void);
void LCD_SleepOut(void);
void Display_Asc_String(u16 x, u16 y, u16 zk_num, u8 text[]);
void Display_Asc_StringF(u16 x, u16 y, u16 zk_num, u8 text[]);
void Display_Arial_String(u16 x, u16 y, u16 zk_num, u8 text[]);
void Display_Arial_StringF(u16 x, u16 y, u16 zk_num, u8 text[]);
void Display_GB2312_String(u16 x, u16 y, u8 zk_num, u8 text[]);
void Display_TimesNewRoman_String(u16 x, u16 y, u16 zk_num, u8 text[]);




#endif

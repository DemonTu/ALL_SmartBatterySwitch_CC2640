#ifndef __OLED_H
#define __OLED_H

//SSD1306 OLED 驱动IC驱动代码
//驱动方式:8080并口/4线串口
//修改日期:2016/64/11
//版本：V1.0
//版权所有，盗版必究。
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
#define INCLUDE_OLED12832

#if defined (INCLUDE_OLED12832)

#define OLED_MODE 0
		    						
//OLED控制用函数
//void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p);	
//void IIC_WriteC(uint8_t cmd);
//void IIC_writeD(uint8_t data);

void oledShowBmp(uint8_t *bmp);

void OLED_showBatteryBmp(uint8_t line, uint8_t row, uint8_t batteryState);
void OLED_showBluetoothBmp(uint8_t line, uint8_t row);

void OLED_testShowGengeeRoll(uint8_t line);
void OLED_showPitchBmp(uint8_t line, uint8_t row);

void OLED_PitchDrawLine(uint8_t y, uint8_t x, uint8_t pointNum, uint8_t direction);

#endif
#endif


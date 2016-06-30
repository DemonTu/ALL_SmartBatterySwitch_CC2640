//#include "includes.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "sensor.h"

#include "bsp_i2c.h"
#include "oled.h"
#include "font.h"  
#include "ICall.h"
#include <ti/sysbios/knl/Semaphore.h>

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SSD1306 OLED ����IC��������
//������ʽ:IIC
////////////////////////////////////////////////////////////////////////////////// 	  

#ifdef INCLUDE_OLED12832

#define IIC_SOFTWARE

#ifdef IIC_SOFTWARE
static PIN_Handle IICGpioPin;
static PIN_State pinGpioState;
// Pins that are actively used by the application
static PIN_Config IICPinTable[] =
{
    Board_I2C0_SDA | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,     
    Board_I2C0_SCL | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
 
    PIN_TERMINATE
};

//ʹ��IIC���нӿ�ʱʹ��
#define OLED_SDA_HIGH() PIN_setOutputValue(IICGpioPin, Board_I2C0_SDA,1)
#define OLED_SDA_LOW()  PIN_setOutputValue(IICGpioPin, Board_I2C0_SDA,0)

#define OLED_SCL_HIGH() PIN_setOutputValue(IICGpioPin, Board_I2C0_SCL,1)
#define OLED_SCL_LOW()  PIN_setOutputValue(IICGpioPin, Board_I2C0_SCL,0)

#else
// Sensor selection/de-selection
#define SENSOR_SELECT1()               bspI2cSelect(BSP_I2C_INTERFACE_0,0x78) 
#define SENSOR_DESELECT1()             bspI2cDeselect()

#endif

#define OLED_CMD   0	    //д����
#define OLED_DATA  1	    //д����
#define IIC_ADD_WC 0x78		//д����ʱ�ĵ�ַ
#define IIC_ADD_WD 0x78		//д����ʱ�ĵ�ַ
#define IIC_CONC   0x00		//�����ֽ�ָ����һ���ֽ�Ϊ����
#define IIC_COND   0x45		//�����ֽ�ָ����һ���ֽ�Ϊ����

#define MAX_LINE_NAME  128
#define MAX_ROW_NAME   64

void delay_us(uint16_t time)
{	  
//	uint16_t i=0;  

//	while(time--)
//	{
//	   i=12;  //�Լ�����
//	   while(i--) ;    
//	}
}

//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
	    	    
#ifdef IIC_SOFTWARE
static void IIC_WriteChar(uint8_t byte)
{
	uint8_t i,temp;
	OLED_SCL_LOW();
	temp = byte;
	for(i=0; i<8; i++)
	{
		if((temp&0x80)>>7){
			OLED_SDA_HIGH();
		}else{
			OLED_SDA_LOW();
		}
		
		OLED_SCL_HIGH();
		//delay_us(1);
		OLED_SCL_LOW();
		//delay_us(1);
		temp = temp<<1;
	}

}

static void IIC_Wait_Ack()
{
	OLED_SCL_HIGH();
	//delay_us(1);
	OLED_SCL_LOW();
}

static void IIC_Start()
{

	OLED_SCL_HIGH();
	OLED_SDA_HIGH();
	//delay_us(1);
	OLED_SDA_LOW();
	//delay_us(1);
	OLED_SCL_LOW();
	//delay_us(1);
}

static void IIC_Stop()
{
	//delay_us(1);
	OLED_SCL_HIGH();
	
	OLED_SDA_LOW();
	//delay_us(1);

	OLED_SDA_HIGH();
	//delay_us(1);
}
#endif

// д����
static void IIC_WriteC(uint8_t cmd)
{
#ifdef IIC_SOFTWARE
	IIC_Start();						//��ʼ����
	IIC_WriteChar(IIC_ADD_WC);			//�����ַ
	IIC_Wait_Ack();						//����1306���ص�ȷ���ź�
	//delay_us(1);
	IIC_WriteChar(IIC_CONC);			//���Ϳ����ֽ�
	IIC_Wait_Ack();						//����1306���ص�ȷ���ź�
	//delay_us(1);
	IIC_WriteChar(cmd);					//���������ֽ�
	IIC_Wait_Ack();						//����1306���ص�ȷ���ź�
	//delay_us(1);
	IIC_Stop();							//��������
#else
	if (SENSOR_SELECT1())
	{
		sensorWriteReg(IIC_CONC, &cmd, 1);
		SENSOR_DESELECT1();
	}
        
#endif
}

// д����
static void IIC_WriteD(uint8_t data)
{
#ifdef IIC_SOFTWARE
	IIC_Start();							//��ʼ����
	IIC_WriteChar(IIC_ADD_WD);			//�����ַ
	IIC_Wait_Ack();							//����1306���ص�ȷ���ź�
	IIC_WriteChar(IIC_COND);			//���Ϳ����ֽ�
	IIC_Wait_Ack();							//����1306���ص�ȷ���ź�
	//delay_us(1);
	IIC_WriteChar(data);					//���������ֽ�
	IIC_Wait_Ack();							//����1306���ص�ȷ���ź�
	//delay_us(1);
	IIC_Stop();							//��������
#else
 
	if (SENSOR_SELECT1())
	{
		sensorWriteReg(IIC_COND, &data, 1);      
		SENSOR_DESELECT1();
	}
	
#endif
}

// cmd : 1 write data; 0 write command
static void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
		uint8_t t_data;

		t_data = dat;
		
		if(!cmd)
		{
			IIC_WriteC(t_data);
		}
		else
		{
			IIC_WriteD(t_data);
		}

}
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
uint8_t OLED_GRAM[MAX_LINE_NAME][MAX_ROW_NAME/8];	 
uint8_t OLED_GRAM_Temp[MAX_LINE_NAME][MAX_ROW_NAME/8];
//�����Դ浽LCD		 
#if 1
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		 
	if (memcmp(OLED_GRAM_Temp, OLED_GRAM, sizeof(OLED_GRAM))){
		memcpy(OLED_GRAM_Temp, OLED_GRAM, sizeof(OLED_GRAM));	
		//printf("OLED Updata\r\n");
	}else{
	// ����һ�����������
		return;
	}
        
	for(i=0;i<(MAX_ROW_NAME/8);i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x04,OLED_CMD);      //������ʾλ�á��е͵�ַ 2Ϊ12864 4Ϊ��������
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<MAX_LINE_NAME;n++){
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
		}
	}    
}
#else
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;		 

    // ���и��� 
    
	for(i=0;i<(MAX_ROW_NAME/8);i++)  
	{  
		if (memcmp(&OLED_GRAM_Temp[i*128], &OLED_GRAM[i*128], 128))
		{
			memcpy(&OLED_GRAM_Temp[i*128], &OLED_GRAM[i*128], 128);
			
			OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
			OLED_WR_Byte (0x04,OLED_CMD);      //������ʾλ�á��е͵�ַ 2Ϊ12864 4Ϊ��������
			OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
			for(n=0;n<MAX_LINE_NAME;n++){
				OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
			}
		}
	}    
}
#endif

	  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0; i<MAX_ROW_NAME/8; i++)
	{
		for(n=0; n<MAX_LINE_NAME; n++)
		{
			OLED_GRAM[n][i]=0X00;  
		}
	}
	OLED_Refresh_Gram();//������ʾ
}
//���� 
//x:0~127
//y:0~64/32
//t:1 ��� 0,���				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>(MAX_ROW_NAME-1))return;//������Χ��.
	pos=(MAX_ROW_NAME/8-1)-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	// ��ʾx���ַ�� 124 125 126 127 0 1 .... 123
	if (x < 4){
		x = 124+x;
	}else{
		x = x - 4;
	}
	if(t){
		OLED_GRAM[x][pos]|=temp;
	}else{
		OLED_GRAM[x][pos]&=~temp;	    
	} 
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
		{
			OLED_DrawPoint(x,y,dot);
		}
	}													    
	OLED_Refresh_Gram();//������ʾ
}
/***********************************************************************
* Funtion Name : oledShowBmp(uint8_t *bmp)
* Description  : ��ʾλͼ, ��ɨ��
* Input        : bmp λͼ����
* Output       : None
* return       : None
**************************************************************************/
void oledShowBmp(uint8_t *bmp)
{ 
	uint8_t i;
	uint8_t x,y;
	uint8_t temp = 0;
	
	for(y=0; y<MAX_ROW_NAME; y++){
		// ÿ�� 16*8=128;
		for(x=0; x<16; x++){
			temp = bmp[x+y*16];
			for(i=0; i<8; i++){
				OLED_DrawPoint(x*8+i,y,temp&0x01);
				temp>>=1;
			}
		}
	}
}

/***********************************************************************
* Funtion Name : OLED_showBatteryBmp
* Description  : ��ʾ���λͼ, ��ɨ��
* Input        : ���ͼ��״̬, ��ʾ��(4�� 0��1��2��3)����ʾ��(128/16=8�� 0,16,32...)
* Output       : None
* return       : None
**************************************************************************/
void OLED_showBatteryBmp(uint8_t line, uint8_t row, uint8_t batteryState)
{
	uint8_t i,j;
	uint8_t x;
	uint8_t y;
	uint8_t temp1 = 0;
	uint8_t temp2 = 0;

	y = line*16;
	j = 0;
	
	// ÿ�� 16*8=128;
	for(x=0; x<16; x++){
		temp1 = batteryBmp[batteryState*2+(x/8)][j];
		j++;
		temp2 = batteryBmp[batteryState*2+(x/8)][j];
		j++;
		j %= 16;
		for(i=0; i<8; i++){
			OLED_DrawPoint(row+i, y, temp1&0x01);
			OLED_DrawPoint((row+i+8), y, temp2&0x01);
			temp1>>=1;
			temp2>>=1;
		}
		y++;
	}

}

/***********************************************************************
* Funtion Name : OLED_showBluetoothBmp
* Description  : ��ʾ����λͼ, ��ɨ��
* Input        : ��ʾ��(4�� 0��1��2��3)����ʾ��(128/16=8�� 0,16,32...)
* Output       : None
* return       : None
**************************************************************************/
void OLED_showBluetoothBmp(uint8_t line, uint8_t row)
{
	uint8_t i,j;
	uint8_t x;
	uint8_t y;
	uint8_t temp1 = 0;
	uint8_t temp2 = 0;

	y = line*16;
	j = 0;
	
	// ÿ�� 16*8=128;
	for(x=0; x<16; x++){
		temp1 = batteryBmp[18+(x/8)][j];
		j++;
		temp2 = batteryBmp[18+(x/8)][j];
		j++;
		j %= 16;
		for(i=0; i<8; i++){
			OLED_DrawPoint(row+i, y, temp1&0x01);
			OLED_DrawPoint((row+i+8), y, temp2&0x01);
			temp1>>=1;
			temp2>>=1;
		}
		y++;
	}
}

/***********************************************************************
* Funtion Name : OLED_showPitchBmp
* Description  : ��ʾ����, ��ɨ��
* Input        : ��ʾ��(2�� 0��1)����ʾ��(0~20)
* Output       : None
* return       : None
**************************************************************************/
void OLED_showPitchBmp(uint8_t line, uint8_t row)
{
	uint8_t i;
	uint8_t x,y;
	uint8_t temp = 0;
	
	for(y=(line*16); y<MAX_ROW_NAME; y++){
		// ÿ�� 16*8=128;
		for(x=row; x<(16+row); x++){
			temp = pitchBmp[y-(line*16)][x];
			for(i=0; i<8; i++){
				OLED_DrawPoint(x*8+i,y,temp&0x01);
				temp>>=1;
			}
		}
	}
}

/*
*		���⺯�����򳡲���
*/
// line 0~64/32, row 0~120	ÿ�λ��˸������
void OLED_PitchDrawLine(uint8_t y, uint8_t x)
{
	uint8_t i;
	
	for(i=0; i<8; i++)
	{
		OLED_DrawPoint(x+i, y, 1);
	}
}

//=====================================================================

/***********************************************************************
* Funtion Name : OLED_showGeengeeBmp
* Description  : ��ʾ����λͼ, ��ɨ��
* Input 	   : ��ʾ��(4�� 0��1��2��3)����ʾ��(128/16=8�� 0,16,32...)
				 cnt = ��(0) ��(1) ��(2) ��(3) ��(4) ��(5) ��(6) ˾(7) ��(8) Ʒ(9)
* Output	   : None
* return	   : None
**************************************************************************/
void OLED_showGeengeeBmp(uint8_t line, uint8_t row, uint8_t cnt)
{
	uint8_t i,j;
	uint8_t x;
	uint8_t y;
	uint8_t temp1 = 0;
	uint8_t temp2 = 0;

	y = line*16;
	j = 0;
	
	// ÿ�� 16*8=128;
	for(x=0; x<16; x++){
		temp1 = GengnnBmp[cnt*2+(x/8)][j];
		j++;
		temp2 = GengnnBmp[cnt*2+(x/8)][j];
		j++;
		j %= 16;
		for(i=0; i<8; i++){
			OLED_DrawPoint(row+i, y, temp1&0x01);
			OLED_DrawPoint((row+i+8), y, temp2&0x01);
			temp1>>=1;
			temp2>>=1;
		}
		y++;
	}
}

void OLED_testShowGengeeRoll(uint8_t line)
{
	static uint8_t rollCnt = 0;
	uint8_t i;

	for (i=0; i<8; i++)
	{
		OLED_showGeengeeBmp(line, i*16, i+rollCnt);
	}
	if (++rollCnt >= 3)
	{
		rollCnt = 0;
	}
	
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ				   
    for(t=0;t<size;t++)
    {   
		if(size==12){
			temp=asc2_1206[chr][t];  //����1206����	
		}else{
			temp=asc2_1608[chr][t];		 //����1608���� 	                          
		} 
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80){
				OLED_DrawPoint(x,y,mode);
			}else{
				OLED_DrawPoint(x,y,!mode);
			}
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n����
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--){
		result*=m;	
	}    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}	   
/*************************************************************
* Funtion Name : OLEDIICGPIO_Init()
* Description  : IIC�ӿ�I/O��ʼ��
* Input        : None
* Output       : None
* Return       : None
*************************************************************/
static void OLEDIICGPIO_Init(void)
{
	OLED_GRAM_Temp[0][0] = 0xff;
#ifdef IIC_SOFTWARE
	IICGpioPin = PIN_open(&pinGpioState, IICPinTable);
#else // Ӳ��IIC
	bspI2cInit();
#endif

}

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
	OLEDIICGPIO_Init();

	delay_ms(100);
   
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(0x80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3f,OLED_CMD); //Ĭ��0X3F(1/64) 0x1F(1/32)
	
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;

	//OLED_WR_Byte(0x21,OLED_CMD); //����ҳ���ַ
	//OLED_WR_Byte(0x00,OLED_CMD); 
	
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����   0x02 Ϊ 12832�� 0x12 Ϊ 12864
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x40,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   

	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	     
	
}  







#endif


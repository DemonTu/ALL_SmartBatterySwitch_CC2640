#include "includes.h"
//#include "I2C_SoftwareDriver.h"
/*
代码如下, 其它参考资料你可能要查一下
这里也有一些内容
http://www.freedocuments.info/376948777/
笔记本电池芯片多数用TI的BQ方案 (BQxxxx)
其地址都是0x0B
然後这些电池是有个标准的, 叫做SBS v1.1 (Smart Battery System), 通信使用SMBus, 类似於I2C但有点不一样.
你可以看到代码里有可读出的参数定义
我们主要有兴趣的是
*/

// standard I2C address for Smart Battery packs
static uint8_t deviceAddress = 11; //0x0B

#define IIC_WRITE		0xfffe
#define IIC_READ		0x0001
//#define INCLUDE_IIC_DRIVER

#ifndef INCLUDE_IIC_DRIVER
/**********************************************************************
* FuntionName : SMB_Read
* Description : smbus read data with addr
* Input       : addr (register address of slaveDevice)
* Output      : None
* Return      : None
***********************************************************************/
void SMB_Read(uint8_t addr, uint8_t *dat, uint8_t len) 
{
	if (bspI2cSelect(BSP_I2C_INTERFACE_0, (deviceAddress)))
	{
		bspI2cWriteRead(&addr, 1, dat, len);
		bspI2cDeselect();
	}
}

#else

void udelay_us(unsigned char us)   //10us延时
{	
#if 1
	
#else
	uint16_t j;
	uint16_t i;

	i = us*20;
	
	while(i--)
	{
		for (j=0; j<12; j++)
		{
		}
	}
#endif	
}

static PIN_Handle IIC0GpioPin;
static PIN_State pinGpioState;

static PIN_Config IIC0PinInTable[] =
{
    Board_I2C0_SDA0 | PIN_INPUT_EN| PIN_PULLUP | PIN_DRVSTR_MAX,     
    Board_I2C0_SCL0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
 
    PIN_TERMINATE
};

static PIN_Config IIC0PinOutTable[] =
{
    Board_I2C0_SDA0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,     
    Board_I2C0_SCL0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
 
    PIN_TERMINATE
};


#define IN            1
#define OUT           0

#define  SetIicScl(x) 	PIN_setOutputValue(IIC0GpioPin, Board_I2C0_SCL0, x)
#define  SetIicSda(x) 	PIN_setOutputValue(IIC0GpioPin, Board_I2C0_SDA0, x)
#define  ReadIicSda()   ((PIN_getInputValue(Board_I2C0_SDA0))!=0)?1:0  //判断接收到的bit

#define  SoftDelay(i)  udelay_us(i)

//设置SDA IO口方向
void SetIicSdaDir(uint8_t flag)
{	
	//PIN_close(IIC0GpioPin);
	//PIN_init(IIC0PinOutTable);
	if (IN == flag)
	{
		IIC0GpioPin = PIN_open(&pinGpioState, IIC0PinInTable);
	}
	else
	{  	
		IIC0GpioPin = PIN_open(&pinGpioState, IIC0PinOutTable);	
	}
	
	SoftDelay(5);
}

/***********************************************************************

 * Name: ReadIicSda()

 * Note: Read SDA state;

 * Para: None

 * Return : SDA state

*/
void IIC_DriverInit(void)
{	
  	IIC0GpioPin = PIN_open(&pinGpioState, IIC0PinOutTable);

	SetIicSda(1);
	SetIicScl(1);				     
}

/***********************************************************************

 * Name: IIC_Start()

 * Note: Start I2C Bus, SDA change to low when SCL is hight

 * Para: None

 * Return : None

*/
static void IIC_Start(void)
{
	SetIicSda(1);
  	SetIicScl(1);
	SoftDelay(1);
	
	SetIicSda(0);
	SoftDelay(0);
	
	SetIicScl(0);

}

/***********************************************************************

 * Name: IIC_Stop()

 * Note: Stop I2C Bus, SCL change to hight when SDA is hight

 * Para: None

 * Return : None

*/
static void IIC_Stop(void)
{
	SetIicScl(1);
	
	SetIicSda(0);
	SoftDelay(1);
	
	SetIicSda(1);
}

/***********************************************************************

 * Name: IIC_SendByte(dat)

 * Note: Write 8bite data and get ack;

 * Para: dat -> the data which will be send out

 * Return : ack -> ack signal

*/
unsigned char IIC_SendByte(unsigned char dat)
{
    unsigned char i;
	unsigned char ack;

	for(i=0; i<8; i++)
	{
	    if(dat & 0x80)
		{
		    SetIicSda(1);
		}
		else
		{

		    SetIicSda(0);
		}
		SoftDelay(1);
		
		SetIicScl(1);
		SoftDelay(1);
		dat <<= 1;
		SetIicScl(0);
	}

	SetIicSdaDir(IN);
	SoftDelay(1);
	
	SetIicScl(1);
	SoftDelay(10);	// 对于smbus 时钟周期要长
	ack = ReadIicSda();
	SetIicScl(0);
	
	SetIicSdaDir(OUT);
	
	return ack;

}

/***********************************************************************

 * Name: IIC_ReadByte

 * Note: Read 8bite data and Send  ack;

 * Para: ack=0 -> Set ack, ack=1 -> Set noack

 * Return : read data

*/
unsigned char IIC_ReadByte(unsigned char ack)
{
    unsigned char i;
	unsigned char dat=0;

	SetIicSdaDir(IN);
	
	//SetIicScl(0);
	for(i=0; i<8; i++)
	{
	    dat <<= 1;
		SetIicScl(1);
		SoftDelay(1);
		if(ReadIicSda())
		{
		    dat |= 1;
		}
		SetIicScl(0);
		SoftDelay(2);
	}
	
	SetIicSdaDir(OUT);

	SetIicSda(ack);	   // ack = 0; ask, ack = 1,stop
	SoftDelay(1);
	
	SetIicScl(1);
	SoftDelay(1);
	SetIicScl(0);

	return dat;
}

/***********************************************************************

 * Name: IicWriteDevice

 * Note: Write data to device

 * Para: device -> device address, buf->subaddress + data

 * Return : how many bytes have been write

*/
unsigned char IIC_WriteDevice(unsigned char device, unsigned char *buf, unsigned char cnt)
{

    unsigned char i;

    IIC_Start();

	if(IIC_SendByte(device))
	{
	    IIC_Stop();
		return 0;
	}

	for(i=0; i<cnt; i++)
	{
	    if(IIC_SendByte(*buf++))
	    {
	        IIC_Stop();
			break;
	    }
    }

	IIC_Stop();

	return i;

}

/***********************************************************************

 * Name: IIC_ReadDevice

 * Note: Read data from device

 * Para: device->device address, subaddr->subaddress, acnt->subaddress lengh

 *       buf->read out data space, bcnt->read out data lengh

 * Return : write subaddress lengh

*/
unsigned char IIC_ReadDevice(unsigned char device, unsigned char *subaddr, unsigned char acnt, unsigned char *buf, unsigned char bcnt)
{
    unsigned char i;
	unsigned char wlen;

    IIC_Start();

	if(IIC_SendByte(device))
	{
		uartWriteDebug("ok2\r\n", 5);
	    IIC_Stop();
	    return 0;
	}

	for (i=0; i<acnt; i++)
	{
		if(IIC_SendByte(*subaddr++))
		{
			uartWriteDebug("ok1\r\n", 5);
			IIC_Stop();

			return i;
		}
	}
	wlen = i;
	
	IIC_Start();
	if (IIC_SendByte(device+0x01))
	{
		uartWriteDebug("error\r\n", 7);
	}

	for(i=0; i<bcnt-1; i++)
	{
	    *buf++ =  IIC_ReadByte(0); // read & send ACK
	}

	*buf = IIC_ReadByte(1); // read & send noack

	IIC_Stop();
	//uartWriteDebug("ok\r\n", 4);
	return wlen;

}

/**********************************************************************
* FuntionName : SMB_Read
* Description : smbus read data with command
* Input       : cmd  command
* Output      : None
* Return      : read data
***********************************************************************/
void SMB_Read(uint8_t addr, uint8_t *dat, uint8_t len)
{
	IIC_ReadDevice((deviceAddress<<1), (unsigned char *)&addr, 1, dat, len);	
}
#endif

/**********************************************************************
* FuntionName : SMB_Init
* Description : smbus init
* Input       : None
* Output      : None
* Return      : None
***********************************************************************/
void SMB_Init(void)
{
#ifdef INCLUDE_IIC_DRIVER
	IIC_DriverInit();
#else
	bspI2cInit();
#endif
}


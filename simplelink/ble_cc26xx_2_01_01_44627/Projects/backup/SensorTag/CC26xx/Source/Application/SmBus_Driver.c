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
/**********************************************************************
* FuntionName : SMB_ReadByte
* Description : smbus read one byte with command
* Input       : cmd  command
* Output      : None
* Return      : read data
***********************************************************************/
uint8_t SMB_ReadByte(uint8_t cmd)
{
	uint8_t dat;

	IIC_Start();

	if(IIC_SendByte((deviceAddress<<1) & IIC_WRITE))
	{
	   IIC_Stop();
	   return 0;
	}
	else
	{
		if(IIC_SendByte(cmd))
		{
		   IIC_Stop();
		   return 0;
		}
	}
	
	IIC_Start();
	IIC_SendByte((deviceAddress<<1) | IIC_READ);

	dat = IIC_ReadByte(1); // read & send noack
	
	return dat;
}

/**********************************************************************
* FuntionName : SMB_Read2Byte
* Description : smbus read two byte with command
* Input       : command
* Output      : None
* Return      : data
***********************************************************************/
uint16_t SMB_Read2Byte(uint8_t cmd)
{
	uint8_t dat[2];
	uint16_t temp;
#if 1
	IIC_ReadDevice((deviceAddress<<1), &cmd, 1, dat, 2);

#else
	IIC_Start();

	if(IIC_SendByte((deviceAddress<<1) & IIC_WRITE))
	{
	   IIC_Stop();
	   printf("noack\r\n");
	   return 0;
	}
	else
	{
		if(IIC_SendByte(cmd))
		{
		   printf("noack1\r\n");
		   IIC_Stop();
		   return 0;
		}
	}
	//IIC_Stop();
	
	IIC_Start();
	
	if (IIC_SendByte((deviceAddress<<1) | IIC_READ))
	{
		printf("noack2\r\n");
		IIC_Stop();
		return 0;
	}

	dat[0] = IIC_ReadByte(0); // read & send ack
	dat[1] = IIC_ReadByte(1); // read & send noack

	IIC_Stop();
#endif

	temp = (uint16_t)(dat[1]<<8)|dat[0];
	
	return temp;
	
}

/**********************************************************************
* FuntionName : SMB_ReaadString
* Description : smbus read string
* Input       : command
* Output      : string
* Return      : None
***********************************************************************/
uint8_t SMB_ReadString(uint8_t cmd, uint8_t *buf)
{
	
	unsigned char i;
	uint8_t datLen;
	
	IIC_Start();

	if(IIC_SendByte((deviceAddress<<1) & IIC_WRITE))
	{
	   IIC_Stop();
	   return 0;
	}
	else
	{
		if (IIC_SendByte(cmd))
		{
			IIC_Stop();
	   		return 0;
		}
		else
		{
			IIC_Start();
			if (IIC_SendByte((deviceAddress<<1) | IIC_READ))	// 发送读地址
			{
				IIC_Stop();
				return 0;
			}
			else
			{
				datLen = IIC_ReadByte(0); // read & send ACK
			}
		}
	}
	
	for(i=0; i<datLen-1; i++)
	{
		*buf++ =  IIC_ReadByte(0); // read & send ACK
	}

	*buf = IIC_ReadByte(1); // read & send noack

	IIC_Stop();

	return (datLen);
}

#endif

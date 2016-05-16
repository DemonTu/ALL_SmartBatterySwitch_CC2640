/*******************************************************************************
 * INCLUDES
 */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/family/arm/cc26xx/Power.h>

#include <ti/drivers/uart/UARTCC26XX.h>

#include <driverlib/prcm.h>

#include "Board.h"
#include "ICall.h"

#include "sensor.h"

#include "bsp_uart.h"
#include "oled.h"
/***************************************************************************/

/*******************************************************************************
 * LOCAL variables
 */
static UART_Handle uartHandle = NULL;
static UART_Params uartParams;

/*******************************************************************************
 * @fn          bspUartWrite
 *
 * @brief       Write to an UART device
 *
 * @param       buf - pointer to data buffer
 * @param       len - number of bytes to write
 *
 * @return      '0' if success, -1 if failed
 */
int bspUartWrite(uint8_t *buf, uint8_t len)
{
	int temp;


	temp = UART_write(uartHandle, buf, len);

	return temp;
}


/*******************************************************************************
 * @fn          bspUartRead
 *
 * @brief       Read from an UART device
 *
 * @param       buf - pointer to data buffer
 * @param       len - number of bytes to write
 *
 * @return      '0' if success, -1 if failed
 */
int bspUartRead(uint8_t *buf, uint8_t len)
{
	int temp;

	temp = UART_read(uartHandle, buf, len);

	return temp;
}


#define UART_RX_LEN   100
uint8_t rxBuf[UART_RX_LEN];

static void bspUartReadCb(UART_Handle uarthd, void *buf, size_t count)
{
	
	UART_readCancel(uarthd);
	// process data that uart receive
		
//	uartWriteDebug("tt", 2);	
//  uartWriteDebug(rxBuf, count);
	
	UART_read(uarthd, rxBuf, UART_RX_LEN);
}

static void bspUartWriteCb(UART_Handle uarthd, void *buf, size_t count)
{
	//UART_writeCancel(uarthd);
	// 每次发送完成后，重新初始化uart,使得uart接收正常
	UART_close(uarthd);
	
	bspUartInit();
}


/*******************************************************************************
 * @fn          bspUartInit
 *
 * @brief       Initialize the RTOS I2C driver (must be called only once)
 *
 * @param       none
 *
 * @return      none
 */
void bspUartInit(void)
{
	UART_init();
	
	UART_Params_init(&uartParams);
	uartParams.readDataMode  = UART_DATA_BINARY;
	uartParams.writeDataMode = UART_DATA_BINARY;
	uartParams.readMode      = UART_MODE_CALLBACK;
	uartParams.writeMode     = UART_MODE_CALLBACK;
	uartParams.baudRate      = 9600;
	uartParams.readEcho      = UART_ECHO_OFF;
	//uartParams.readReturnMode= UART_RETURN_FULL; 
	uartParams.readCallback  = bspUartReadCb;
	uartParams.writeCallback = bspUartWriteCb;
	uartHandle = UART_open(Board_UART, &uartParams);
	
 	UART_control(uartHandle, UARTCC26XX_CMD_RETURN_PARTIAL_ENABLE, NULL); // 设置 有数据就调用回调函数，而不需等数据满
 // Begin read
    bspUartRead(rxBuf, UART_RX_LEN);
 
	if (uartHandle == NULL)
	{
		Task_exit();
	}
}




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
#include "sensor.h"

#include "bsp_uart.h"
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
	return UART_write(uartHandle, buf, len);
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
	return UART_read(uartHandle, buf, len);
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
	uartParams.baudRate = 115200;
	uartHandle = UART_open(Board_UART, &uartParams);

	if (uartHandle == NULL)
	{
		Task_exit();
	}
}




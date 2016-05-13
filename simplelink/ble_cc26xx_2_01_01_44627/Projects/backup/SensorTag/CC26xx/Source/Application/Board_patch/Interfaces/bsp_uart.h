#ifndef __BSP_UART_H
	#define __BSP_UART_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stdbool.h"
#include "stdint.h"
#include <inc/hw_memmap.h>

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * FUNCTIONS
 */
void bspUartInit(void);

int bspUartRead(uint8_t *buf, uint8_t len);
int bspUartWrite(uint8_t *buf, uint8_t len);


/////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

#endif // __BSP_UART_H
	
#ifndef __UART_H__
#define __UART_H__

#include "stm32f4xx_hal.h"





int uart_send_buf(uint8_t *pbuf, uint32_t len);

int uart_rec_start(void);


#endif

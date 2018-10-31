#include "uart.h"
#include "inc/b_tp.h"
#include "stdio.h"

extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

uint8_t dma_buf[4096 + 32];

int fputc(int c, FILE *pf)
{
	uint8_t ch = c & 0xff;
	HAL_UART_Transmit(&huart1, &ch, 1, 0xfff);
	return c;
}


int uart_send_buf(uint8_t *pbuf, uint32_t len)
{
	HAL_UART_Transmit(&huart1, pbuf, len, 0xfff);
	return 0;
}


int uart_rec_start()
{
#if 1	
	HAL_UART_Receive_DMA(&huart1, dma_buf, sizeof(dma_buf));
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
#else
	HAL_UART_Receive_IT(&huart1, dma_buf, 1);
#endif
	return 0;
}


static uint32_t count;
void uart1_idle_cb()
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
	count = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
	HAL_UART_DMAStop(&huart1);
	
	b_tp_receive_data(dma_buf, 4096 + 32 - count);
	
	HAL_UART_Receive_DMA(&huart1, dma_buf, sizeof(dma_buf));
	
}






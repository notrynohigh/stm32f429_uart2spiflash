#ifndef __PLATFORM_H__
#define __PLATFORM_H__




#include "stm32f4xx_hal.h"
#include "mem_map.h"
#include "stdio.h"

#define F_CS_PORT			GPIOF
#define F_CS_PIN			GPIO_PIN_6

/**----------------------------------------------------------------------for heap_4-------*/

#define configTOTAL_HEAP_SIZE      SDRAM_HEAP_SIZE
#define portBYTE_ALIGNMENT			8

#if portBYTE_ALIGNMENT == 8
	#define portBYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

#ifndef mtCOVERAGE_TEST_MARKER
	#define mtCOVERAGE_TEST_MARKER()
#endif

#ifndef configASSERT
	#define configASSERT( x )
	#define configASSERT_DEFINED 0
#else
	#define configASSERT_DEFINED 1
#endif

#ifndef traceMALLOC
    #define traceMALLOC( pvAddress, uiSize )
#endif

#ifndef traceFREE
    #define traceFREE( pvAddress, uiSize )
#endif


void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );


/**----------------------------------------------------------------------for heap_4 end-------*/

#define DEBUG_ENABLE          1

#if DEBUG_ENABLE
#define b_log(...)   printf(__VA_ARGS__)
#else
#define b_log(...)
#endif




#endif



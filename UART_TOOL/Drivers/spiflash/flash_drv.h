#ifndef __FLASH_DRV_H__
#define __FLASH_DRV_H__


#include "platform.h"


#define FLASH_CS_SET()			HAL_GPIO_WritePin(F_CS_PORT, F_CS_PIN, GPIO_PIN_SET)
#define FLASH_CS_RESET()		HAL_GPIO_WritePin(F_CS_PORT, F_CS_PIN, GPIO_PIN_RESET)



/**
 * @defgroup FLASH_REG_MAP
 * @{
 */
#define FLASH_PAGE_SIZE 			  0X100		
#define FLASH_SECTOR_SIZE 			  0X1000	

#define W25X_WRITEENABLE		      0X06		
#define W25X_WRITEDISABLE		      0X04		
#define W25X_READSTATUSREG		      0X05		
#define W25X_WRITESTATUSREG		      0X01	
#define W25X_READDATA			      0X03		
#define W25X_FASTREADDATA		      0X0B		
#define W25X_FASTREADDUAL		      0X3B		 
#define W25X_PAGEPROGRAM		      0X02		
#define W25X_BLOCKERASE			      0XD8	
#define W25X_SECTORERASE		      0X20		
#define W25X_CHIPERASE			      0XC7		
#define W25X_POWERDOWN			      0XB9		
#define W25X_RELEASEPOWERDOWN		  0XAB		
#define W25X_DEVICEID			      0XAB		
#define W25X_MANUFACTDEVICEID		  0X90		
#define W25X_JEDECDEVICEID		      0X9F 
#define W25X_READUNIQUEID			  0X4B		

#define W25X_RESETENABLE              0X66
#define W25X_RESETCMD                 0X99
#define WIP_FLAG					  0X01		
#define WEL_FLAG					  0X02     
#define DUMMY_BYTE				      0XFF	

/*************************************************/
#define FlashMID_Winbond		      0xEF      

#define FlashMID      FlashMID_Winbond

/**
 * @}
 */


/**
 * @addtogroup FLASH_PUBLIC_INTERFACE
 * @{
 */ 

void flash_read_buf(uint8_t * , uint32_t , uint16_t );
void flash_write_buf(uint8_t * , uint32_t , uint16_t );
void flash_erase_xsector(uint32_t addr, uint32_t sector_n);

void flash_power_down(void);
void flash_wakeup(void);
void flash_erase_chip(void);

uint8_t flash_breakdown(uint8_t n);
/**
 * @}
 */




#endif




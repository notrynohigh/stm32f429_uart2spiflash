#include "flash_drv.h"

extern SPI_HandleTypeDef hspi5;


uint8_t f_spi_rw_byte(uint8_t dat)
{
	uint8_t retval;
	HAL_SPI_TransmitReceive(&hspi5, &dat, &retval, 1, 0xfff);
    return retval;
}

void f_spi_write(uint8_t *pbuf, uint32_t len)
{
	HAL_SPI_Transmit(&hspi5, pbuf, len, 0xfff);
}


void f_spi_read(uint8_t *pbuf, uint32_t len)
{
	HAL_SPI_Receive(&hspi5, pbuf, len, 0xfff);
}


/*-------------------------------------------------------------------------------*/
static void flash_write_enable()    
{
    FLASH_CS_RESET();
    f_spi_rw_byte(W25X_WRITEENABLE);
    FLASH_CS_SET();
}


static void flash_wait_write_end()    
{
    uint8_t status = 0;
    uint16_t err_count = 0;
    do 
    {
        HAL_Delay(5);
        FLASH_CS_RESET();
        f_spi_rw_byte(W25X_READSTATUSREG);        
        status = f_spi_rw_byte(DUMMY_BYTE);
        FLASH_CS_SET();
        err_count++;
    }
    while((status & WIP_FLAG) == 1 && err_count < 400);
}


static uint8_t flash_read_sr(uint8_t regno)   
{  
	uint8_t byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command = W25X_ReadStatusReg1;  
            break;
        case 2:
            command = W25X_ReadStatusReg2;   
            break;
        case 3:
            command = W25X_ReadStatusReg3;    
            break;
        default:
            command=W25X_ReadStatusReg1;    
            break;
    }    
	FLASH_CS_RESET();  
	f_spi_rw_byte(command);             
	byte=f_spi_rw_byte(0Xff);         
	FLASH_CS_SET();                          
	return byte;   
}


static void flash_erase_sector(uint32_t addr)    
{
    flash_write_enable();
    flash_wait_write_end();
    FLASH_CS_RESET();
    f_spi_rw_byte(W25X_SECTORERASE);
    if(W25QXX_TYPE == W25Q256)         
    {
        f_spi_rw_byte(((addr & 0xFF000000) >> 24));    
    }       
    f_spi_rw_byte((addr & 0xFF0000) >> 16);
    f_spi_rw_byte((addr & 0xFF00) >> 8);
    f_spi_rw_byte(addr & 0xFF);
    FLASH_CS_SET();
    flash_wait_write_end();
}
 

static void flash_write_page(uint8_t * pbuf, uint32_t addr, uint16_t len)
{
    flash_write_enable();
    FLASH_CS_RESET();
    f_spi_rw_byte(W25X_PAGEPROGRAM);
    if(W25QXX_TYPE == W25Q256)         
    {
        f_spi_rw_byte((uint8_t)((addr & 0xFF000000) >> 24));    
    }     
    f_spi_rw_byte((addr & 0xFF0000) >> 16);
    f_spi_rw_byte((addr & 0xFF00) >> 8);
    f_spi_rw_byte(addr & 0xFF);
    if (len > FLASH_PAGE_SIZE)
    {
        len = FLASH_PAGE_SIZE;
    }

		f_spi_write(pbuf, len);
		
    FLASH_CS_SET();
    flash_wait_write_end();
}

#define flash_lock()    
#define flash_unlock()  





/**
 * @defgroup FLASH_PUBLIC_INTERFACE
 * @{
 */

static uint8_t sg_flash_breakdown = 0;
#define FLASH_SELFCHECK()   do{if(sg_flash_breakdown > 0){return;}}while(0)

uint8_t flash_breakdown(uint8_t n)
{
    if(n > 1)
    {
        return sg_flash_breakdown;
    }
    sg_flash_breakdown = n;
    return sg_flash_breakdown;
}

static uint8_t sg_flash_power_count = 0;

void flash_power_down()
{
    FLASH_SELFCHECK();
    if(sg_flash_power_count > 0)
    {
        sg_flash_power_count--;
        if(sg_flash_power_count == 0)
        {
            flash_lock();
            FLASH_CS_RESET();
            f_spi_rw_byte(W25X_POWERDOWN);
            FLASH_CS_SET();
            HAL_Delay(1);
            flash_unlock();
        }
    }
}


void flash_wakeup()    
{
    FLASH_SELFCHECK();
    if(sg_flash_power_count == 0)
    {
        flash_lock();
        FLASH_CS_RESET();
        if (FlashMID == FlashMID_Winbond) 
        {
            f_spi_rw_byte(W25X_RELEASEPOWERDOWN);
        }
        FLASH_CS_SET();
        HAL_Delay(1);
        flash_unlock();
    }
    sg_flash_power_count++;
}



void flash_write_buf(uint8_t * pbuf, uint32_t addr, uint16_t len)
{
    uint16_t first_page_number = FLASH_PAGE_SIZE - (uint16_t)(addr % FLASH_PAGE_SIZE);
    uint16_t pages, i = 0;
    uint16_t last_page_number;
    FLASH_SELFCHECK();
    flash_lock();
    if(len > first_page_number)
    {
        flash_write_page(pbuf, addr, first_page_number);
        addr += first_page_number;
        pbuf += first_page_number;
        pages = (len - first_page_number) / FLASH_PAGE_SIZE;
        last_page_number = (len - first_page_number) % FLASH_PAGE_SIZE;
        for( i = 0;i < pages;i++)
        {
            flash_write_page(pbuf, addr, FLASH_PAGE_SIZE);
            addr += FLASH_PAGE_SIZE;
            pbuf += FLASH_PAGE_SIZE;
        }
        if(last_page_number != 0)
        {
            flash_write_page(pbuf, addr, last_page_number);
        }
    }
    else
    {
        flash_write_page(pbuf, addr, len);
    } 
    flash_unlock();
}


void flash_read_buf(uint8_t * pbuf, uint32_t addr, uint16_t len)    
{
    FLASH_SELFCHECK();
    FLASH_CS_RESET();
    f_spi_rw_byte(W25X_READDATA);
    if(W25QXX_TYPE == W25Q256)         
    {
        f_spi_rw_byte((uint8_t)((addr & 0xFF000000) >> 24));    
    }    
    f_spi_rw_byte((addr & 0xFF0000) >> 16);
    f_spi_rw_byte((addr & 0xFF00) >> 8);
    f_spi_rw_byte(addr & 0xFF);
    f_spi_read(pbuf, len);
    FLASH_CS_SET();
}


/**
 * @brief flash erase. the unit is the size of sector
 * @param addr start address
 * @param sector_n the number of sectors that want to erase
 */
void flash_erase_xsector(uint32_t addr, uint32_t sector_n)
{
    uint32_t i = 0;
    FLASH_SELFCHECK();
    flash_lock();
    for(i = 0;i < sector_n;i++)
    {
        flash_erase_sector(addr);
        addr += FLASH_SECTOR_SIZE;
    }
    flash_unlock();
}


void flash_erase_chip()
{
    FLASH_SELFCHECK();
    flash_lock();
    flash_write_enable();
    flash_wait_write_end();
    FLASH_CS_RESET();
    f_spi_rw_byte(W25X_CHIPERASE);
    FLASH_CS_SET();
    flash_wait_write_end(); 
    flash_unlock();    
}



uint8_t tmp_buf[FLASH_SECTOR_SIZE];	
void flash_write_buf_plus(uint8_t * pbuf, uint32_t addr, uint16_t len)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t *tmp_p;	  
   	tmp_p = tmp_buf;	 
    
 	secpos = addr / FLASH_SECTOR_SIZE;
	secoff = addr % FLASH_SECTOR_SIZE;
	secremain = FLASH_SECTOR_SIZE - secoff;

 	if(len <= secremain)
    {
        secremain = len;
    }
	while(1) 
	{	
		flash_read_buf(tmp_p, secpos * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);
		for(i = 0;i < secremain;i++)
		{
			if(tmp_p[secoff + i] != 0XFF) break;  
		}
		if(i < secremain)
		{
            flash_erase_xsector(secpos * FLASH_SECTOR_SIZE, 1);
			for(i = 0;i < secremain;i++)
			{
				tmp_p[i + secoff] = pbuf[i];	  
			}
			flash_write_buf(tmp_p, secpos * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE);

		}
        else
        {
            flash_write_buf(pbuf, addr, secremain);
        }  
        
		if(len == secremain)
            break;
		else
		{
			secpos++;
			secoff=0;

		   	pbuf += secremain;  
			addr += secremain;   
		   	len -= secremain;				
			if(len > FLASH_SECTOR_SIZE)
            {
                secremain = FLASH_SECTOR_SIZE;
            }	
			else
            {
                secremain = len;
            }        			
		}	 
	}	 
}

void flash_init()
{
    static uint8_t temp;
    if(W25QXX_TYPE==W25Q256)               
    {
        temp = flash_read_sr(3);              
        if((temp & 0X01) == 0)			      
		{
			FLASH_CS_RESET();
			f_spi_rw_byte(W25X_Enable4ByteAddr); 
			FLASH_CS_SET();
		}
    }
}














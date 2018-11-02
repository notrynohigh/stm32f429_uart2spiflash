/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "flash_drv.h"

/* Definitions of physical drive number for each drive */
#define FLS		0	

#define FLS_SECTOR_SIZE   512
#define FLS_SECTOR_COUNT  (1024 * 16 * 2)
#define FLS_BLOCK_SIZE    8

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{

	switch (pdrv) {
	case FLS :

		return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{

	switch (pdrv) {
	case FLS :

		return RES_OK;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch (pdrv) {
	case FLS :
		// translate the arguments here

		for( ; count > 0;count--)
        {
            flash_read_buf(buff, sector * FLS_SECTOR_SIZE, FLS_SECTOR_SIZE);
            sector++;
            buff += FLS_SECTOR_SIZE;
        }
        res = RES_OK;
		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch (pdrv) {
	case FLS :
		// translate the arguments here

	    for( ; count > 0;count--)
        {
            flash_write_buf_plus((uint8_t *)buff, sector * FLS_SECTOR_SIZE, FLS_SECTOR_SIZE);
            sector++;
            buff += FLS_SECTOR_SIZE;
        }
        res = RES_OK;
		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	switch (pdrv) {
	case FLS :
        switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLS_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLS_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLS_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
		return res;
    } 

	return RES_PARERR;
}


DWORD get_fattime()
{
	return 0;
}	




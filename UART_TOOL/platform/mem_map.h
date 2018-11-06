#ifndef __MEM_MAP_H__
#define __MEM_MAP_H__


#include "stdint.h"

#define SDRAM_TOTAL_SIZE       ((uint32_t)32 * 1024 *1024)
#define SPIFLASH_TOTAL_SIZE    ((uint32_t)32 * 1024 *1024)


/*
 ************************************************************************
 *                         SDRAM MAP
 ************************************************************************
*/
#define I_IMG_WIDTH     (480)
#define I_IMG_HEIGHT    (360)
#define I_IMG_SIZE      (I_IMG_WIDTH * I_IMG_HEIGHT)

#define SDRAM_BASE_ADDRESS          ((uint32_t)0xC0000000)

#define SDRAM_HEAP_ADDRESS          SDRAM_BASE_ADDRESS
#define SDRAM_HEAP_SIZE             (1024 * 256)

#define SDRAM_O_IMG_ADDRESS         (SDRAM_HEAP_ADDRESS + SDRAM_HEAP_SIZE)
#define SDRAM_O_IMG_SIZE            (I_IMG_SIZE) 

#define SDRAM_SVS_COEF_ADDRESS      (SDRAM_O_IMG_ADDRESS + SDRAM_O_IMG_SIZE)
#define SDRAM_SVS_COEF_SIZE         (0X2000)

#define SDRAM_I_IMG_ADDRESS         (SDRAM_SVS_COEF_ADDRESS + SDRAM_SVS_COEF_SIZE)
#define SDRAM_I_IMG_SIZE            (I_IMG_SIZE)


#define SDRAM_IMG_GREY_ADDRESS      (SDRAM_I_IMG_ADDRESS + SDRAM_I_IMG_SIZE)
#define SDRAM_IMG_GREY_SIZE         (I_IMG_SIZE)

#define SDRAM_IMG_CROP_ADDRESS      (SDRAM_IMG_GREY_ADDRESS + SDRAM_IMG_GREY_SIZE)
#define SDRAM_IMG_CROP_SIZE         (I_IMG_SIZE)

#define SDRAM_IMG_DS_ADDRESS        (SDRAM_IMG_CROP_ADDRESS + SDRAM_IMG_CROP_SIZE)
#define SDRAM_IMG_DS_SIZE           (I_IMG_SIZE / DSSCALE / DSSCALE)

#define SDRAM_IMG_MAG_ADDRESS       (SDRAM_IMG_DS_ADDRESS + SDRAM_IMG_DS_SIZE)
#define SDRAM_IMG_MAG_SIZE          (I_IMG_SIZE / DSSCALE / DSSCALE * 4)

#define SDRAM_IMG_THETA_ADDRESS     (SDRAM_IMG_MAG_ADDRESS + SDRAM_IMG_MAG_SIZE)
#define SDRAM_IMG_THETA_SIZE        (I_IMG_SIZE / DSSCALE / DSSCALE * 4)

#define SDRAM_IMG_HOG_ADDRESS       (SDRAM_IMG_THETA_ADDRESS + SDRAM_IMG_THETA_SIZE)
#define SDRAM_IMG_HOG_SIZE          (HOGVECL * BLOCKVECL * BINNUM * 4)

#define SDRAM_IMG_WIN_ADDRESS       (SDRAM_IMG_HOG_ADDRESS + SDRAM_IMG_HOG_SIZE)
#define SDRAM_IMG_WIN_SIZE          (HOGWIN * HOGWIN / HOGCELLSIZE / HOGCELLSIZE * BINNUM * 4)

/*
 ************************************************************************
 *                         SPIFLASH MAP
 ************************************************************************
*/
#define FLS_BASE_ADDRESS             ((uint32_t)0x0)

#define FLS_SVS_ADDRESS              (FLS_BASE_ADDRESS)
#define FLS_SVS_SIZE                 (0X700000)

#define FLS_SVS_COEF_ADDRESS         ((uint32_t)0X700000)
#define FLS_SVS_COEF_SIZE            (0X2000)

#define FLS_ALGO_SIZE                (0X800000)

#define FLS_I_IMG_ADDRESS            (FLS_BASE_ADDRESS + FLS_ALGO_SIZE)
#define FLS_I_IMG_SIZE               (0X2B000)









#endif





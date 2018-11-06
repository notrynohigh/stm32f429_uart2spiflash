#ifndef __FLASH_MANAGE_H__
#define __FLASH_MANAGE_H__


#include "flash_drv.h"
#include "mem_map.h"











void fm_load_in_img(void);
void fm_load_svs_coef(uint8_t *pbuf);
void fm_load_svs(uint8_t *pbuf, uint32_t len, uint32_t off);

#endif



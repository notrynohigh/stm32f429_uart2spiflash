#include "flash_manage.h"

uint8_t in_img_tmp[I_IMG_SIZE] __attribute__ ((at(SDRAM_I_IMG_ADDRESS))); 
 


void fm_load_in_img()
{
    uint32_t i = 0;
    for(i = 0;i < (I_IMG_SIZE / 4096);i++)
    {
        flash_read_buf(&in_img_tmp[i * 4096], FLS_I_IMG_ADDRESS + i * 4096, 4096);
    }
    if(I_IMG_SIZE % 4096)
    {
        flash_read_buf(&in_img_tmp[i * 4096], FLS_I_IMG_ADDRESS + i * 4096, I_IMG_SIZE % 4096);
    }
}


void fm_load_svs_coef(uint8_t *pbuf)
{
    uint32_t i;
    if(pbuf == NULL)
    {
        return;
    }
    for(i = 0;i < (FLS_SVS_COEF_SIZE / 4096);i++)
    {
        flash_read_buf(&pbuf[i * 4096], FLS_SVS_COEF_ADDRESS + i * 4096, 4096);
    }
    if(FLS_SVS_COEF_SIZE % 4096)
    {
        flash_read_buf(&pbuf[i * 4096], FLS_SVS_COEF_ADDRESS + i * 4096, FLS_SVS_COEF_SIZE % 4096);
    } 
}

void fm_load_svs(uint8_t *pbuf, uint32_t len, uint32_t off)
{
    flash_read_buf(pbuf, off + FLS_BASE_ADDRESS, len);
}














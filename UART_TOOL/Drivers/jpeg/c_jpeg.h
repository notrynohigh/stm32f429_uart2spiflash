#ifndef __C_JPEG_H__
#define __C_JPEG_H__

#include "stdint.h"
#include "libjpeg.h"




#define I_IMG_QUALITY   (40)




unsigned long c_jpeg_grayscale(uint8_t* p_in_img, 
                               uint8_t *p_out_img, unsigned long out_buf_size, 
                               uint16_t width, uint16_t height);


#endif



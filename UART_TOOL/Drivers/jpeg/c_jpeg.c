#include "c_jpeg.h"




unsigned long c_jpeg_grayscale(uint8_t* p_in_img, 
                               uint8_t *p_out_img, unsigned long out_buf_size, 
                               uint16_t width, uint16_t height)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  uint8_t *  outfile = p_out_img;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);
  jpeg_mem_dest (&cinfo, &outfile, &out_buf_size);

  cinfo.image_width = width; 	            /* image width and height, in pixels */
  cinfo.image_height = height;
  cinfo.input_components = 1;		        /* # of color components per pixel */
  cinfo.in_color_space = JCS_GRAYSCALE; 	/* colorspace of input image */

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, I_IMG_QUALITY, TRUE /* limit to baseline-JPEG values */);
  jpeg_start_compress(&cinfo, TRUE);
  row_stride = width * 1;	             /* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = &p_in_img[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }
  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  return out_buf_size;
}










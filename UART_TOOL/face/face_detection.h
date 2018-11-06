/*--------------------------------------------------
Face Detection
BY: SIJIA WANG
DATE:OCT-2018
DESIGNED FOR A 36X36 WINDOW
----------------------------------------------------*/
#ifndef __FACE_DETECTION_H__
#define __FACE_DETECTION_H__

#include "SVM.h"
#include <math.h>
#include <stdio.h>     // for debug
#include "arm_math.h"

#define SVMTHRES 	-0.05     	//SVM classifcation threshold
#define DSSCALE 	6 		//downsample scale
#define SSTEP 		3		//sliding window step
#define IMGWIDTH 	480
#define IMGHEIGHT 	360
#define IMGCWIDTH   240		//image crop width must be even
#define IMGCHEIGHT 	320		//image crop height must be even

typedef struct{
	
	image_matrix * ds_buf;        //downsample image buffer
	gradient_matrix * grad_buf;  //gradient matrix buffer
	HOG_vec * hog_vec;        //HOG vector buffer
	wind_cell * win_cell;     //window cell buffer
	
} calc_buffer;

typedef struct{
	uint32_t idx_min;
	uint32_t idx_max;
	uint8_t  scale;
} face_location;         //face location in original img

void downsample(image_matrix * Input,image_matrix * Output, uint8_t Scale);
int face_detection(image_matrix * img_In, svm_model * svm_mdl, uint8_t Scale, uint8_t step, calc_buffer cbuff, face_location * output);
void cbuf_init(calc_buffer * cbuf, image_matrix * img_buf, gradient_matrix * img_grad, float32_t * hog_buf, float32_t * win_buf);
void face_crop(image_matrix * img_in, face_location floc, image_matrix * img_crop);

#endif


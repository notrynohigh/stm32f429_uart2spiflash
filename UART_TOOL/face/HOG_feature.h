/*--------------------------------------------------
HOG FACE DETECTOR
BY: SIJIA WANG
DATE:OCT-2018
DESIGNED FOR A 36X36 WINDOW
----------------------------------------------------*/
#ifndef __HOG_FEATURE_H__
#define __HOG_FEATURE_H__


#include <stdio.h>     // for debug
#include "arm_math.h"

#define HOGCELLSIZE 6   //define the HOG cell pixel size
#define HOGBLOCKSIZE 12  //define the HOG block pixel size
#define BINNUM 9        //define the number of orientional bins
#define HOGWIN 36       //define the HOG-window pixel size
#define FTRL 3          //define the filter length
#define BLOCKVECL (HOGBLOCKSIZE / HOGCELLSIZE * HOGBLOCKSIZE / HOGCELLSIZE) //define block vector size
#define HOGVECL (HOGWIN / HOGCELLSIZE - HOGBLOCKSIZE / HOGCELLSIZE + 1) * (HOGWIN / HOGCELLSIZE - HOGBLOCKSIZE / HOGCELLSIZE + 1) //define HOG vector length


static int16_t FILTER[3] = {-1,0,1};


/*---------------------------
Define a image matrix structure for 8bit grayscale image
----------------------------*/
typedef struct
{
	uint16_t w;
	uint16_t h;
	uint8_t *pData; 
} image_matrix;
/*-----------------------------
Define a gradient matrix structure
------------------------------*/
typedef struct
{
	uint16_t w;
	uint16_t h;
	float32_t *pMag;
	float32_t *pTheta;
} gradient_matrix;

/*--------------------------------
Define a cell histogram vector
---------------------------------*/
typedef struct
{
	float32_t hist[BINNUM];
} cell_hist;

/*--------------------------------
Define cells in window
--------------------------------*/
typedef struct
{
	cell_hist cell[HOGWIN * HOGWIN / HOGCELLSIZE / HOGCELLSIZE];
}wind_cell;


/*--------------------------------
Define a block vector
--------------------------------*/
typedef struct
{
	cell_hist cell[BLOCKVECL]; 
} block_vector;
/*--------------------------------
Define a HOG vector
--------------------------------*/
typedef struct
{
	block_vector block[HOGVECL];
} HOG_vec;
/*---------------------------
functions
----------------------------*/

void calc_px(image_matrix * Input, uint32_t offset, float32_t * mag, float32_t * theta);
void calc_imggrad(image_matrix * Input, gradient_matrix * output);
void calc_cell(gradient_matrix * Input,uint32_t offset, float32_t * res);
int calc_wincell(gradient_matrix * Input, uint32_t offset, cell_hist * res);
int block_norm(cell_hist * Input, uint32_t offset, block_vector * Output);
int calc_HOGvec(cell_hist * Input, HOG_vec * Output);
int HOG_feature(uint32_t offset, gradient_matrix * img_grad, wind_cell * img_cell, HOG_vec * img_hog);

#endif



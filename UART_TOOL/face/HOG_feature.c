/*--------------------------------------------------
HOG FACE DETECTOR
BY: SIJIA WANG
DATE:OCT-2018
DESIGNED FOR A 36X36 WINDOW

----------------------------------------------------*/
#include "HOG_feature.h"

/*-------------------------------------------------
Calculate Pixel Gradient
*Input: Input image
offset: Offset of the pixel
*res[2]: [0] Mag [1] Theta without sign
-------------------------------------------------*/
void calc_px(image_matrix * Input, uint32_t offset, float32_t * mag, float32_t * theta){
	int16_t px_vecx[FTRL] = {0}, px_vecy[FTRL] = {0};
	uint8_t * pIn = (*Input).pData;
	uint16_t wIn, hIn, x, y;
	int64_t Gx, Gy;
	//float32_t angle;
	
	wIn = (*Input).w;
	hIn = (*Input).h;
	x = offset % wIn;     // x-axis index
	y = offset / wIn;     // y-axis index
	
	for(int i = 0; i < FTRL; i++){
		if( (x + i - 1) < 0 || (x + i) > wIn ){
			break;
		}else{
			px_vecx[i] = *(pIn + offset + i - 1);
		};
	}
	for(int i = 0; i < FTRL; i++){
		if( (y + i - 1) < 0 || (y + i) > hIn ){
			break;
		}else{
			px_vecy[i] = *(pIn + offset + (i - 1)*wIn);
		};
//		printf("%d %d \n",px_vecx[i],px_vecy[i]);
	};				
	arm_dot_prod_q15(&FILTER[0],&px_vecx[0],FTRL,&Gx);
	arm_dot_prod_q15(&FILTER[0],&px_vecy[0],FTRL,&Gy);		
//	printf("%lld %lld\n", Gx, Gy);
	arm_sqrt_f32((float32_t)Gx * (float32_t)Gx + (float32_t)Gy * (float32_t)Gy,mag);
//	printf("%lld %lld %f\n", Gx, Gy, sqrt((float32_t)Gx * (float32_t)Gx + (float32_t)Gy * (float32_t)Gy));
	if(Gy == 0){
		*theta = 0.0;
	}else if(Gx == 0){
		*theta = 90.0;
	}else if(Gy > 0){
		*theta = atan2((float32_t)Gy , (float32_t)Gx) / PI * 180;
	}else{
		*theta = 180 + atan2((float32_t)Gy , (float32_t)Gx) / PI * 180;
	};	
	
};	

	
/*--------------------------------------------
Calculate image gradient
---------------------------------------------*/
void calc_imggrad(image_matrix * Input, gradient_matrix * output){
	(*output).h = (*Input).h;
	(*output).w = (*Input).w;
	for( int i = 0; i < (*Input).w * (*Input).h; i++){
		calc_px(Input, i, (*output).pMag + i, (*output).pTheta + i);
	};	
};
/*--------------------------------------------
Calculate cell histograms
*Input: image gradient input
offset: cell start position
*res[9]: orientation histogram. Evenly distributed from 0 to 160, bin size is 20
--------------------------------------------*/
void calc_cell_hist(float32_t mag, float32_t theta, float32_t * res){
	uint32_t factor = theta / 20;
	if( factor > 7 ){
		*(res + 8) += mag / HOGCELLSIZE / HOGCELLSIZE ;
//		printf("%d   %f    %f\n", factor, 180 - theta, theta - 160);
	}else{
		*(res + factor) += mag / HOGCELLSIZE / HOGCELLSIZE;
//		printf("%d   %f    %f\n", factor,(factor + 1) * 20 - theta, theta - factor * 20);
	};
	
};


void calc_cell(gradient_matrix * Input,uint32_t offset, float32_t * res){
	uint16_t wIn, x, y;
	uint32_t idx;
//	float32_t px_grad[2];

	wIn = (*Input).w;
	x = offset % wIn;     // x-axis index
	y = offset / wIn;     // y-axis index
	
	for( int i = 0; i < HOGCELLSIZE * HOGCELLSIZE; i++){
		idx = (y + i / HOGCELLSIZE)*wIn + x + i % HOGCELLSIZE;
		calc_cell_hist(*((*Input).pMag + idx),*((*Input).pTheta + idx),res);		
//		printf("%d   %f   %f \n",idx, *((*Input).pMag + idx),*((*Input).pTheta + idx));
	};
	
};
/*-----------------------------------------------------
Calculate all cells in the window
return 0 if the window is out of the image
*Input: image input
offset: first pixel offset
*res: cell histogram results
------------------------------------------------------*/
int calc_wincell(gradient_matrix * Input, uint32_t offset, cell_hist * res){
	uint16_t wIn, hIn, x, y;
	uint32_t idx;
	
	wIn = (*Input).w;
	hIn = (*Input).h;
	x = offset % wIn;     // x-axis index
	y = offset / wIn;     // y-axis index
	
	if( (x + HOGWIN + 1) > wIn || (y + HOGWIN + 1 ) > hIn ){
		return 0;
	}else{
		for( int i = 0; i < HOGWIN * HOGWIN / HOGCELLSIZE / HOGCELLSIZE; i++){
			idx = (y + i / (HOGWIN / HOGCELLSIZE) * HOGCELLSIZE)*wIn + x + i % (HOGWIN / HOGCELLSIZE) * HOGCELLSIZE;
//			if( i == 0){
//				printf("%d\n", idx);
//			};
			calc_cell(Input, idx, (float32_t*)(res + i));
		};	
//// debug code		
//		for( int i = 0; i < HOGWIN * HOGWIN / HOGCELLSIZE / HOGCELLSIZE; i++){
//			if(offset == 10){
//				printf("%f\n", *((float32_t*)(res + i)));
//			};
//		};			
////debug code
		
		return 1;
	};
		
};
/*-------------------------------------------------------
Normalize by block
return 0 if the block is out of the window
*Input: The address of 1st cell_hist in the window
--------------------------------------------------------*/
int block_norm(cell_hist * Input, uint32_t offset, block_vector * Output){
	float32_t denom = 0;
	uint16_t x, y, idx;
	x = offset % (HOGWIN / HOGCELLSIZE);     // x-axis index
	y = offset / (HOGWIN / HOGCELLSIZE);     // y-axis index
	
	if( (x + HOGBLOCKSIZE / HOGCELLSIZE ) > (HOGWIN / HOGCELLSIZE) || (y + HOGBLOCKSIZE / HOGCELLSIZE ) > (HOGWIN / HOGCELLSIZE)){
//		printf("Block error\n");
		return 0;
	}else{	
		for( int i = 0; i < BLOCKVECL; i++){
			idx = (y + i / (HOGBLOCKSIZE / HOGCELLSIZE)) * (HOGWIN / HOGCELLSIZE) + x + i % (HOGBLOCKSIZE / HOGCELLSIZE); 
//			printf("offset idx %d\n", idx);
			for(int j = 0; j < BINNUM; j++){
				denom += (*(Input + idx)).hist[j] * (*(Input + idx)).hist[j];
//				printf("%f\n",(*(Input + idx)).hist[j]);
			};
		};

		denom = sqrt(denom + 1e-5 * 1e-5);

//		printf("============\n");
//		printf("%d   %f\n",offset, denom);
//		printf("============\n");
		for( int i = 0; i < BLOCKVECL; i++){
			idx = ( y + i / (HOGBLOCKSIZE / HOGCELLSIZE)) * (HOGWIN / HOGCELLSIZE) + x + i % (HOGBLOCKSIZE / HOGCELLSIZE); 
			
			for(int j = 0; j < BINNUM; j++){
				(*Output).cell[i].hist[j] = (*(Input + idx)).hist[j] / denom;
				//printf("%f \n", (*(Input + idx)).hist[j] / denom);
			};
//		printf("block finish\n");
		};
		return 1;
	};	
};
/*--------------------------------------------------------
Calculate HOG vector from window
---------------------------------------------------------*/
int calc_HOGvec(cell_hist * Input, HOG_vec * Output){
	uint16_t idx,res;	
	for( int i = 0; i < HOGVECL; i++){
		idx = i / (HOGWIN / HOGCELLSIZE - HOGBLOCKSIZE / HOGCELLSIZE + 1) * (HOGWIN / HOGCELLSIZE) + i % (HOGWIN / HOGCELLSIZE - HOGBLOCKSIZE / HOGCELLSIZE + 1);
//		printf("%d\n", idx);
		res = block_norm(Input, idx, &(*Output).block[i]);
		if(res == 0){
//			printf("calc_HOGvec error\n");
			return 0;
		};
	};
//	printf("calc_HOGvec finish\n");
	return 1;
};

/*---------------------------------------------------------
Image HOG feature calculation
---------------------------------------------------------*/
int HOG_feature(uint32_t offset, gradient_matrix * img_grad, wind_cell * img_cell, HOG_vec * img_hog){
	uint8_t res,res1;
	
	res = calc_wincell(img_grad, offset, &(*img_cell).cell[0]);
	if(res == 1){
		res1 = calc_HOGvec(&(*img_cell).cell[0],img_hog);
		if(res1 == 0){
			return 0;
		}
	}else{
		return 0;
	};
	return 1;
};

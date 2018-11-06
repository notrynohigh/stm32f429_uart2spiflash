/*--------------------------------------------------
HOG FACE DETECTOR
BY: SIJIA WANG
DATE:OCT-2018
DESIGNED FOR A 36X36 WINDOW
INPUT 
----------------------------------------------------*/

#include "face_detection.h"


/*------------------------------------
Downsample the uint8 image in half of the row/col numbers
*Input: input image pointer
*Output: output image pointer
-------------------------------------*/
void downsample(image_matrix * Input,image_matrix * Output, uint8_t Scale){
	uint8_t * pIn, * pOut;
	uint16_t wIn, hIn, wOut, hOut;
	
	wIn = (*Input).w;
	hIn = (*Input).h;
	wOut = wIn / Scale;
	hOut = hIn / Scale;
	pIn = (*Input).pData;
	pOut = (*Output).pData;
	
	for(uint16_t i = 0; i < hOut; i++){
		for(uint16_t j = 0; j < wOut; j++){
			uint16_t temp = 0;
			for (uint16_t k = 0; k < Scale * Scale; k++){
				temp += (uint16_t)(*(pIn + (i * Scale + k / Scale) * wIn + (j * Scale + k % Scale)));
			};
//			temp = (uint16_t)(*(pIn + 2 * i * wIn + 2 * j)) +
//				   (uint16_t)(*(pIn + 2 * i * wIn + 2 * j + 1)) +
//			       (uint16_t)(*(pIn + (2 * i + 1) * wIn + 2 * j)) +
//			       (uint16_t)(*(pIn + (2 * i + 1) * wIn + 2 * j + 1));			
			temp = temp / (Scale * Scale);	
			//printf("%d\n",temp);
			*pOut = (uint8_t)temp;
//			printf("%d\n",*pOut);
			pOut++;
		};
	};
	(*Output).w = wOut;
	(*Output).h = hOut;
};

/*----------------------------------------
Initialize Buffer
----------------------------------------*/
void cbuf_init(calc_buffer * cbuf, image_matrix * img_buf, gradient_matrix * img_grad, float32_t * hog_buf, float32_t * win_buf){
	(*cbuf).ds_buf = img_buf;
	(*cbuf).grad_buf  = img_grad;
	(*cbuf).hog_vec = (HOG_vec*)hog_buf;
	(*cbuf).win_cell = (wind_cell*)win_buf;
};

/*---------------------------------
Face detection
-----------------------------------*/


int face_detection(image_matrix * img_In, svm_model * svm_mdl, uint8_t Scale, uint8_t step, calc_buffer cbuff, face_location * output){
	uint16_t res = 0, slide_n, data_class;
	uint32_t idx = 0, idx_min, idx_max;
	image_matrix * img_mat = cbuff.ds_buf;
	
	
	downsample(img_In, img_mat, Scale);
	calc_imggrad(img_mat, cbuff.grad_buf);
	idx_max = 0;
	idx_min = (*img_mat).h * (*img_mat).w - 1;
	slide_n = ((*img_mat).w - HOGWIN) / step + 1;
	for( int i = 0; i < slide_n * slide_n ; i++){
		idx  = i / slide_n * step * (*img_mat).w + i % slide_n * step;
		res = HOG_feature(idx,cbuff.grad_buf,cbuff.win_cell, cbuff.hog_vec);
		
//		for(int mm = 0; mm < 900;mm++){
//			printf("%f\n",*((float32_t*)cbuff.hog_vec + mm));
//		};
//		printf("============================\n");
		
		if(res == 1){
			data_class = SVM_detect(svm_mdl, (float32_t *)(cbuff.hog_vec), SVMTHRES);
			if(data_class == 1){
				if(idx < idx_min){
					idx_min = idx;
				};
				if(idx > idx_max){
					idx_max = idx;
				};
//				printf("%d	%d	%d\n", idx, idx_min, idx_max);
			};
		};
		//clear buffer win_cell
		for( int j = 0; j < HOGWIN * HOGWIN / HOGCELLSIZE / HOGCELLSIZE * BINNUM; j++){
			*((float32_t*)cbuff.win_cell + j) = 0;
		};
		//clear buffer hog_vec
		for( int j = 0; j < HOGVECL * BLOCKVECL * BINNUM; j++){
			*((float32_t*)cbuff.hog_vec + j) = 0;
		};
	};
	(*output).idx_min = idx_min;
	(*output).idx_max = (idx_max / (*img_mat).w + HOGWIN) * (*img_mat).w + idx_max % (*img_mat).w + HOGWIN;
	(*output).scale = Scale;
	if (idx_min == ((*img_mat).h * (*img_mat).w - 1) && idx_max == 0){
//		printf("no face detected");
		return 0;
	}else{
		return 1;
	};

};

/*---------------------------------
Face crop
---------------------------------*/
void face_crop(image_matrix * img_in, face_location floc, image_matrix * img_crop){
	uint32_t idx;
	int32_t Cx, Cy, ix, iy;
	Cx = (floc.idx_min / (IMGWIDTH / floc.scale) + floc.idx_max / (IMGWIDTH / floc.scale)) / 2 * floc.scale;
	Cy = (floc.idx_min % (IMGWIDTH / floc.scale) + floc.idx_max % (IMGWIDTH / floc.scale)) / 2 * floc.scale;	
	for(int i = 0; i < IMGCHEIGHT * IMGCWIDTH; i++){
		ix = (Cx - IMGCHEIGHT / 2) + i / IMGCWIDTH;
		iy = (Cy - IMGCWIDTH / 2) + i % IMGCWIDTH;
		if( ix < 0 || iy < 0 || ix > (*img_in).h || iy > (*img_in).w){
			*((*img_crop).pData + i) = 0;
		}else{
			idx = ix * IMGWIDTH + iy;   
			*((*img_crop).pData + i) = *((*img_in).pData + idx);	
		};
	};
};



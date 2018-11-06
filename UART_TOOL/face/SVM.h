/*--------------------------------------------------
SUPPORT VECTOR MACHINE
BY: SIJIA WANG
DATE:OCT-2018
----------------------------------------------------*/

#ifndef __SVM_H__
#define __SVM_H__

#include <stdio.h>
#include <math.h>
#include "arm_math.h"
#include "HOG_feature.h"

#define SVSN 1774
#define SVSL 900
#define GAMMA 0.008958
#define BIAS -3.645816

typedef struct
{
	uint16_t n_sVs;  // number of support vectors
	uint16_t l_sVs;  // length of support vectors
	float32_t gamma ; //gamma value
	float32_t bias; // bias
	float32_t * dual_coef; //support vector coefs
	float32_t * sVs; // support vectors
} svm_model;


float32_t kernel_rbf(float32_t x, float32_t y);
int SVM_detect(svm_model * svm_mdl, float32_t * hog_vec, float32_t thres);

#endif


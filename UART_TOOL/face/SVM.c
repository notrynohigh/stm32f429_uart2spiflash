/*--------------------------------------------------
SUPPORT VECTOR MACHINE
BY: SIJIA WANG
DATE:OCT-2018
----------------------------------------------------*/
#include "SVM.h"

extern void fm_load_svs(uint8_t *, uint32_t , uint32_t );

float32_t kernel_rbf(float32_t x, float32_t y){
	float64_t temp;
	temp = x * x + y * y - 2 * x * y;
	return (float32_t)temp;
};


uint8_t svs_tmp_buf[SVSL * sizeof(float)];

int SVM_detect(svm_model * svm_mdl, float32_t * hog_vec, float32_t thres){
	float32_t res = 0;
	float32_t tmp = 0;
	for( int i = 0; i < SVSN; i++){
		tmp = 0;
        fm_load_svs(svs_tmp_buf, (SVSL * sizeof(float)), i * (SVSL * sizeof(float)));
		for( int j = 0; j < SVSL; j++){
			//tmp += kernel_rbf(*((*svm_mdl).sVs + i * SVSL + j),*(hog_vec + j));
            tmp += kernel_rbf(((float *)svs_tmp_buf)[j],*(hog_vec + j));
		};	
		res += (*svm_mdl).dual_coef[i] * exp( -(*svm_mdl).gamma * tmp);
	}
	res = res + (*svm_mdl).bias;
//	printf("%f\n", res);
	if ( res > thres ){
		return 1;
	}else{
		return 0;
	};
};

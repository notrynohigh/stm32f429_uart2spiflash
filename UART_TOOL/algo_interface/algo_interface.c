#include "algo_interface.h"
#include "flash_manage.h"

uint8_t img_grey_buf[SDRAM_IMG_GREY_SIZE] __attribute__ ((at(SDRAM_IMG_GREY_ADDRESS))); 
uint8_t img_crop_buf[SDRAM_IMG_CROP_SIZE] __attribute__ ((at(SDRAM_IMG_CROP_ADDRESS))); 
uint8_t img_ds_buf[SDRAM_IMG_DS_SIZE] __attribute__ ((at(SDRAM_IMG_DS_ADDRESS))); 
uint8_t img_mag_buf[SDRAM_IMG_MAG_SIZE] __attribute__ ((at(SDRAM_IMG_MAG_ADDRESS))); 
uint8_t img_Hog_buf[SDRAM_IMG_HOG_SIZE] __attribute__ ((at(SDRAM_IMG_HOG_ADDRESS))); 
uint8_t img_win_buf[SDRAM_IMG_WIN_SIZE] __attribute__ ((at(SDRAM_IMG_WIN_ADDRESS))); 
uint8_t img_theta_buf[SDRAM_IMG_THETA_SIZE] __attribute__ ((at(SDRAM_IMG_THETA_ADDRESS))); 
uint8_t algo_svs_coef[SDRAM_SVS_COEF_SIZE] __attribute__ ((at(SDRAM_SVS_COEF_ADDRESS)));

static face_location floc = {I_IMG_SIZE / DSSCALE / DSSCALE - 1, 0, DSSCALE};  


static image_matrix data_in = {I_IMG_WIDTH, I_IMG_HEIGHT, 0};
static image_matrix data_ds = {0};
static image_matrix data_crop = {I_IMG_WIDTH, I_IMG_HEIGHT, 0};
static gradient_matrix data_grad ={0};
static svm_model svm_mdl ={SVSN, SVSL, GAMMA, BIAS, 0, 0};
static calc_buffer cbuf = {0};


void algo_interface_init()
{
    memset(img_Hog_buf, 0, SDRAM_IMG_HOG_ADDRESS);
    memset(img_win_buf, 0, SDRAM_IMG_WIN_ADDRESS);
    
    fm_load_svs_coef(algo_svs_coef);
 
	data_ds.pData = (uint8_t *)img_ds_buf;
	data_crop.pData = (uint8_t *)img_crop_buf;
	data_grad.pMag = (float32_t *)img_mag_buf;
	data_grad.pTheta = (float32_t *)img_theta_buf;
	svm_mdl.dual_coef = (float32_t *)algo_svs_coef;

}

uint8_t find_face(uint8_t *pgrey)
{
    uint8_t res;
    data_in.pData = pgrey;
	res = face_detection(&data_in, &svm_mdl, DSSCALE, SSTEP, cbuf, &floc);
    if(res)
    {
        face_crop(&data_in, floc, &data_crop);
    }
	return res;
}








#ifndef __ALGO_INTERFACE_H__
#define __ALGO_INTERFACE_H__

#include "SVM.h"
#include "face_detection.h"
#include "mem_map.h"



void algo_interface_init(void);
uint8_t find_face(uint8_t *pgrey);


#endif




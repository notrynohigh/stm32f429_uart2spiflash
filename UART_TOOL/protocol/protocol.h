#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "stdint.h"


#define CMD_SEND_FILE       0X1
#define CMD_SEND_ALGO_P     0X2
#define CMD_CHECK_FILE      0X51


#pragma pack(1)

typedef struct
{
	uint8_t cmd;
	uint8_t status;
	uint8_t buf[1];
}proto_struct_t;


typedef struct
{
	uint8_t cmd;
	uint8_t status;
	uint8_t *pbuf;
}proto_struct2_t;



typedef struct
{
	uint32_t address;
	uint32_t len;
	uint8_t  buf[1];
}proto_file_info_t;


#pragma pack()





void protocol_init(void);

void pro_file_ack(uint8_t ack);
void pro_algo_p_ack(uint8_t ack);

#endif







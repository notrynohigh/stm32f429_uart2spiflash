#include "protocol.h"
#include "inc/b_tp.h"
#include "uart.h"
#include "string.h"
#include "flash_drv.h"
#include "mem_map.h"

static void proto_get_file(proto_struct2_t proto_struct2)
{
	proto_file_info_t *pfile_info = (proto_file_info_t *)proto_struct2.pbuf;
	if((pfile_info->address & 0xfff) != 0)
    {
        pro_file_ack(2);
        return;
    }
    pfile_info->address += FLS_I_IMG_ADDRESS;
	flash_erase_xsector(pfile_info->address, 1);
    flash_write_buf(pfile_info->buf, pfile_info->address, pfile_info->len);
	pro_file_ack(0);
}

static void proto_get_algo_param(proto_struct2_t proto_struct2)
{
	proto_file_info_t *pfile_info = (proto_file_info_t *)proto_struct2.pbuf;
	if((pfile_info->address & 0xfff) != 0)
    {
        pro_algo_p_ack(2);
        return;
    }
    pfile_info->address += FLS_SVS_ADDRESS;
	flash_erase_xsector(pfile_info->address, 1);
    flash_write_buf(pfile_info->buf, pfile_info->address, pfile_info->len);
	pro_algo_p_ack(0);
}


static void proto_check_file()
{
    ;
}

static void dispatch_cmd(proto_struct2_t proto_struct2)
{
	switch(proto_struct2.cmd)
	{
		case CMD_SEND_FILE:
			proto_get_file(proto_struct2);
			break;
        case CMD_SEND_ALGO_P:
            proto_get_algo_param(proto_struct2);
            break;
        case CMD_CHECK_FILE:
            proto_check_file();
            break;
		default:
			break;
	}
}


void protocol_cb(uint8_t *pbuf, uint32_t len)
{
	proto_struct2_t protocol_struct2;
	if(len < 2)
	{
		return;
	}
	protocol_struct2.cmd = pbuf[0];
	protocol_struct2.status = pbuf[1];
	if(len == 2)
	{
		protocol_struct2.pbuf = NULL;
	}
	else
	{
		protocol_struct2.pbuf = pbuf + 2;
	}
	dispatch_cmd(protocol_struct2);
}


void protocol_init()
{
	b_tp_reg_callback(protocol_cb);
}

void protocol_send(uint8_t cmd, uint8_t status, uint8_t *pbuf, uint8_t buf_len)
{
	uint8_t buf[250];
	proto_struct_t *proto_struct = (proto_struct_t *)buf;
	
	proto_struct->cmd = cmd;
	proto_struct->status = status;
	memcpy(proto_struct->buf, pbuf, buf_len);
	
	b_tp_send_data(buf, buf_len + 2);
}




void pro_file_ack(uint8_t ack)
{
	protocol_send(CMD_SEND_FILE, ack, NULL, 0);
}

void pro_algo_p_ack(uint8_t ack)
{
	protocol_send(CMD_SEND_ALGO_P, ack, NULL, 0);
}




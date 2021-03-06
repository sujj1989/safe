/**************************
* Created by:     XMAN
* Created date:   2016-06-06
* Version:        V1.0
* Descriptions:   
**************************/
#ifndef  _AD9528_H
#define  _AD9528_H

#include "helios.h"

#define WIDTH_32BIT     4
#define WIDTH_24BIT        3
#define WIDTH_16BIT        2
#define WIDTH_8BIT        1

#define AD9528_RST_REG     0x81

typedef struct {
    u16 reg;     // �Ĵ�����ַ
    u8 val;      // д��Ĵ�����ֵ
    u8 width;   // �Ĵ���ֵ�ֽڸ���
}AD9528_REG_STRUC;


extern u8 ad9528_config (void);
#endif


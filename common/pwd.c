//#include "c.h"
#include <string.h>
#include "pwd.h"

const u8 key[56] = {0xa4, 0x15, 0x20, 0x7f, 0x5a, 0xee, 0x12, 0x36, 0x17, 0xA0, 0x05, 0x6E, 0xED, 0xD6, 0x35, 0x0B,
                    0xcc, 0xcc, 0x3c, 0xc3, 0xbb, 0xb8, 0x11, 0x93, 0x8e, 0xf6, 0x6a, 0xbd, 0x60, 0x97, 0x8e, 0xea,
                    0x3b, 0xc6, 0x74, 0xac, 0xc3, 0xf9, 0xfc, 0xec, 0x29, 0x48, 0x81, 0x1f, 0xf6, 0x0d, 0x22, 0x95,
                    0xb0, 0x66, 0xc6, 0x41, 0xfd, 0x1a, 0x92, 0xe5
                   };

u8 Gene[56];

void SetGene(s8 *buf)
{
	memcpy(Gene, buf, 56);
}


/*
���ܣ�����
*/
void EncryptCrack(s8 *buf)
{
	u8 i;


	for (i = 0; i < 56; i++)
	{
		buf[i] ^= Gene[i];
		buf[i] ^= key[i];
	}

}
void EncryptCrackExt(s8 *buf, u32 buf_len)
{
	u8 i;
	u32 temp_cnt = 0;

	while (temp_cnt < buf_len) {
		for (i = 0; i < 56; i++) {
			//buf[temp_cnt]^=Gene[i];
			buf[temp_cnt] ^= key[i];
			temp_cnt++;

			if (temp_cnt >= buf_len)
				break;
		}
	}
}

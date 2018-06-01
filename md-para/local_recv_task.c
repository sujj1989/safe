/*
name:local_recv_task.c
author:salan
date:20170322
ver:1.0
description:
	monitor task
*/

/*****************************include files***********************/
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <string.h>
#include "helios.h"
#include "protocol_def.h"
#include "para_table.h"
#include "porting.h"
#include "protocol_sem.h"
#include "protocol_task.h"
#include "local_recv_task.h"
#include "log.h"
/*************************data struct define***********************/
pthread_t local_recv_ts_id;
pthread_attr_t local_recv_ts_attr;
/****************************funs*******************************/
void *local_recv_task(void* arg)
{
	s32 err;
	s32 cnt;

	//RLDEBUG("local_recv_task task start!\r\n");

	arg = arg;
	//init local tty
	err = frame_tty_open();
	if (err < 0) {
		RLDEBUG("local_recv_task: tty open false!\r\n");
	}

	while (1) {
		/*判断上一包数据是否已经处理？*/
		cnt = 0;
		if (sem_getvalue(&(protocol_sem_t.protocol_recv_sem), &cnt)) {
			RLDEBUG("local_recv_task:get sem value err\r\n");
		}
		if (cnt > 0) {
			timedelay(0, 0, 100, 0);
			continue;
		}

		/*recv local data*/
		memset(&frame_local_recv, 0, sizeof(frame));
		cnt = frame_tty_recv((s8*)&frame_local_recv, sizeof(frame));
#ifdef PRO_DEBUG_ENABLE
		if (cnt < 0) {
			RLDEBUG("protocol task tty read false!\r\n");
		}
		///*
		else {
			//RLDEBUG("protocol task read sus! frame len:%d\r\n", cnt);
			/*RLDEBUG("frame data:");
			for (err = 0; err < cnt; err++) {
				RLDEBUG("%x,", frame_local_recv.arr[err]);
			}
			RLDEBUG("\r\n");*/
		}//*/
#endif
		/*post recv data*/
		if (cnt > 0)
		{
			frame_info_t.local_recv_len = cnt;
			sem_post(&(protocol_sem_t.protocol_recv_sem));
		} else if (!cnt) {
			timedelay(0, 0, 500, 0);
			frame_tty_close();
			timedelay(0, 0, 300, 0);
			err = frame_tty_open();
			if (err < 0) {
				RLDEBUG("local_recv_task:frame_tty_open() false\r\n");
			}
		}
	}
}
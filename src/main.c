/*
 * File      : main.c
 *
 *
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-30      sujj      first version
*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "log.h"
#include "digmod_task.h"
#include "monitor_task.h"
#include "pamod_task.h"
#include "para_table.h"
#include "porting.h"
#include "protocol_sem.h"
#include "protocol_task.h"
#include "dig_band0.h"
#include "data_task_def.h"
#include "local_recv_task.h"
#include "data_task.h"
#include "usr_def.h"
#include "alarm_def.h"
#include "protocol_trans.h"

/* #define FILE_SAFE_EEXIST "/tmp/safe_eexist_abc"
void delete_file(void)
{
	remove(FILE_SAFE_EEXIST);
} */
u8 reset_agc_flag = 0;

int main(int argc, char const *argv[])
{
    s32 err;
	u8 run_led=0;
	u16 Search_Sys_Time=0;
/*     // 程序执行之初，先去判断文件是否存在
    int fd = -1;
    fd = open(FILE_SAFE_EEXIST, O_RDWR | O_TRUNC | O_CREAT | O_EXCL, 0664);
    if (fd < 0)
    {
        if (errno == EEXIST)
        {
            printf("进程已经存在，并不要重复执行\n");
            return -1;
        }
    }

	atexit(delete_file);	// 注册进程清理函数 */
	
    RLDEBUG("start system...\r\n");
	RLDEBUG("software version: 0.0.5\r\n");

	//init data
	err = data_init();
	if (err < 0) {
		RLDEBUG("main:data_init err\r\n");
		band_dynamic_para_a[0].alarm.ap_init = 1;
	}
	
	//protocol sem init
	err = sem_init(&(protocol_sem_t.protocol_send_sem), 0, 0);
	if (err) {
		RLDEBUG("main:creat protocol_send_sem sem err\r\n");
		RLDEBUG("main:errno is:%s \r\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	err = sem_init(&(protocol_sem_t.protocol_recv_sem), 0, 0);
	if (err) {
		RLDEBUG("main:creat protocol_recv_sem sem err\r\n");
		exit(EXIT_FAILURE);
	}

	// pthread_mutex_init(&exmod_para_mutex, NULL);


    pthread_attr_init(&digmod_attr);
    pthread_attr_setdetachstate(&digmod_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&digmod_attr, STACKSIZE);
    err = pthread_create(&digmod_tid, &digmod_attr, digmod_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat digmod thread false!\r\n");
    }

    pthread_attr_init(&monitor_attr);
    pthread_attr_setdetachstate(&monitor_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&monitor_attr, STACKSIZE);
    err = pthread_create(&monitor_tid, &monitor_attr, monitor_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat monitor thread false!\r\n");
    }

    pthread_attr_init(&pamod_attr);
    pthread_attr_setdetachstate(&pamod_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&pamod_attr, STACKSIZE);
    err = pthread_create(&pamod_tid, &pamod_attr, pamod_thread, NULL);
    if (err < 0) {
        RLDEBUG("creat pamod thread false!\r\n");
    }

	//creat local recv task
	pthread_attr_init(&local_recv_ts_attr);
	pthread_attr_setdetachstate(&local_recv_ts_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&local_recv_ts_attr, STACKSIZE);
	err = pthread_create(&local_recv_ts_id, &local_recv_ts_attr, local_recv_task, &local_recv_ts_attr);
	if (err < 0) {
		RLDEBUG("creat local recv task false!\r\n");
	}

	#if 1
	//creat protocol send task
	pthread_attr_init(&frame_send_ts_attr);
	pthread_attr_setdetachstate(&frame_send_ts_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&frame_send_ts_attr, STACKSIZE);
	err = pthread_create(&frame_send_ts_id, &frame_send_ts_attr, frame_send_task, &frame_send_ts_attr);
	if (err < 0) {
		RLDEBUG("creat frame send task false!\r\n");
	}
	#endif

	//creat protocol processing task
	pthread_attr_init(&protocol_ts_attr);
	pthread_attr_setdetachstate(&protocol_ts_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&protocol_ts_attr, STACKSIZE);
	err = pthread_create(&protocol_ts_id, &protocol_ts_attr, protocol_task, &protocol_ts_attr);
	if (err < 0) {
		RLDEBUG("creat protocol task false!\r\n");
	}

	/********creat data update task*************/
	pthread_attr_init(&data_ts_attr);
	pthread_attr_setdetachstate(&data_ts_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&data_ts_attr, STACKSIZE);
	err = pthread_create(&data_ts_id, &data_ts_attr, data_task, &data_ts_attr);
	if (err < 0) {
		RLDEBUG("creat data task false!\r\n");
	}

	//alarm task
	pthread_attr_init(&alarm_ts_attr);
	pthread_attr_setdetachstate(&alarm_ts_attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setstacksize(&alarm_ts_attr, STACKSIZE);
	err = pthread_create(&alarm_ts_id, &alarm_ts_attr, alarm_task, &alarm_ts_attr);
	if (err < 0) {
		RLDEBUG("creat data task false!\r\n");
	}

	err = led_open();
	if (err < 0) {
		RLDEBUG("led dev open err!\r\n");
	}

    while(1){
        sleep(1);
		usr_auth_lapse();//授权计时
		led_run_control(run_led++%2);

/* 		if(Search_Sys_Time++ >= 60)
		{
			Search_Sys_Time = 0;
			Search_Sys();
		} */

		if(reset_agc_flag){
			reset_agc_flag = 0;
			reset_ch_agc_val();
		}
    }


    return 0;

}


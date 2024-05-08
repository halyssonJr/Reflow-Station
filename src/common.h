#ifndef __COMMON_H_
#define __COMMON_H_

#include <nuttx/config.h>
#include <sys/boardctl.h>
#include <sys/param.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <debug.h>
#include <mqueue.h>
#include <syslog.h>

#define MQ_MAX_MSG 4
#define MQ_MSG_SIZE 256
#define MQ_NAME "tempeture_mq"

/**
 * 
*/
#define MAX6675_SENSOR_PATH "/dev/temp0" 
#define MAX6675_TASK_NAME  "max6675"
#define MAX6675_TASK_STACK_SIZE 2048
#define MAX6675_TASK_PRIORITY 100

/**
 * 
*/
#define DISPLAY_TASK_NAME  "lcd"
#define DISPLAY_TASK_STACK_SIZE 4096
#define DISPLAY_TASK_PRIORITY 120
#define LCDDEV_PATH "/dev/lcd0"

#define REFLOW_DEBUG

#ifdef REFLOW_DEBUG
    #define REFLOW_INFO_LOG(format, ...) syslog((LOG_INFO), format, ##__VA_ARGS__)
    #define REFLOW_ERR_LOG(format, ...) syslog((LOG_ERR), format, ##__VA_ARGS__)
    #define REFLOW_WARNING_LOG(format, ...) syslog((LOG_WARNING), format, ##__VA_ARGS__)
#else
    #define REFLOW_INFO_LOG(format, ...)
    #define REFLOW_ERR_LOG(format, ...)
    #define REFLOW_WARNING_LOG(format, ...)
#endif

#endif
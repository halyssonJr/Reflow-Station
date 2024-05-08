/****************************************************************************
 * apps/examples/lvgl_encoder/display.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <lvgl/lvgl.h>
#include <port/lv_port.h>
#include <lvgl/demos/lv_demos.h>

#include "display.h"
#include "common.h"

#define DISPLAY_RES_H 320
#define DISPLAY_RES_V 240

static pid_t display_pid;
static mqd_t g_display_mq;


void main_screen_cb(lv_event_t *event)
{
    
}

void create_main_screen(lv_obj_t *parent)
{
 
}

static int display_task(int argc, char *argv[])
{
    int ret;

    while (1)
    {
        ret = mq_receive(g_display_mq, (char *)&pos, sizeof(pos), 0);
        if(ret > 0)
        {

        }

        usleep(1000);
    }

    return EXIT_SUCCESS;
}

/**
 * 
 * @brief :
 * @name : create_display_mq
 * @param : void
*/
int create_display_mq(void)
{
    g_display_mq = mq_open(MQ_NAME, O_RDONLY, 0666, NULL);
    if (g_display_mq < 0)
    {
        return ERROR;
    }
    return OK;
}

/**
 * 
 * @brief :
 * @name : close_display_mq
 * @param : void
*/
static int close_display_mq(void)
{
  int ret;

  ret = mq_close(g_display_mq);
  if (ret < 0)
  {
    return ret;
  }
  
  ret = mq_unlink(MQ_NAME);
  if (ret < 0)
  {
    return ret;
  }

  return 0;
}


int init_display(void)
{

    #ifdef NEED_BOARDINIT
    /* Perform board-specific driver initialization */

    boardctl(BOARDIOC_INIT, 0);

    #ifdef CONFIG_BOARDCTL_FINALINIT
    /* Perform architecture-specific final-initialization (if configured) */

    boardctl(BOARDIOC_FINALINIT, 0);
    #endif
    #endif

    /* LVGL initialization */
    lv_init();

    /* LVGL port initialization */
    lv_port_init();

    /* Display driver initialization */
    create_main_screen(lv_scr_act());

    int ret = create_display_mq();
    if (ret < 0)
    {
        REFLOW_ERR_LOG("Failed to open message queue\n");
        return ret;
    }

    display_pid = task_create(DISPLAY_TASK_NAME,
                              DISPLAY_TASK_PRIORITY,
                              DISPLAY_TASK_STACK_SIZE,
                              display_task,
                              NULL);
    if (display_pid < 0)
    {
        close_display_mq();
        REFLOW_ERR_LOG("Failed to create the display task\n");
        return ERROR;
    }

    while (1)
    {
        lv_timer_handler();
        usleep(10000);
    }
    return OK;
}

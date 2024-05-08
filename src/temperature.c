
/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "temperature.h"
#include "common.h"

mqd_t g_temperature_mq;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/**
 * 
 * @brief :
 * @name :
 * @param :
*/
static int max6675_task(int argc, char *argv[])
{
  int16_t temp;
  int fd;
  int ret;

  fd = open(MAX6675_SENSOR_PATH, O_RDONLY);
  if (fd < 0)
  {
    REFLOW_ERR_LOG("Failed to open %s", MAX6675_SENSOR_PATH);
    return EXIT_FAILURE;
  }

  while (1)
  {
    ret = read(fd, &temp, 2);
    if (ret > 0)
    {
      REFLOW_INFO_LOG("Temperature = %d!\n", temp/4);
      // ret = mq_send(g_temperature_mq, (char *)&temp, sizeof(temp), 0);
    }
    else 
    {
      REFLOW_WARNING_LOG("Falied to read MAX6675");
    }
    sleep(1);
  }

  return OK;
}

/**
 * 
 * @brief :
 * @name :
 * @param :
*/
static int create_max6675_mq(void)
{

  g_temperature_mq = mq_open(MQ_NAME, O_WRONLY | O_CREAT, 0666, NULL);
  if (g_temperature_mq < 0)
  {
    return ERROR;
  }

  return OK;
}

/**
 * 
 * @brief :
 * @name :
 * @param :
*/
static int close_max6675_mq(void)
{
  int ret;

  ret = mq_close(g_temperature_mq);
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

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int init_max6675(void)
{
  int ret;

  ret = create_max6675_mq();
  if (ret < 0)
  {
    REFLOW_ERR_LOG("Failed to open message queue \n");
    return ret;
  }
  
  ret = task_create(MAX6675_TASK_NAME,
                    MAX6675_TASK_PRIORITY,
                    MAX6675_TASK_STACK_SIZE,
                    max6675_task,
                    NULL);
  if (ret < 0)
  {
    printf("Failed to start MAX6675 task: %d\n", ret);
    return ret;
  }

  return OK;
}
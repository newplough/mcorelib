#include "board_log.h"
#include "usart.h"
#include "cmsis_os.h"
#include <string.h>

extern osMutexId_t logMutexHandle;


void BoardLog_Write(const char *data, int size)
{
    if (size > 0)
    {
        //if (osOK == osMutexAcquire(logMutexHandle, 1000))
        {
            HAL_UART_Transmit(&huart1, data, size, 1000);
            //osMutexRelease(logMutexHandle);
        }
    }
}

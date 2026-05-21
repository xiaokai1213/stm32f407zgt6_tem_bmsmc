#include "task_io_read.h"
#include "io_in.h"
#include "task.h"

/*******************************************************************************************************/
/* 全局变量定义 */

volatile IO_State_t g_io_state = {0}; /* 全局IO输入状态变量，volatile防止编译器优化 */

/**
 * @brief       IO输入读取任务（50ms周期任务）
 *   @note      使用vTaskDelayUntil实现精确的50ms固定周期
 *              通过BSP层函数读取IO信号，更新全局volatile变量g_io_state
 *              外部任务通过g_io_state直接读取最新IO状态
 * @param       argument: 任务参数（未使用）
 * @retval      无
 */
void Task_IO_Read(void *argument) {
    TickType_t xLastWakeTime;

    /* 初始读取一次IO状态 */
    g_io_state.io_wake_up = io_read_wake_up();
    g_io_state.io_fault00 = io_read_fault00();

    /* 获取当前系统滴答作为周期起始点 */
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        /* 等待下一个50ms周期 - 使用绝对延迟保证固定周期 */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));

        /* 通过BSP函数读取各IO输入信号，更新全局状态变量 */
        g_io_state.io_wake_up = io_read_wake_up();
        g_io_state.io_fault00 = io_read_fault00();

        /* 后续可在此扩展读取其他IO输入信号 */
        /* g_io_state.xxx = io_read_xxx(); */
    }
}

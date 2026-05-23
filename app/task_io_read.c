#include "task_io_read.h"
#include "bms_state.h"
#include "io_in.h"
#include "task.h"

/*******************************************************************************************************/

/**
 * @brief       IO输入读取任务（50ms周期任务）
 *   @note      使用vTaskDelayUntil实现精确的50ms固定周期
 *              读取IO信号并发送对应事件到BMS状态机：
 *              - wake_up 有效 → 发送放电请求事件 (BMS_SetDischargeReq)
 *              - fault00 有效 → 发送故障事件 (BMS_SetFault)
 * @param       argument: 任务参数（未使用）
 * @retval      无
 */
void Task_IO_Read(void *argument) {
    TickType_t xLastWakeTime;

    /* 获取当前系统滴答作为周期起始点 */
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        /* 等待下一个50ms周期 - 使用绝对延迟保证固定周期 */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));

        /* 读取IO输入信号，根据状态发送对应事件到BMS状态机 */
        if (io_read_wake_up()) {
            BMS_SetDischargeReq();  /* 唤醒信号有效 → 发送放电请求事件 */
        } else {
            BMS_ClearDischargeReq(); /* 唤醒信号无效 → 清除放电请求 */
        }

        if (io_read_fault00()) { BMS_SetFault(); /* 故障信号有效 → 发送故障事件 */ }

        /* 后续可在此扩展读取其他IO输入信号 */
    }
}

#include "scheduler.h"

sTask SCH_tasks_G[SCH_MAX_TASKS];
uint32_t current_tasks = 0;
static uint32_t next_task_id = 1;

void SCH_Init(void) {
    current_tasks = 0;
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
    }
}

uint32_t system_tick_count = 0;

void SCH_Update(void) {
    system_tick_count++;

    if (SCH_tasks_G[0].pTask != 0) {
        if (SCH_tasks_G[0].Delay > 0) {
            SCH_tasks_G[0].Delay--;
        }
        if (SCH_tasks_G[0].Delay == 0) {
            SCH_tasks_G[0].RunMe = 1;
        }
    }
}

uint32_t get_time(void) {
    return system_tick_count * 10;
}

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    if (current_tasks >= SCH_MAX_TASKS) return 0;

    uint32_t newDelay = DELAY;
    int insertIndex = 0;

    while (insertIndex < current_tasks && newDelay >= SCH_tasks_G[insertIndex].Delay) {
        newDelay -= SCH_tasks_G[insertIndex].Delay;
        insertIndex++;
    }

    for (int i = current_tasks; i > insertIndex; i--) {
        SCH_tasks_G[i] = SCH_tasks_G[i - 1];
    }

    SCH_tasks_G[insertIndex].pTask = pFunction;
    SCH_tasks_G[insertIndex].Delay = newDelay;
    SCH_tasks_G[insertIndex].Period = PERIOD;
    SCH_tasks_G[insertIndex].RunMe = 0;
    SCH_tasks_G[insertIndex].TaskID = next_task_id++;

    if (DELAY == 0) {
        SCH_tasks_G[insertIndex].RunMe = 1;
    } else {
        SCH_tasks_G[insertIndex].RunMe = 0;
    }

    if (insertIndex + 1 <= current_tasks) {
        SCH_tasks_G[insertIndex + 1].Delay -= newDelay;
    }

    current_tasks++;
    return SCH_tasks_G[insertIndex].TaskID;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    int deleteIndex = -1;
    for (int i = 0; i < current_tasks; i++) {
        if (SCH_tasks_G[i].TaskID == taskID) {
            deleteIndex = i;
            break;
        }
    }
    if (deleteIndex == -1) return 0;

    if (deleteIndex + 1 < current_tasks) {
        SCH_tasks_G[deleteIndex + 1].Delay += SCH_tasks_G[deleteIndex].Delay;
    }

    for (int i = deleteIndex; i < current_tasks - 1; i++) {
        SCH_tasks_G[i] = SCH_tasks_G[i + 1];
    }

    SCH_tasks_G[current_tasks - 1].pTask = 0;
    current_tasks--;
    return 1;
}

void SCH_Dispatch_Tasks(void) {
    if (current_tasks > 0 && SCH_tasks_G[0].RunMe > 0) {

        (*SCH_tasks_G[0].pTask)();
        SCH_tasks_G[0].RunMe = 0;

        void (*pTask)() = SCH_tasks_G[0].pTask;
        uint32_t period = SCH_tasks_G[0].Period;
        uint32_t taskID = SCH_tasks_G[0].TaskID;

        SCH_Delete_Task(taskID);

        if (period > 0) {
            SCH_Add_Task(pTask, period, period);
        }

        if (current_tasks > 0 && SCH_tasks_G[0].Delay == 0) {
            SCH_tasks_G[0].RunMe = 1;
        }
    }
}

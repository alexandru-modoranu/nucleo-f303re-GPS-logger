#include "Os.h"

static thread_t *OsCfg_TaskPool[OS_THREAD_NUMBER];

void Os_Init(void)
{
	uint32_t id = 0u;

	chSysInit();

	for (id = 0u; id < OS_THREAD_NUMBER; id ++)
	{
		thread_descriptor_t tdp = {NULL,
								   OsCfg_Config[id].pvTaskStackStart,
								   OsCfg_Config[id].pvTaskStackEnd,
								   OsCfg_Config[id].ulPriority,
								   OsCfg_Config[id].pfTaskFunction,
								   OsCfg_Config[id].pvArguments};
		OsCfg_TaskPool[id] = chThdCreateSuspended(&tdp);
		OsCfg_TaskPool[id]->startoffset = OsCfg_Config[id].ulOffset;
		OsCfg_TaskPool[id]->recurrence = OsCfg_Config[id].ulRecurrence;
	}
}

void Os_StartTasks(void)
{
	const systime_t startTime = chVTGetSystemTime();
	uint32_t noOfStartedTasks = 0u;
	while (noOfStartedTasks <= OS_THREAD_NUMBER)
	{
		uint32_t id = 0u;
		for (id = 0u; id < OS_THREAD_NUMBER; id++)
		{
			if (!chVTIsSystemTimeWithin(startTime, (startTime + OsCfg_TaskPool[id]->startoffset))
			&& (OsCfg_TaskPool[id]->state == CH_STATE_WTSTART))
			{
				chThdStart(OsCfg_TaskPool[id]);
				noOfStartedTasks++;
				chSysLock();
				chSchRescheduleS();
				chSysUnlock();
			}
		}
	}
}


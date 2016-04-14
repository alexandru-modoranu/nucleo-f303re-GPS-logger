/*
 * os_cfg.c
 *
 *  Created on: Apr 12, 2016
 *      Author: modoranu_a1
 */

#include "Os_Cfg.h"

#define THREAD_STACK(thread_name)	(thread_name##_Stack)

#define TASK(tname)					static THD_FUNCTION(tname, arg) { \
										(void)arg; \
										while(TRUE)

#define TerminateTask()				chThdSleepMilliseconds(currp->recurrence); }

static THD_FUNCTION(Task_2ms, arg);
static THD_FUNCTION(Task_5ms, arg);
static THD_FUNCTION(Task_10ms, arg);
static THD_FUNCTION(Task_20ms, arg);
static THD_FUNCTION(Task_40ms, arg);
static THD_FUNCTION(Task_80ms, arg);
static THD_FUNCTION(Task_100ms, arg);

static THD_WORKING_AREA(THREAD_STACK(Task_2ms), OS_THREAD_STACK_SIZE);
static THD_WORKING_AREA(THREAD_STACK(Task_5ms), OS_THREAD_STACK_SIZE);
static THD_WORKING_AREA(THREAD_STACK(Task_10ms), OS_THREAD_STACK_SIZE);
static THD_WORKING_AREA(THREAD_STACK(Task_20ms), OS_THREAD_STACK_SIZE);
static THD_WORKING_AREA(THREAD_STACK(Task_40ms), OS_THREAD_STACK_SIZE);
static THD_WORKING_AREA(THREAD_STACK(Task_80ms), OS_THREAD_STACK_SIZE);
static THD_WORKING_AREA(THREAD_STACK(Task_100ms), OS_THREAD_STACK_SIZE);

const OsCfg_ConfigType OsCfg_Config[OS_THREAD_NUMBER] =
{
	{	Task_2ms,	NULL,	NORMALPRIO + 70u,	0u,	2u, 	THD_WORKING_AREA_BASE(THREAD_STACK(Task_2ms)),		THD_WORKING_AREA_END(THREAD_STACK(Task_2ms))  },
	{	Task_5ms,	NULL,	NORMALPRIO + 60u,	0u,	5u, 	THD_WORKING_AREA_BASE(THREAD_STACK(Task_5ms)),		THD_WORKING_AREA_END(THREAD_STACK(Task_5ms))  },
	{	Task_10ms,	NULL,	NORMALPRIO + 50u,	0u,	10u,	THD_WORKING_AREA_BASE(THREAD_STACK(Task_10ms)),		THD_WORKING_AREA_END(THREAD_STACK(Task_10ms)) },
	{	Task_20ms,	NULL,	NORMALPRIO + 40u,	0u,	20u,	THD_WORKING_AREA_BASE(THREAD_STACK(Task_20ms)),		THD_WORKING_AREA_END(THREAD_STACK(Task_20ms)) },
	{	Task_40ms,	NULL,	NORMALPRIO + 30u, 	0u,	40u,	THD_WORKING_AREA_BASE(THREAD_STACK(Task_40ms)),		THD_WORKING_AREA_END(THREAD_STACK(Task_40ms)) },
	{	Task_80ms,	NULL,	NORMALPRIO + 20u,	0u,	80u,	THD_WORKING_AREA_BASE(THREAD_STACK(Task_80ms)),		THD_WORKING_AREA_END(THREAD_STACK(Task_80ms)) },
	{	Task_100ms,	NULL,	NORMALPRIO + 10u,	0u,	100u,	THD_WORKING_AREA_BASE(THREAD_STACK(Task_100ms)),	THD_WORKING_AREA_END(THREAD_STACK(Task_100ms))}
};

TASK(Task_2ms)
{

	TerminateTask();
}

TASK(Task_5ms)
{
	TerminateTask();
}

TASK(Task_10ms)
{
	TerminateTask();
}

TASK(Task_20ms)
{
	TerminateTask();
}
TASK(Task_40ms)
{
	TerminateTask();
}

TASK(Task_80ms)
{
	TerminateTask();
}

TASK(Task_100ms)
{
	TerminateTask();
}

/*
 * os_cfg.h
 *
 *  Created on: Apr 12, 2016
 *      Author: modoranu_a1
 */

#if !defined(OS_CFG_H)
#define OS_CFG_H

#include "ch.h"

#define OS_THREAD_NUMBER				(7u)
#define OS_THREAD_STACK_SIZE			(512u)

typedef struct OsCfg_ConfigTypeTag
{
	tfunc_t pfTaskFunction;
	void *pvArguments;
	tprio_t ulPriority;
	systime_t ulOffset;
	systime_t ulRecurrence;
	stkalign_t *pvTaskStackStart;
	stkalign_t *pvTaskStackEnd;
} OsCfg_ConfigType;

extern const OsCfg_ConfigType OsCfg_Config[OS_THREAD_NUMBER];

#endif /* OS_CFG_H */

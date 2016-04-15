#if !defined(LED_H)
#define LED_H

#include "halconf.h"
#include "chtypes.h"
#include "cmparams.h"

typedef enum Led_StateTypeTag
{
	LED_STATE_OFF = 0u,
	LED_STATE_ON,
	LED_STATE_UNKNOWN
} Led_StateType;

typedef enum Led_IdTypeTag
{
	LED_ID_USER0 = 0u,
	LED_ID_UNKNOWN
} Led_IdType;

extern void Led_Init(const uint32_t rec);
extern void Led_MainFunction(void);
extern void Led_Deinit(void);
extern void Led_SetOn(const Led_IdType id);
extern void Led_SetOff(const Led_IdType id);
extern void Led_Toggle(const Led_IdType id, const uint32_t activeTime, const uint32_t idleTime);
extern void Led_HeartBeat(const Led_IdType id, const uint32_t activeTime, const uint8_t activeCycles, const uint32_t idleTime);
extern Led_StateType Led_GetState(const Led_IdType id);

#endif /* LED_H */

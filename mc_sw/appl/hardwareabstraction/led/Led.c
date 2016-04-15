#include <string.h>

#include "Led.h"
#include "hal_pal.h"
#include "SoftwareTimer.h"

typedef enum Led_ExecStateTypeTag
{
	LED_EXEC_STATE_INACTIVE = 0u,
	LED_EXEC_STATE_ACTIVE,
	LED_EXEC_STATE_UNKNOWN
} Led_ExecStateType;

typedef struct Led_ConfigTypeTag
{
	void (*exec)(const Led_IdType id);
	stm32_gpio_t *portNumber;
	uint32_t pinNumber;
	uint8_t isInverted;
} Led_ConfigType;

typedef struct Led_TimerTypeTag
{
	uint32_t activeElapsed;
	uint32_t activeReload;
	uint8_t activeCyclesElapsed;
	uint8_t activeCyclesReload;
	uint32_t inactiveElapsed;
	uint32_t inactiveReload;
} Led_TimerType;

typedef struct Led_ContainerTypeTag
{
	Led_ConfigType config;
	Led_TimerType timer;
	Led_StateType state;
	Led_ExecStateType execState;
} Led_ContainerType;

typedef struct Led_DataTypeTag
{
	uint32_t mainFunctionRec;
	Led_ContainerType container[LED_ID_UNKNOWN];
} Led_DataType;

static void setLedActive(const Led_IdType id);
static void setLedInactive(const Led_IdType id);
static Led_StateType getLedState(const Led_IdType id);
static void toggleLed(const Led_IdType id);

static void toggleExec(const Led_IdType id);
static void heartBeatExec(const Led_IdType id);

static Led_DataType Led_Data;
static const Led_ConfigType Led_Config[LED_ID_UNKNOWN] =
{
	{heartBeatExec, GPIOA, GPIOA_PIN5, FALSE}
};

void Led_Init(uint32_t rec)
{
	uint32_t id = 0u;

	memset(&Led_Data, 0u, sizeof(Led_Data));
	Led_Data.mainFunctionRec = rec;

	for (id = 0u; id < (uint32_t)LED_ID_UNKNOWN; id++)
	{
		memcpy(&Led_Data.container[id].config, &Led_Config[id], sizeof(Led_Data.container[id].config));
		setLedInactive(id);
		Led_Data.container[id].state = getLedState(id);
		Led_Data.container[id].execState = LED_EXEC_STATE_UNKNOWN;

		if (Led_Data.container[id].config.exec != NULL)
		{
			switch (Led_Data.container[id].state)
			{
			case LED_STATE_OFF:
				Led_Data.container[id].execState = LED_EXEC_STATE_INACTIVE;
				break;
			case LED_STATE_ON:
				Led_Data.container[id].execState = LED_EXEC_STATE_ACTIVE;
				break;
			default:
				Led_Data.container[id].execState = LED_EXEC_STATE_UNKNOWN;
				break;
			}
		}
	}
}

void Led_MainFunction(void)
{
	uint32_t id = 0u;

	for (id = 0u; id < LED_ID_UNKNOWN; id++)
	{
		if (Led_Data.container[id].config.exec != NULL)
		{
			Led_Data.container[id].config.exec(id);
		}
	}
}

void Led_Deinit(void)
{
	uint32_t id = 0u;

	for (id = 0u; id < (uint32_t)LED_ID_UNKNOWN; id++)
	{
		setLedInactive(id);
		Led_Data.container[id].state = LED_STATE_OFF;
		memset(&Led_Data.container[id].timer, 0u, sizeof(Led_Data.container[id].timer));
	}
}

void Led_SetOn(const Led_IdType id)
{
	setLedActive(id);
}

void Led_SetOff(const Led_IdType id)
{
	setLedInactive(id);
}

void Led_Toggle(const Led_IdType id, const uint32_t activeTime, const uint32_t idleTime)
{
	if ((id < LED_ID_UNKNOWN) && (Led_Data.mainFunctionRec != 0u))
	{
		Led_Data.container[id].timer.activeReload = activeTime / Led_Data.mainFunctionRec;
		Led_Data.container[id].timer.activeCyclesReload = 1u;
		Led_Data.container[id].timer.inactiveReload = idleTime / Led_Data.mainFunctionRec;
	}
}

void Led_HeartBeat(const Led_IdType id, const uint32_t activeTime, const uint8_t activeCycles, const uint32_t idleTime)
{
	if ((id < LED_ID_UNKNOWN) && (Led_Data.mainFunctionRec != 0u))
	{
		Led_Data.container[id].timer.activeReload = activeTime / Led_Data.mainFunctionRec;
		Led_Data.container[id].timer.activeCyclesReload = activeCycles;
		Led_Data.container[id].timer.inactiveReload = idleTime / Led_Data.mainFunctionRec;
	}
}

Led_StateType Led_GetState(const Led_IdType id)
{
	return Led_Data.container[id].state;
}


static void setLedActive(const Led_IdType id)
{
	if (id < LED_ID_UNKNOWN)
	{
		if (Led_Data.container[id].config.isInverted)
		{
			palClearPad(Led_Data.container[id].config.portNumber, Led_Data.container[id].config.pinNumber);
		}
		else
		{
			palSetPad(Led_Data.container[id].config.portNumber, Led_Data.container[id].config.pinNumber);
		}
		Led_Data.container[id].state = getLedState(id);
	}
}

static void setLedInactive(const Led_IdType id)
{
	if (id < LED_ID_UNKNOWN)
	{
		if (Led_Data.container[id].config.isInverted)
		{
			palSetPad(Led_Data.container[id].config.portNumber, Led_Data.container[id].config.pinNumber);
		}
		else
		{
			palClearPad(Led_Data.container[id].config.portNumber, Led_Data.container[id].config.pinNumber);
		}
		Led_Data.container[id].state = getLedState(id);
	}
}

static Led_StateType getLedState(const Led_IdType id)
{
	Led_StateType retVal = LED_STATE_UNKNOWN;
	if (id < LED_ID_UNKNOWN)
	{
		uint8_t tmp;

		if (Led_Config[id].isInverted)
		{
			tmp = ((~palReadPad(Led_Data.container[id].config.portNumber, Led_Data.container[id].config.pinNumber)) & 1u);
		}
		else
		{
			tmp = ((palReadPad(Led_Data.container[id].config.portNumber, Led_Data.container[id].config.pinNumber)) & 1u);
		}
		if (tmp != 0u)
		{
			retVal = LED_STATE_ON;
		}
		else
		{
			retVal = LED_STATE_OFF;
		}
	}

	return retVal;
}

static void toggleLed(const Led_IdType id)
{
	palTogglePad(Led_Data.container[id].config.portNumber, Led_Data.container[id].config.pinNumber);
	Led_Data.container[id].state = getLedState(id);
}

static void toggleExec(const Led_IdType id)
{
	switch (Led_Data.container[id].execState)
	{
	case LED_EXEC_STATE_INACTIVE:
		if (swTimer_isElapsed(Led_Data.container[id].timer.inactiveElapsed))
		{
			Led_Data.container[id].execState = LED_EXEC_STATE_ACTIVE;
			swTimer_set(Led_Data.container[id].timer.activeElapsed, Led_Data.container[id].timer.activeReload);
			setLedActive(id);
		}
		swTimer_tick(Led_Data.container[id].timer.inactiveElapsed);
		break;
	case LED_EXEC_STATE_ACTIVE:
		if (swTimer_isElapsed(Led_Data.container[id].timer.activeElapsed))
		{
			Led_Data.container[id].execState = LED_EXEC_STATE_INACTIVE;
			swTimer_set(Led_Data.container[id].timer.inactiveElapsed, Led_Data.container[id].timer.inactiveReload);
			setLedInactive(id);
		}
		swTimer_tick(Led_Data.container[id].timer.activeElapsed);
		break;
	default:
		swTimer_stop(Led_Data.container[id].timer.activeElapsed);
		swTimer_stop(Led_Data.container[id].timer.inactiveElapsed);
		setLedInactive(id);
		break;
	}
}

static void heartBeatExec(const Led_IdType id)
{
	switch (Led_Data.container[id].execState)
	{
	case LED_EXEC_STATE_INACTIVE:
		if (swTimer_isElapsed(Led_Data.container[id].timer.inactiveElapsed))
		{
			Led_Data.container[id].execState = LED_EXEC_STATE_ACTIVE;
			swTimer_set(Led_Data.container[id].timer.activeElapsed, Led_Data.container[id].timer.activeReload);
			Led_Data.container[id].timer.activeCyclesElapsed = Led_Data.container[id].timer.activeCyclesReload;
			setLedActive(id);
		}
		swTimer_tick(Led_Data.container[id].timer.inactiveElapsed);
		break;
	case LED_EXEC_STATE_ACTIVE:
		if (swTimer_isElapsed(Led_Data.container[id].timer.activeElapsed))
		{
			if (Led_Data.container[id].timer.activeCyclesElapsed == 0u)
			{
				Led_Data.container[id].execState = LED_EXEC_STATE_INACTIVE;
				swTimer_set(Led_Data.container[id].timer.inactiveElapsed, Led_Data.container[id].timer.inactiveReload);
				setLedInactive(id);
			}
			else
			{
				swTimer_set(Led_Data.container[id].timer.activeElapsed, Led_Data.container[id].timer.activeReload);
				Led_Data.container[id].timer.activeCyclesElapsed--;
				toggleLed(id);
			}
		}
		swTimer_tick(Led_Data.container[id].timer.activeElapsed);
		break;
	default:
		swTimer_stop(Led_Data.container[id].timer.activeElapsed);
		swTimer_stop(Led_Data.container[id].timer.inactiveElapsed);
		Led_Data.container[id].timer.activeCyclesElapsed = 0u;
		setLedInactive(id);
		break;
	}
}

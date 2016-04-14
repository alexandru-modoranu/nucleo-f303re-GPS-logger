#include <string.h>

#include "Led.h"
#include "board.h"
#include "hal_pal.h"
#include "SoftwareTimer.h"

typedef struct Led_ConfigTypeTag
{
	void (*exec)(const Led_IdType id);
	stm32_gpio_t *portNumber;
	uint32_t pinNumber;
	uint8_t isInverted;
} Led_ConfigType;

typedef struct Led_TimerTypeTag
{
	uint32_t shortPulseCounterValue;
	uint32_t shortPulseCounterReload;
	uint32_t longPulseCounterValue;
	uint32_t longPulseCounterReload;
	uint8_t shortPulseCyclesValue;
	uint8_t shortPulseCyclesReload;
	uint8_t longPulseCyclesValue;
	uint8_t longPulseCyclesReload;
} Led_TimerType;

typedef struct Led_ContainerTypeTag
{
	Led_ConfigType config;
	Led_TimerType timer;
	Led_StateType state;
} Led_ContainerType;

typedef struct Led_DataTypeTag
{
	uint32_t mainFunctionRec;
	Led_ContainerType container[LED_ID_UNKNOWN];
} Led_DataType;

static void setLedOn(const Led_IdType id);
static void setLedOff(const Led_IdType id);
static uint8_t getLedState(const Led_IdType id);
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
		setLedOff(id);
		Led_Data.container[id].state = LED_STATE_OFF;
		memcpy(&Led_Data.container[id].config, &Led_Config[id], sizeof(Led_Data.container[id].config));
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
		setLedOff(id);
		Led_Data.container[id].state = LED_STATE_OFF;
		memset(&Led_Data.container[id].timer, 0u, sizeof(Led_Data.container[id].timer));
	}
}

void Led_SetOn(const Led_IdType id)
{
	setLedOn(id);
}

void Led_SetOff(const Led_IdType id)
{
	setLedOff(id);
}

void Led_Toggle(const Led_IdType id, const uint32_t shortRec, const uint8_t shortcycles)
{
	if (id < LED_ID_UNKNOWN)
	{
		Led_Data.container[id].timer.shortPulseCounterReload = shortRec;
		Led_Data.container[id].timer.shortPulseCyclesReload = shortcycles;
		Led_Data.container[id].timer.longPulseCounterReload = 0u;
		Led_Data.container[id].timer.longPulseCounterReload = 0u;
	}
}

void Led_HeartBeat(const Led_IdType id, const uint32_t shortRec, const uint8_t shortCycles, const uint32_t longRec, const uint8_t longCycles)
{
	if (id < LED_ID_UNKNOWN)
	{
		Led_Data.container[id].timer.shortPulseCounterReload = shortRec;
		Led_Data.container[id].timer.shortPulseCyclesReload = shortCycles;
		Led_Data.container[id].timer.longPulseCounterReload = longRec;
		Led_Data.container[id].timer.longPulseCounterReload = longCycles;
	}
}

Led_StateType Led_GetState(const Led_IdType id)
{
	return Led_Data.container[id].state;
}


static void setLedOn(const Led_IdType id)
{
	if (id < LED_ID_UNKNOWN)
	{
		if (Led_Config[id].isInverted)
		{
			palClearPad(Led_Config[id].portNumber, Led_Config[id].pinNumber);
		}
		else
		{
			palSetPad(Led_Config[id].portNumber, Led_Config[id].pinNumber);
		}
		Led_Data.container[id].state = getLedState(id);
	}
}

static void setLedOff(const Led_IdType id)
{
	if (id < LED_ID_UNKNOWN)
	{
		if (Led_Config[id].isInverted)
		{
			palSetPad(Led_Config[id].portNumber, Led_Config[id].pinNumber);
		}
		else
		{
			palClearPad(Led_Config[id].portNumber, Led_Config[id].pinNumber);
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
			tmp = ((~palReadPad(Led_Config[id].portNumber, Led_Config[id].pinNumber)) & 1u);
		}
		else
		{
			tmp = ((palReadPad(Led_Config[id].portNumber, Led_Config[id].pinNumber)) & 1u);
		}
		if (tmp != 0u)
		{
			retVal = LED_STATE_ON;
		}
		else
		{
			retVal = LED_STATE_OFF;
		}
		Led_Data.container[id].state = getLedState(id);
	}

	return retVal;
}

static void toggleLed(const Led_IdType id)
{
	palTogglePad(Led_Config[id].portNumber, Led_Config[id].pinNumber);
	Led_Data.container[id].state = getLedState(id);
}

static void toggleExec(const Led_IdType id)
{

}

static void heartBeatExec(const Led_IdType id)
{

}

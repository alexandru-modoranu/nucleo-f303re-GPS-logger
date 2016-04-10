/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "SoftwareTimer.h"

static THD_WORKING_AREA(pool_Task_50ms, 128);
static THD_WORKING_AREA(pool_Task_5ms, 128);

static const int8_t casb_MsgToSend[] = "Received: ";
static volatile uint8_t rbi_LedStatus;
static uint8_t rub_LedTimeout;
static uint8_t buffer[64];

static const SerialConfig cs_Serial2Config =
{
  SERIAL_DEFAULT_BITRATE,
  0,
  USART_CR2_STOP1_BITS | USART_CR2_LINEN,
  0
};

/*
 * This callback is invoked when a transmission buffer has been completely
 * read by the driver.
 */
static void txend1(UARTDriver *uartp) {
    (void)uartp;
}

/*
 * This callback is invoked when a transmission has physically completed.
 */
static void txend2(UARTDriver *uartp) {
    (void)uartp;

}

/*
 * This callback is invoked on a receive error, the errors mask is passed
 * as parameter.
 */
static void rxerr(UARTDriver *uartp, uartflags_t e) {
    (void)uartp;
    (void)e;

}

/*
 * This callback is invoked when a character is received but the application
 * was not ready to receive it, the character is passed as parameter.
 */
static void rxchar(UARTDriver *uartp, uint16_t c) {
	uint8_t val = (uint8_t)c;
	(void)uartp;
	rbi_LedStatus ^= 1u;
	if (rbi_LedStatus == FALSE)
	{
		palClearPad(GPIOA, GPIOA_LED_GREEN);
	}
	else
	{
		palSetPad(GPIOA, GPIOA_LED_GREEN);
	}
    /* Flashing the LED each time a character is received.*/
    sdWrite(&SD2, (uint8_t *)&val, 1);
}

/*
 * This callback is invoked when a receive buffer has been completely written.
 */
static void rxend(UARTDriver *uartp) {
    /* When the buffer is full, flush it and restart listening */
    (void)uartp;
}

/*
 * UART driver configuration structure.
 */
static UARTConfig uartcfg = {
    txend1,                 /* End of transmission callback */
    txend2,                 /* Physical end of transmission callback */
    rxend,                  /* Receive buffer filled callback */
    rxchar,                 /* Character received while out */
    rxerr,                  /* Receive error callback */
    9600,
	0,
	USART_CR2_STOP1_BITS | USART_CR2_LINEN,
	0
};

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_FUNCTION(Task_50ms, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
	if (swTimer_isElapsed(rub_LedTimeout))
	{
		swTimer_set(rub_LedTimeout, 10);

	}

	swTimer_tick(rub_LedTimeout);
    chThdSleepMilliseconds(50);
  }
}

/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_FUNCTION(Task_5ms, arg) {

  (void)arg;
  chRegSetThreadName("button");

  while (TRUE) {
		chThdSleepMilliseconds(5);
  }
}

/*
 * Application entry point.
 */
void main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
     * Activates the serial driver 2 using the driver default configuration.
     */
  sdStart(&SD2, &cs_Serial2Config);
  uartStart(&UARTD3, &uartcfg);

  palSetPadMode(GPIOC, 10, PAL_MODE_ALTERNATE(7));	/* USART3_TX */
  palSetPadMode(GPIOC, 11, PAL_MODE_ALTERNATE(7));	/* USART3_RX */

  rbi_LedStatus = FALSE;
  rub_LedTimeout = 0;


  uartStopReceive(&UARTD3);
  uartStopSend(&UARTD3);
  uartStartReceive(&UARTD3, 64, buffer);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(pool_Task_50ms, sizeof(pool_Task_50ms), NORMALPRIO, Task_50ms, NULL);
  chThdCreateStatic(pool_Task_5ms, sizeof(pool_Task_5ms), NORMALPRIO, Task_5ms, NULL);

  chThdExit(0);
}
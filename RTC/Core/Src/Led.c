#include "Led.h"

#define MAX_BRIGHTNESS 100

void ledInit(LED* led, GPIO_TypeDef* port, uint16_t pin)
{
	led->port = port;
	led->pin = pin;
	ledOff(led);
}

void ledOn(LED* led)
{
	if (led->state != LED_ON) {
		HAL_GPIO_WritePin(led->port, led->pin, SET);
		led->state = LED_ON;
	}
}

void ledOff(LED* led)
{
	if (led->state != LED_OFF) {
		HAL_GPIO_WritePin(led->port, led->pin, RESET);
		led->state = LED_OFF;
	}
}


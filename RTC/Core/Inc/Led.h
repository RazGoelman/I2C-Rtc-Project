#ifndef INC_LED_H_
#define INC_LED_H_

#include "main.h"

typedef enum
{
	LED_OFF,
	LED_ON,
} LED_STATE;

typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;
	LED_STATE state;
} LED;

void ledInit(LED* led, GPIO_TypeDef* port, uint16_t pin);
void ledOn(LED* led);
void ledOff(LED* led);

#endif /* INC_LED_H_ */

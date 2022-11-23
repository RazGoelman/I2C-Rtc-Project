#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include <stdint.h>
#include "MyMain.h"

typedef struct
{
	TIM_HandleTypeDef* timer;
	uint32_t channel;
	uint32_t counter;
	uint32_t maxCount;
	int isOn;
} BUZZER;

void buzzerInit(BUZZER* buzzer, TIM_HandleTypeDef* timer, uint32_t channel);
void buzzerStart(BUZZER* buzzer, uint32_t maxCount);
void buzzerOnTimerInterrupt(BUZZER* buzzer);

#endif /* INC_BUZZER_H_ */

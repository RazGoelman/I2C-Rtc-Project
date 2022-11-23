#include "MyMain.h"
#include "Buzzer.h"

void buzzerInit(BUZZER* buzzer, TIM_HandleTypeDef* timer, uint32_t channel)
{
	buzzer->timer = timer;
	buzzer->channel = channel;
	buzzer->counter = 0;
	buzzer->maxCount = 0;
	buzzer->isOn = 0;
}

void buzzerStart(BUZZER* buzzer, uint32_t maxCount)
{
	HAL_TIM_PWM_Start(buzzer->timer, buzzer->channel);
	buzzer->maxCount = maxCount;
	buzzer->isOn = 1;
}

void buzzerOnTimerInterrupt(BUZZER* buzzer)
{
	if (buzzer->isOn)
	{
		buzzer->counter++;
		if ((buzzer->counter) >= (buzzer->maxCount))
		{
			HAL_TIM_PWM_Stop(buzzer->timer, buzzer->channel);
			buzzer->counter = 0;
			buzzer->isOn = 0;
		}
	}
}

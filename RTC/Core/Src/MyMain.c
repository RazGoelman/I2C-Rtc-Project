#include <stdio.h>
#include <string.h>
#include "DateTime.h"
#include "RTC.h"
#include "UARTProtocol.h"
#include "Led.h"
#include "Buzzer.h"
#include "Security.h"
#include "MyMain.h"

LED L1;
LED L2;
BUZZER buzzer;

void mainloop()
{
	// RTC
	rtcInit(&hi2c1, -1);

	// Security
	securityInit();

	// Leds
	ledInit(&L1, LED_L1_GPIO_Port, LED_L1_Pin);
	ledInit(&L2, LED_L2_GPIO_Port, LED_L2_Pin);

	// Buzzer
	buzzerInit(&buzzer, &htim3, TIM_CHANNEL_1);
	HAL_TIM_Base_Start(buzzer.timer);

	// 1 MS timer
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	HAL_TIM_Base_Start_IT(&htim6);

	// 1 Second timer
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_TIM_Base_Start_IT(&htim7);

	while(1)
	{
		commTask();
	}
}

void securityTask()
{
	static LOCK_STATE lockState = SECURITY_STATE_UNDEFINED;

	securityOnTimerInterrupt();
	LOCK_STATE newState = securityGetState();
	if (lockState != newState)
	{
		lockState = newState;
		switch (lockState)
		{
		case SECURITY_STATE_OPENED:
			ledOff(&L2);
			ledOn(&L1);
			break;
		case SECURITY_STATE_COOLDOWN:
			ledOff(&L1);
			ledOn(&L2);
			break;
		default:
			ledOff(&L1);
			ledOff(&L2);
			break;
		}
	}
}

int _write(int fd, char* ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    return len;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
	if (htim->Instance == TIM6) {
		buzzerOnTimerInterrupt(&buzzer);
	}
	else if (htim->Instance == TIM7) {
		securityTask();
	}
}

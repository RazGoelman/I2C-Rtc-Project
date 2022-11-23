#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "RTC.h"
#include "UARTProtocol.h"
#include "Led.h"
#include "Security.h"
#include "MyMain.h"

#define MAX_BUFFER_LENGTH 100

static uint8_t cmdbuffer[MAX_BUFFER_LENGTH];
static int cmdcount = 0;
static int cmdprint = 0;

static void handleCommand();

void commTask()
{
	uint8_t ch;

	HAL_StatusTypeDef Status = HAL_UART_Receive(&huart2, &ch, 1, 10);

	if (Status != HAL_OK)
	{
		if ((huart2.Instance->ISR & USART_ISR_ORE) != 0)
		{
			__HAL_UART_CLEAR_OREFLAG(&huart2);
		}

		while (cmdprint < cmdcount)
		{
			HAL_UART_Transmit(&huart2, &cmdbuffer[cmdprint++], 1, HAL_MAX_DELAY);
		}

		return;
	}

	if (ch == '\b')
	{
		cmdcount--;
		cmdprint--;

		HAL_UART_Transmit(&huart2, (uint8_t*)"\b \b", 3, HAL_MAX_DELAY);

		return;
	}

	if (ch != '\r' && ch != '\n')
	{
		if (cmdcount >= MAX_BUFFER_LENGTH)
		{
			cmdcount = 0;
			cmdprint = 0;
		}

		cmdbuffer[cmdcount++] = ch;

		return;
	}

	while (cmdprint < cmdcount)
	{
		HAL_UART_Transmit(&huart2, &cmdbuffer[cmdprint++], 1, 0xFFFF);
	}

	HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, 0xFFFF);

	cmdbuffer[cmdcount] = 0;
	cmdcount = 0;
	cmdprint = 0;

	handleCommand();
}

static void handleCommand()
{
	char cmd[20]   = { 0 };
	char param[50] = { 0 };

	int numParams = sscanf((const char*)cmdbuffer, "%s %s", cmd, param);

	if (numParams == 0)
	{
	  return;
	}

	if ((strcmp(cmd, "gettime") == 0) && (numParams == 1))
	{
		DateTime_t dt;
		rtcGetTime(&dt);
		dateTimePrint(&dt);
	}
	else if ((strcmp(cmd, "settime") == 0) && (numParams == 2))
	{
		rtcSetTimeString(param);
	}
	else if ((strcmp(cmd, "ledon") == 0) && (numParams == 2))
	{
		if (strcmp(param, "L1") == 0)
		{
			ledOn(&L1);
		}
		if (strcmp(param, "L2") == 0)
		{
			ledOn(&L2);
		}
	}
	else if ((strcmp(cmd, "ledoff") == 0) && (numParams == 2))
	{
		if (strcmp(param, "L1") == 0)
		{
			ledOff(&L1);
		}
		if (strcmp(param, "L2") == 0)
		{
			ledOff(&L2);
		}
	}
	else if ((strcmp(cmd, "setcode") == 0) && (numParams == 2))
	{
		int res = securitySetCode(param, strlen(param) + 1);

		switch (res)
		{
		case SECURITY_ERROR_NONE:
			printf("New code successfully set\r\n");
			break;
		case SECURITY_ERROR_BAD_FORMAT:
			printf("Illegal code\r\n");
			break;
		case SECURITY_ERROR_AUTHORIZATION:
			printf("No authorization for operation\r\n");
			break;
		default:
			break;
		}
	}
	else if ((strcmp(cmd, "entercode") == 0) && (numParams == 2))
	{
		int res = securityEnterCode(param, strlen(param) + 1);

		switch (res)
		{
		case SECURITY_ERROR_NONE:
			printf("System was successfully unlocked!\r\n");
			break;
		case SECURITY_ERROR_INVALID_REQUEST:
			printf("Invalid operation - system is already unlocked\r\n");
			break;
		case SECURITY_ERROR_WRONG_CODE:
			buzzerStart(&buzzer, 700);
			printf("Wrong code\r\n");
			break;
		case SECURITY_ERROR_COOLDOWN:
			printf("Please wait %d seconds before attempting to unlock again\r\n", securityRemainingLockTime());
			break;
		default:
			break;
		}
	}
	else if ((strcmp(cmd, "systemlock") == 0) && (numParams == 1))
	{
		int res = securityLockSystem();

		switch (res)
		{
		case SECURITY_ERROR_NONE:
			printf("System is locked\r\n");
			break;
		case SECURITY_ERROR_AUTHORIZATION:
			printf("No authorization for operation\r\n");
			break;

		default:
			break;
		}
	}
	else
	{
	  printf("Invalid command\r\n");
	}
}


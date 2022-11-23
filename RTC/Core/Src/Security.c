#include <string.h>
#include <stdlib.h>
#include "RTC.h"
#include "Security.h"

#define INIT_VAL				0x123456789
#define CODE_MAX_LENGTH			0x10
#define CODE_ADDRESS			(RTC_PERSISTENT_ADDEESS)
#define STATUS_ADDRESS			(CODE_ADDRESS + CODE_MAX_LENGTH)

#define SOFT_LOCK_MAX_TRIES		3
#define SOFT_LOCK_TIME			30

#define HARD_LOCK_MAX_TRIES		9
#define HARD_LOCK_TIME			300

typedef enum
{
	SECURITY_LOCK_UNDEFINED,
	SECURITY_LOCK_SOFT,
	SECURITY_LOCK_HARD
} LOCK_TYPE;

static struct
{
	uint64_t initVal;
	uint8_t tries;
	DateTime_t lastTryTimestamp;
	DateTime_t lockTimestamp;
	LOCK_TYPE lockType;
	LOCK_STATE state;
} security;

static uint8_t sessionTries = 0;

void securityInit()
{
	rtcLoadPersistent((uint8_t *)&security, sizeof(security), STATUS_ADDRESS);

	int saveRequired = 0;

	// First use of system - keep unlocked until first set code
	if (security.initVal != INIT_VAL)
	{
		memset(&security, 0, sizeof(security));
		security.initVal = INIT_VAL;
		security.state = SECURITY_STATE_OPENED;
		security.lockType = SECURITY_LOCK_UNDEFINED;
		saveRequired = 1;
	}

	// Rebooting during soft lock removes CD time
	if (security.state == SECURITY_STATE_COOLDOWN && security.lockType == SECURITY_LOCK_SOFT)
	{
		security.state = SECURITY_STATE_LOCKED;
		saveRequired = 1;
	}

	// Reset login tries if enough time had passed
	if (rtcElapsedTime(&security.lastTryTimestamp) >= HARD_LOCK_TIME)
	{
		security.tries = 0;
		saveRequired = 1;
	}

	// Saving status to memory if needed
	if (saveRequired)
	{
		rtcSavePersistent((uint8_t *)&security, sizeof(security), STATUS_ADDRESS);
	}
}

int securitySetCode(char *inputCode, size_t length)
{
	if (security.state != SECURITY_STATE_OPENED)
	{
		return SECURITY_ERROR_AUTHORIZATION;
	}

	if (length > CODE_MAX_LENGTH)
	{
		return SECURITY_ERROR_BAD_FORMAT;
	}

	// Save new code
	rtcSavePersistent((uint8_t *)inputCode, length, CODE_ADDRESS);

	// Change saved state to locked - necessary for first use
	LOCK_STATE lockedState = SECURITY_STATE_LOCKED;
	rtcSavePersistent((uint8_t *)&lockedState, sizeof(lockedState),
		STATUS_ADDRESS + offsetof(typeof(security), state));

	return SECURITY_ERROR_NONE;
}

int securityEnterCode(char *inputCode, size_t length)
{
	if (security.state == SECURITY_STATE_COOLDOWN)
	{
		return SECURITY_ERROR_COOLDOWN;
	}

	if (security.state == SECURITY_STATE_OPENED)
	{
		return SECURITY_ERROR_INVALID_REQUEST;
	}

	char realCode[CODE_MAX_LENGTH] = { 0 };
	rtcLoadPersistent((uint8_t *)realCode, CODE_MAX_LENGTH, CODE_ADDRESS);

	if (strcmp(inputCode, realCode))
	{
		rtcGetTime(&security.lastTryTimestamp);
		security.tries++;
		sessionTries++;

		if ((security.tries == HARD_LOCK_MAX_TRIES) || (sessionTries == SOFT_LOCK_MAX_TRIES))
		{
			rtcGetTime(&security.lockTimestamp);
			security.state = SECURITY_STATE_COOLDOWN;
			security.lockType = ((security.tries == HARD_LOCK_MAX_TRIES) ?
				SECURITY_LOCK_HARD : SECURITY_LOCK_SOFT);
		}

		rtcSavePersistent((uint8_t *)&security, sizeof(security), STATUS_ADDRESS);

		return SECURITY_ERROR_WRONG_CODE;
	}

	security.tries = 0;
	sessionTries = 0;
	rtcSavePersistent((uint8_t *)&security, sizeof(security), STATUS_ADDRESS);
	security.state = SECURITY_STATE_OPENED;

	return SECURITY_ERROR_NONE;
}

int securityLockSystem()
{
	if (security.state == SECURITY_STATE_OPENED)
	{
		security.state = SECURITY_STATE_LOCKED;
		rtcSavePersistent((uint8_t *)&security, sizeof(security), STATUS_ADDRESS);
		return SECURITY_ERROR_NONE;
	}

	return SECURITY_ERROR_AUTHORIZATION;
}

int securityRemainingLockTime()
{
	return (security.lockType == SECURITY_LOCK_HARD ? HARD_LOCK_TIME : SOFT_LOCK_TIME) -
			rtcElapsedTime(&security.lockTimestamp);
}

void securityOnTimerInterrupt()
{
	if (security.state == SECURITY_STATE_COOLDOWN)
	{
		if(securityRemainingLockTime() <= 0)
		{
			if (security.lockType == SECURITY_LOCK_HARD)
			{
				security.tries = 0;
			}

			sessionTries = 0;

			security.state = SECURITY_STATE_LOCKED;
			security.lockType = SECURITY_LOCK_UNDEFINED;
			rtcSavePersistent((uint8_t *)&security, sizeof(security), STATUS_ADDRESS);
		}
	}
}

LOCK_STATE securityGetState()
{
	return security.state;
}

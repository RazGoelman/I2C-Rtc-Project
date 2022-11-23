#ifndef INC_SECURITY_H_
#define INC_SECURITY_H_

typedef enum
{
	SECURITY_STATE_UNDEFINED,
	SECURITY_STATE_LOCKED,
	SECURITY_STATE_OPENED,
	SECURITY_STATE_COOLDOWN
}LOCK_STATE;

typedef enum
{
	SECURITY_ERROR_NONE,
	SECURITY_ERROR_BAD_FORMAT,
	SECURITY_ERROR_AUTHORIZATION,
	SECURITY_ERROR_WRONG_CODE,
	SECURITY_ERROR_INVALID_REQUEST,
	SECURITY_ERROR_COOLDOWN
}LOCK_ERROR;

void securityInit();
int securitySetCode(char *inputCode, size_t length);
int securityEnterCode(char *inputCode, size_t length);
int securityRemainingLockTime();
int securityLockSystem();

LOCK_STATE securityGetState();

void securityOnTimerInterrupt();

#endif /* INC_SECURITY_H_ */

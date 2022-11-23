#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "DateTime.h"
#include "MyMain.h"

#define RTC_ADDRESS     		0xD0
#define RTC_MEM_ADDRESS 	 	0x00
#define RTC_PERSISTENT_ADDEESS  0x10

typedef struct
{
	I2C_HandleTypeDef *i2cHandle;
	uint32_t timeout;
} RTC_t;

extern RTC_t rtc;

void rtcInit(I2C_HandleTypeDef *i2c, uint32_t timeout);

void rtcGetTime(DateTime_t *dt);
void rtcSetTimeDT(DateTime_t *dt);
void rtcSetTimeString(char *timeString);
int rtcElapsedTime(DateTime_t *dt);

void rtcSavePersistent(uint8_t* data, uint8_t numBytes, uint8_t address);
void rtcLoadPersistent(uint8_t* data, uint8_t numBytes, uint8_t address);

#endif /* INC_RTC_H_ */

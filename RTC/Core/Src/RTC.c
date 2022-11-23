#include "RTC.h"

RTC_t rtc;

void rtcInit(I2C_HandleTypeDef *i2c, uint32_t timeout)
{
	rtc.i2cHandle = i2c;
	rtc.timeout = timeout;
}

void rtcGetTime(DateTime_t *dt)
{
	uint8_t timeRawData[7] = { 0 };
	HAL_I2C_Mem_Read(rtc.i2cHandle, RTC_ADDRESS, RTC_MEM_ADDRESS, 1, timeRawData, sizeof(timeRawData), rtc.timeout);
	dateTimeFromRawData(timeRawData, dt);
}

void rtcSetTimeDT(DateTime_t *dt)
{
	uint8_t timeRawData[7] = { 0 };
	dateTimeToRawData(timeRawData, dt);
	HAL_I2C_Mem_Write(rtc.i2cHandle, RTC_ADDRESS, RTC_MEM_ADDRESS, 1, timeRawData, sizeof(timeRawData), rtc.timeout);
}

void rtcSetTimeString(char *timeString)
{
	uint8_t timeRawData[7] = { 0 };
	DateTime_t dt;
	dateTimeFromString(timeString, &dt);
	dateTimeToRawData(timeRawData, &dt);
	HAL_I2C_Mem_Write(rtc.i2cHandle, RTC_ADDRESS, RTC_MEM_ADDRESS, 1, timeRawData, sizeof(timeRawData), rtc.timeout);
}

int rtcElapsedTime(DateTime_t *dt)
{
	DateTime_t now;
	rtcGetTime(&now);
	return dateTimeDiff(dt, &now);
}

void rtcSavePersistent(uint8_t* data, uint8_t numBytes, uint8_t address)
{
	HAL_I2C_Mem_Write(rtc.i2cHandle, RTC_ADDRESS, address, 1, data, numBytes, rtc.timeout);
}

void rtcLoadPersistent(uint8_t* data, uint8_t numBytes, uint8_t address)
{
	HAL_I2C_Mem_Read(rtc.i2cHandle, RTC_ADDRESS, address, 1, data, numBytes, rtc.timeout);
}

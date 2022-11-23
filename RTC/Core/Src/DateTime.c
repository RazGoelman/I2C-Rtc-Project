#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MyMain.h"
#include "DateTime.h"

const char *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void dateTimeFromRawData(const uint8_t *data, DateTime_t *dt)
{
	dt->second = (data[0] & 0xF) + (data[0] >> 4) * 10;
	dt->minute = (data[1] & 0xF) + (data[1] >> 4) * 10;

	dt->hour   = ((data[2] & 0xF) +
			     ((data[2] & 0x10) >> 4) * 10 +
			     ((data[2] & 0x20) >> 5) * (data[2] & 0x40 ? 12 : 20));

	dt->day    = (data[3] & 0xF);
	dt->date   = (data[4] & 0xF) + (data[4] >> 4) * 10;
	dt->month  = (data[5] & 0xF) + (data[5] >> 4) * 10;
	dt->year   = (data[6] & 0xF) + (data[6] >> 4) * 10;
}

void dateTimeToRawData(uint8_t *data, const DateTime_t *dt)
{
	data[0] = (dt->second % 10) | ((dt->second / 10) << 4);
	data[1] = (dt->minute % 10) | ((dt->minute / 10) << 4);
	data[2] = (dt->hour   % 10) | ((dt->hour   / 10) << 4);
	data[3] = (dt->day);
	data[4] = (dt->date   % 10) | ((dt->date   / 10) << 4);
	data[5] = (dt->month  % 10) | ((dt->month  / 10) << 4);
	data[6] = (dt->year   % 10) | ((dt->year   / 10) << 4);
}

void dateTimeFromString(char *timeString, DateTime_t *dt)
{
	char *delim = ":";
	char *token = 0;

	token = strtok(timeString, delim);
	dt->year   = atoi(token);
	token = strtok(NULL, delim);
	dt->month  = atoi(token);
	token = strtok(NULL, delim);
	dt->date   = atoi(token);
	token = strtok(NULL, delim);
	dt->day    = atoi(token);
	token = strtok(NULL, delim);
	dt->hour   = atoi(token);
	token = strtok(NULL, delim);
	dt->minute = atoi(token);
	token = strtok(NULL, delim);
	dt->second = atoi(token);
}

uint32_t dateTimeToSeconds(const DateTime_t *dt)
{
	// Improve this calculation

	uint32_t res =
		(dt->second) +
		(dt->minute * 60) +
		(dt->hour * 60 * 60) +
		(dt->date * 60 * 60 * 24) +
		(dt->month * 60 * 60 * 24 * 30) +
		(dt->year * 60 * 60 * 24 * 30 * 12);

	return res;
}

int dateTimeDiff(DateTime_t *t1, DateTime_t *t2)
{
	return (dateTimeToSeconds(t2) - dateTimeToSeconds(t1));
}

void dateTimePrint(DateTime_t *dt)
{
	printf("%02u/%02u/%02u, %s %02u:%02u:%02u\r\n",
	   dt->year, dt->month, dt->date, days[dt->day - 1],
	   dt->hour, dt->minute, dt->second);
}


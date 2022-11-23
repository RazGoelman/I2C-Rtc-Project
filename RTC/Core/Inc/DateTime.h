#ifndef INC_DATETIME_H_
#define INC_DATETIME_H_

#include <stdint.h>

typedef struct
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}DateTime_t;

void dateTimeFromRawData(const uint8_t *data, DateTime_t *dt);
void dateTimeFromString(char *timeString, DateTime_t *dt);
void dateTimeToRawData(uint8_t *data, const DateTime_t *dt);
uint32_t dateTimeToSeconds(const DateTime_t *dt);
int dateTimeDiff(DateTime_t *t1, DateTime_t *t2);
void dateTimePrint(DateTime_t *dt);

#endif /* INC_DATETIME_H_ */

#ifndef INC_MYMAIN_H_
#define INC_MYMAIN_H_

#include "Led.h"
#include "Buzzer.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

extern LED L1;
extern LED L2;
extern BUZZER buzzer;

void mainloop();

#endif /* INC_MYMAIN_H_ */

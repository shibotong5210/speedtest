#ifndef __EXTI_H
#define __EXTI_H

#include "stm32f10x.h"

#define PULSE_PIN1 GPIO_Pin_0
#define PULSE_PIN2 GPIO_Pin_1
#define EXTI_PIN1 GPIO_Pin_0
#define EXTI_PIN2 GPIO_Pin_1
#define EXTI_LINE1 EXTI_Line0
#define EXTI_LINE2 EXTI_Line1
#define EXTI_PORT GPIOB

void GPIO_Configuration(void);
void EXTI_Configuration(void);

#endif

#ifndef __LED_H
#define __LED_H
#include "sys.h"
#include "delay.h"
///////

#define BEEP_TIM TIM3

#define LED PBout(4) // PB4

#define PEDAL_IN PBin(2)    // PB2
#define BEEP PBout(5)       // PB5
#define Speed_Up PAin(11)   // PA11
#define Speed_Down PAin(12) // PA12
#define PEDAL 1
#define UP 2
#define DOWN 3

void TIM3_Config(void);
void LED_Init(void); // ≥ı ºªØ
void Init_Pwm(void);
void Key_GPIO_Config(void);
void TIM4_Configuration(void);
void Beep_Init(void);
void Motor_On(uint8_t Value);
uint8_t Input_Scan(void);
#endif

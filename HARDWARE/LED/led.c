#include "led.h"
/**
 * @brief 系统io输出定义
 *
 * @return * void
 */

extern uint32_t Speed_1;
extern uint32_t Speed_2;
extern double Velocity;
// TIM3 定时器中断初始化 0.1s定时
void TIM3_Config(void)
{
    // 10Hz 0.1s
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 使能TIM3时钟
    TIM_DeInit(TIM3);                                    // 复位TIM3
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;        // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;       // 定时器分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             // 初始化TIM3
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                  // 允许更新中断
    TIM_Cmd(TIM3, ENABLE);                                      // 使能定时器4
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             // 定时器4中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3, DISABLE); // 关闭定时器4
}
// TIM4
void TIM4_Configuration(void)
{ // 定时器配置函数
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    /* Enable the TIM3 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;           // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;         //
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    TIM_Cmd(TIM4, ENABLE);
}

//  TIM 2
void Init_Pwm(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; /*配置GPIO*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  /*开定时器2时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*GPIO时钟开启*/

    /*引脚配置*/

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*复用输出*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*复用输出*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO PA2 初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*复用输出*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO PA3 初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*复用输出*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*配置TIMER2*/
    /*配置TIMER2*/
    TIM_DeInit(TIM2); /*初始化定时器2*/
    // 72M/72/1000=100Hz
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;                /*重装值 计数到最大为3600-1 然后从0开始*/
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;              /*分频系数*/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     /*时钟分割*/
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /*向上计数*/

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /*配置TIMER2PWM输出*/
    /*配置TIMER2PWM输出*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             /*PWM2模式输出*/
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; /*比较输出使能*/
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     /*输出极性高 输出高电平
                                                                   */

    /*通道1*/
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    /*通道2*/
    TIM_OCInitStructure.TIM_Pulse = 0;                /*设置占空比*/
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);          /*初始化外设TIMx 通道*/
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); /*CH1预装载使能*/
    /*通道3*/
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    /*通道4*/
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE); /*使能定时器2*/
}

void Motor_On(uint8_t Value)
{
    uint16_t temp = 0;
    temp = Value * 10;
    TIM_SetCompare1(TIM2, temp); // PA0
    TIM_SetCompare2(TIM2, temp); // PA1
}

// LED IO初始化
void LED_Init(void)
{
    // PB4 引脚输出初始化
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);
}
// 蜂鸣器引脚初始化
void Beep_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

uint8_t Input_Scan(void)
{
    if (Speed_Up == 1 || Speed_Down == 1)
    {
        delay_ms(10); // 防抖
        if (Speed_Up == 1)
            return UP;
        if (Speed_Down == 1)
            return DOWN;
        else
            return 0;
    }
    else
        return 0;
}

// 按键开关输入初始化
void Key_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;     // 选择按键引脚PB2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 设置引脚模式为上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);        // 初始化按键
    // PA11 PA12 初始化
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12; // PA11 PA12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

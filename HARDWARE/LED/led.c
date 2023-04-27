#include "led.h"
/**
 * @brief ϵͳio�������
 *
 * @return * void
 */

extern uint32_t Speed_1;
extern uint32_t Speed_2;
extern double Velocity;
// TIM3 ��ʱ���жϳ�ʼ�� 0.1s��ʱ
void TIM3_Config(void)
{
    // 10Hz 0.1s
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // ʹ��TIM3ʱ��
    TIM_DeInit(TIM3);                                    // ��λTIM3
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;        // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;       // ��ʱ����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             // ��ʼ��TIM3
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                  // ��������ж�
    TIM_Cmd(TIM3, ENABLE);                                      // ʹ�ܶ�ʱ��4
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             // ��ʱ��4�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM3, DISABLE); // �رն�ʱ��4
}
// TIM4
void TIM4_Configuration(void)
{ // ��ʱ�����ú���
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
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;           // �Զ���װ��ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;         //
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    TIM_Cmd(TIM4, ENABLE);
}

//  TIM 2
void Init_Pwm(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; /*����GPIO*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  /*����ʱ��2ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); /*GPIOʱ�ӿ���*/

    /*��������*/

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*�������*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*�������*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO PA2 ��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*�������*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // GPIO PA3 ��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; /*�������*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*����TIMER2*/
    /*����TIMER2*/
    TIM_DeInit(TIM2); /*��ʼ����ʱ��2*/
    // 72M/72/1000=100Hz
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;                /*��װֵ ���������Ϊ3600-1 Ȼ���0��ʼ*/
    TIM_TimeBaseStructure.TIM_Prescaler = 720 - 1;              /*��Ƶϵ��*/
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     /*ʱ�ӷָ�*/
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; /*���ϼ���*/

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /*����TIMER2PWM���*/
    /*����TIMER2PWM���*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             /*PWM2ģʽ���*/
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; /*�Ƚ����ʹ��*/
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     /*������Ը� ����ߵ�ƽ
                                                                   */

    /*ͨ��1*/
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    /*ͨ��2*/
    TIM_OCInitStructure.TIM_Pulse = 0;                /*����ռ�ձ�*/
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);          /*��ʼ������TIMx ͨ��*/
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); /*CH1Ԥװ��ʹ��*/
    /*ͨ��3*/
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    /*ͨ��4*/
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE); /*ʹ�ܶ�ʱ��2*/
}

void Motor_On(uint8_t Value)
{
    uint16_t temp = 0;
    temp = Value * 10;
    TIM_SetCompare1(TIM2, temp); // PA0
    TIM_SetCompare2(TIM2, temp); // PA1
}

// LED IO��ʼ��
void LED_Init(void)
{
    // PB4 ���������ʼ��
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
// ���������ų�ʼ��
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
        delay_ms(10); // ����
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

// �������������ʼ��
void Key_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;     // ѡ�񰴼�����PB2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // ��������ģʽΪ��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);        // ��ʼ������
    // PA11 PA12 ��ʼ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12; // PA11 PA12
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

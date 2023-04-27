#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "key.h"
#include "24cxx.h"
#include "myiic.h"
#include "vl53l0x.h"
#include "oled.h"
#include "exti.h"
/**
 * @brief ������
 * @author BT.S
 * @date 2023-04-23
 * @return int
 */
#define Set_Speed 25     // �趨�ٶ�ֵ,������ֵ����,�����Լ��뷨���ã���������25���ٶȵ�λ60�����쾯��
#define Set_Distance 500 // �趨����ֵ,������ֵ����
#define Distance_2 400   // �趨����ֵ,������ֵ����
#define Distance_3 300   // �趨����ֵ,������ֵ����
#define Distance_4 200   // �趨����ֵ,������ֵ����
#define Distance_5 100   // �趨����ֵ,������ֵ����

extern VL53L0X_Dev_t vl53l0x_dev;                     // �豸I2C���ݲ���
extern VL53L0X_RangingMeasurementData_t vl53l0x_data; // �������ṹ��
extern vu16 Distance_data;                            // ����������
uint32_t Speed_1;
uint32_t Speed_2;
uint32_t Average_Speed;
uint16_t BEEP_Time = 0;
uint16_t BEEP_Counter = 0;
uint8_t PEDAL_STATE = 0; // ��̤��״̬,1Ϊ����,0Ϊ�ɿ�
int16_t Speed_Value = 0; // �趨�ٶ�ֵ
double Velocity = 0;     // �ٶ�
int8_t WARNING_Flag = 0; // �ٶȱ�־λ
/**
 * @brief ��������
 *
 */
void OLED_UI(void);
void OLED_DIS_Level(void);
/**
 * @brief ������
 *
 * @return int
 */
int main(void)
{
    /***
     * @brief  ϵͳ��ʼ��
     */
    uint8_t k;
    VL53L0X_Error Status;                           // ״̬
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����2
    delay_init();                                   // ��ʱ������ʼ��
    uart_init(115200);                              // ���ڳ�ʼ��Ϊ115200
    Key_GPIO_Config();
    // ��ʱ�� TIM3��ʼ�� TIM4��ʼ��
    TIM3_Config();
    TIM4_Configuration();
    // �ⲿ�жϣ����ձ�������ʼ��
    EXTI_Configuration();
    // LED ������ �������� ���������ʼ��
    LED_Init();
    Beep_Init();
    BEEP = 0; // �رշ�����
    LED = 1;  // �ر�LED
    // ������PWM��ʼ��
    Init_Pwm();
    Motor_On(0); // �رյ��
                 // OLED��ʼ��
    OLED_Init();
    OLED_Clear(); // ����
    // ��ʾ����
    OLED_ShowString(50, 0, "W.W");
    OLED_ShowString(10, 2, "WARNING SYSTEM");
    for (k = 0; k < 6; k++)
    {
        OLED_ShowCHinese(20 + 16 * k, 4, 8 + k);
    }
    delay_ms(1000);
    delay_ms(1000);
    OLED_Clear();
    // ��ʼ��VL53L0X
    vl53l0x_init(&vl53l0x_dev);        // ��ʼ��vl53l0x
    vl53l0x_set_mode(&vl53l0x_dev, 0); // ����Ĭ�ϲ���ģʽ

    while (1)
    {
        // ���̤��״̬
        PEDAL_STATE = !PEDAL_IN; // �͵�ƽ���ذ��£��ߵ�ƽ�����ɿ�     // �ߵ�ƽIN = 1,�͵�ƽ IN = 0

        // ����ٶȵ���
        if (Speed_Down == 0)
            Speed_Value += 20;

        if (Speed_Value > 100)
            Speed_Value = 0;

        // ������
        vl53l0x_general_test(&vl53l0x_dev); // vl53l0x����
        // key_scan();
        PEDAL_STATE = !PEDAL_IN;
        // ����
        Average_Speed = (uint32_t)(Speed_1 + Speed_2) / 2;
        // �����ж�

        if (Average_Speed > Set_Speed)
        {
            // ����������
            if (Distance_data < Set_Distance)
            {
                if (PEDAL_STATE == 1) // ��������
                {
                    WARNING_Flag = 1;
                    BEEP_Time = 15; // 1.5s
                    if (Distance_data < Distance_2)
                    {
                        BEEP_Time = 10; // 1.0s
                    }
                    if (Distance_data < Distance_3)
                    {
                        BEEP_Time = 6; // 0.6s
                    }
                    if (Distance_data < Distance_4)
                    {
                        BEEP_Time = 3; // 0.3s
                    }
                    if (Distance_data < Distance_5)
                    {
                        BEEP_Time = 1; // 0.1s
                    }
                    TIM_Cmd(BEEP_TIM, ENABLE); // ����������
                }
                else
                {
                    WARNING_Flag = 0;
                    BEEP = 0;                   // �رշ�����
                    LED = 1;                    // �ر�LED
                    TIM_Cmd(BEEP_TIM, DISABLE); // �رշ�����
                }
            }
            else
            {
                WARNING_Flag = 0;
                BEEP = 0;                   // �رշ�����
                LED = 1;                    // �ر�LED
                TIM_Cmd(BEEP_TIM, DISABLE); // �رշ�����
            }
        }
        else
        {
            WARNING_Flag = 0;
            BEEP = 0;                   // �رշ�����
            LED = 1;                    // �ر�LED
            TIM_Cmd(BEEP_TIM, DISABLE); // �رշ�����
        }
        // ������ƣ�����̤���ж�
        if (PEDAL_STATE == 1)
        {
            Motor_On(Speed_Value);
        }
        else
        {
            Motor_On(0);
        }

        // ��ʾUI
        OLED_UI();
    }
}

// ��OLED����ʾ���ݣ��ٶȣ������̤��״̬
void OLED_UI(void)
{
    // ���ٶȣ�����ת��Ϊ�ַ���
    char Speed[10];
    char Distance[10];
    char Level[10];
    uint8_t i;
    /*������*/
    OLED_ShowString(40, 0, "           ");
    OLED_ShowString(40, 2, "        ");
    OLED_ShowString(40, 4, "        ");
    OLED_Set_Pos(0, 7);
    for (i = 0; i < 128; i++)
    {
        OLED_WR_Byte(0x00, OLED_DATA);
    }
    sprintf(Speed, "%.1lfmm/s", Velocity);

    sprintf(Distance, "%dmm", Distance_data);
    sprintf(Level, "%d", Speed_Value);
    // ��ʾ�ٶ�
    OLED_ShowCHinese(0, 0, 0);
    OLED_ShowCHinese(16, 0, 1);
    OLED_ShowChar(32, 0, ':');
    OLED_ShowString(40, 0, Speed);

    // ��ʾ����
    OLED_ShowCHinese(0, 2, 2);
    OLED_ShowCHinese(16, 2, 3);
    OLED_ShowChar(32, 2, ':');
    OLED_ShowString(40, 2, Distance);
    // ��ʾ�ٶȵ�λ
    OLED_ShowCHinese(0, 4, 4);
    OLED_ShowCHinese(16, 4, 5);
    OLED_ShowChar(32, 4, ':');
    OLED_ShowString(40, 4, Level);
    // ��ʾ̤��״̬
    OLED_ShowCHinese(95, 2, 6);      // ��
    OLED_ShowCHinese(95 + 16, 2, 7); // ��
    if (PEDAL_STATE == 1)
    {
        OLED_ShowString(100, 4, "ON ");
    }
    else
    {
        OLED_ShowString(100, 4, "OFF");
    }
    OLED_DIS_Level();

    if (WARNING_Flag == 1)
        OLED_ShowString(65, 4, "!!!"); // ��ʾ��̾��
    else
        OLED_ShowString(65, 4, "   "); // ����ʾ��̾��
}

/**
 * @brief OLEDλ�õ�λ��ʾ
 *
 */
void OLED_DIS_Level(void)
{
    uint8_t i = 0;
    uint8_t num = 0;
    if (Distance_data > 2000)
        num = 128;
    else
        num = (uint8_t)((Distance_data / 2000.0) * 128);
    num = 128 - num;
    OLED_Set_Pos(0, 7);
    for (i = 0; i < num; i++)
    {
        OLED_WR_Byte(0xFF, OLED_DATA);
    }
}
/**
 * @brief  1s�ļ������������ڼ��㳵�����ٶ�
 * ����ֱ��d = 6.5cm һȦ���ܳ�L = pi *d = 20.41cm һȦ��������N = 20 * 2 = 40
 * ���� v = 20.41 * (N) / 40 N Ϊ1s�ڵ�������
 */

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        Velocity = (double)(Speed_1 + Speed_2) / (2.0 * 40.0) * (204.1);
        Speed_1 = 0;
        Speed_2 = 0; // �ٶ�����
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}
/**
 * @brief TIM3�жϷ����� 0.1s��ʱ
 * ���ڷ�������LED�Ŀ��ƣ������
 *
 */
void TIM3_IRQHandler(void)
{
    // 0.1s�ж�һ��
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        BEEP_Counter++;
        if (BEEP_Counter > BEEP_Time)
        {
            BEEP = !BEEP;
            LED = !LED;
            BEEP_Counter = 0;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // ����жϱ�־λ
    }
}

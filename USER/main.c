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
 * @brief 主函数
 * @author BT.S
 * @date 2023-04-23
 * @return int
 */
#define Set_Speed 25     // 设定速度值,超过该值报警,根据自己想法设置；这里设置25；速度挡位60以上响警报
#define Set_Distance 500 // 设定距离值,超过该值报警
#define Distance_2 400   // 设定距离值,超过该值报警
#define Distance_3 300   // 设定距离值,超过该值报警
#define Distance_4 200   // 设定距离值,超过该值报警
#define Distance_5 100   // 设定距离值,超过该值报警

extern VL53L0X_Dev_t vl53l0x_dev;                     // 设备I2C数据参数
extern VL53L0X_RangingMeasurementData_t vl53l0x_data; // 测距测量结构体
extern vu16 Distance_data;                            // 保存测距数据
uint32_t Speed_1;
uint32_t Speed_2;
uint32_t Average_Speed;
uint16_t BEEP_Time = 0;
uint16_t BEEP_Counter = 0;
uint8_t PEDAL_STATE = 0; // 脚踏板状态,1为按下,0为松开
int16_t Speed_Value = 0; // 设定速度值
double Velocity = 0;     // 速度
int8_t WARNING_Flag = 0; // 速度标志位
/**
 * @brief 函数声明
 *
 */
void OLED_UI(void);
void OLED_DIS_Level(void);
/**
 * @brief 主函数
 *
 * @return int
 */
int main(void)
{
    /***
     * @brief  系统初始化
     */
    uint8_t k;
    VL53L0X_Error Status;                           // 状态
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置中断优先级分组2
    delay_init();                                   // 延时函数初始化
    uart_init(115200);                              // 串口初始化为115200
    Key_GPIO_Config();
    // 定时器 TIM3初始化 TIM4初始化
    TIM3_Config();
    TIM4_Configuration();
    // 外部中断，接收编码器初始化
    EXTI_Configuration();
    // LED 蜂鸣器 按键输入 油门输入初始化
    LED_Init();
    Beep_Init();
    BEEP = 0; // 关闭蜂鸣器
    LED = 1;  // 关闭LED
    // 电机输出PWM初始化
    Init_Pwm();
    Motor_On(0); // 关闭电机
                 // OLED初始化
    OLED_Init();
    OLED_Clear(); // 清屏
    // 显示标题
    OLED_ShowString(50, 0, "W.W");
    OLED_ShowString(10, 2, "WARNING SYSTEM");
    for (k = 0; k < 6; k++)
    {
        OLED_ShowCHinese(20 + 16 * k, 4, 8 + k);
    }
    delay_ms(1000);
    delay_ms(1000);
    OLED_Clear();
    // 初始化VL53L0X
    vl53l0x_init(&vl53l0x_dev);        // 初始化vl53l0x
    vl53l0x_set_mode(&vl53l0x_dev, 0); // 设置默认测量模式

    while (1)
    {
        // 检测踏板状态
        PEDAL_STATE = !PEDAL_IN; // 低电平开关按下，高电平开关松开     // 高电平IN = 1,低电平 IN = 0

        // 检测速度调节
        if (Speed_Down == 0)
            Speed_Value += 20;

        if (Speed_Value > 100)
            Speed_Value = 0;

        // 检测距离
        vl53l0x_general_test(&vl53l0x_dev); // vl53l0x测试
        // key_scan();
        PEDAL_STATE = !PEDAL_IN;
        // 测速
        Average_Speed = (uint32_t)(Speed_1 + Speed_2) / 2;
        // 距离判断

        if (Average_Speed > Set_Speed)
        {
            // 蜂鸣器报警
            if (Distance_data < Set_Distance)
            {
                if (PEDAL_STATE == 1) // 按下油门
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
                    TIM_Cmd(BEEP_TIM, ENABLE); // 开启蜂鸣器
                }
                else
                {
                    WARNING_Flag = 0;
                    BEEP = 0;                   // 关闭蜂鸣器
                    LED = 1;                    // 关闭LED
                    TIM_Cmd(BEEP_TIM, DISABLE); // 关闭蜂鸣器
                }
            }
            else
            {
                WARNING_Flag = 0;
                BEEP = 0;                   // 关闭蜂鸣器
                LED = 1;                    // 关闭LED
                TIM_Cmd(BEEP_TIM, DISABLE); // 关闭蜂鸣器
            }
        }
        else
        {
            WARNING_Flag = 0;
            BEEP = 0;                   // 关闭蜂鸣器
            LED = 1;                    // 关闭LED
            TIM_Cmd(BEEP_TIM, DISABLE); // 关闭蜂鸣器
        }
        // 电机控制，油门踏板判断
        if (PEDAL_STATE == 1)
        {
            Motor_On(Speed_Value);
        }
        else
        {
            Motor_On(0);
        }

        // 显示UI
        OLED_UI();
    }
}

// 在OLED上显示数据，速度，距离和踏板状态
void OLED_UI(void)
{
    // 将速度，距离转化为字符串
    char Speed[10];
    char Distance[10];
    char Level[10];
    uint8_t i;
    /*先清屏*/
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
    // 显示速度
    OLED_ShowCHinese(0, 0, 0);
    OLED_ShowCHinese(16, 0, 1);
    OLED_ShowChar(32, 0, ':');
    OLED_ShowString(40, 0, Speed);

    // 显示距离
    OLED_ShowCHinese(0, 2, 2);
    OLED_ShowCHinese(16, 2, 3);
    OLED_ShowChar(32, 2, ':');
    OLED_ShowString(40, 2, Distance);
    // 显示速度挡位
    OLED_ShowCHinese(0, 4, 4);
    OLED_ShowCHinese(16, 4, 5);
    OLED_ShowChar(32, 4, ':');
    OLED_ShowString(40, 4, Level);
    // 显示踏板状态
    OLED_ShowCHinese(95, 2, 6);      // 油
    OLED_ShowCHinese(95 + 16, 2, 7); // 门
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
        OLED_ShowString(65, 4, "!!!"); // 显示感叹号
    else
        OLED_ShowString(65, 4, "   "); // 不显示感叹号
}

/**
 * @brief OLED位置挡位显示
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
 * @brief  1s的计数函数，用于计算车辆的速度
 * 轮子直径d = 6.5cm 一圈的周长L = pi *d = 20.41cm 一圈的脉冲数N = 20 * 2 = 40
 * 车速 v = 20.41 * (N) / 40 N 为1s内的脉冲数
 */

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        Velocity = (double)(Speed_1 + Speed_2) / (2.0 * 40.0) * (204.1);
        Speed_1 = 0;
        Speed_2 = 0; // 速度清零
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}
/**
 * @brief TIM3中断服务函数 0.1s定时
 * 用于蜂鸣器和LED的控制，间隔响
 *
 */
void TIM3_IRQHandler(void)
{
    // 0.1s中断一次
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        BEEP_Counter++;
        if (BEEP_Counter > BEEP_Time)
        {
            BEEP = !BEEP;
            LED = !LED;
            BEEP_Counter = 0;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // 清除中断标志位
    }
}

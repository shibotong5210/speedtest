#ifndef __VL53L0X_H
#define __VL53L0X_H
/***
 * @file vl53l0x.h
 * @author BT.S (
 * @brief   vl53l0x driver
 * @version 0.1
 * @date 2023-04-23
 */

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_gen.h"
#include "vl53l0x_cali.h"
#include "sys.h"
#include "24cxx.h"
#include "key.h"
#include "delay.h"
#include "led.h"
// VL53L0X�������ϵ�Ĭ��IIC��ַΪ0X52(���������λ)
#define VL53L0X_Addr 0x52

// ����Xshut��ƽ,�Ӷ�ʹ��VL53L0X���� 1:ʹ�� 0:�ر�
#define VL53L0X_Xshut PAout(5)

// ʹ��2.8V IO��ƽģʽ
#define USE_I2C_2V8 1

// ����ģʽ
#define Default_Mode 0  // Ĭ��
#define HIGH_ACCURACY 1 // �߾���
#define LONG_RANGE 2    // ������
#define HIGH_SPEED 3    // ����

// vl53l0xģʽ���ò�����
typedef __packed struct
{
    FixPoint1616_t signalLimit;    // Signal������ֵ
    FixPoint1616_t sigmaLimit;     // Sigmal������ֵ
    uint32_t timingBudget;         // ����ʱ������
    uint8_t preRangeVcselPeriod;   // VCSEL��������
    uint8_t finalRangeVcselPeriod; // VCSEL�������ڷ�Χ

} mode_data;

extern mode_data Mode_data[];
extern uint8_t AjustOK;

VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev); // ��ʼ��vl53l0x
void print_pal_error(VL53L0X_Error Status);     // ������Ϣ��ӡ
void mode_string(u8 mode, char *buf);           // ģʽ�ַ�����ʾ
void vl53l0x_test(void);                        // vl53l0x����
void vl53l0x_reset(VL53L0X_Dev_t *dev);         // vl53l0x��λ

#endif

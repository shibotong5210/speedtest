/**
 * @file vl53l0x_gen.c general
 * @author BT.S (you@domain.com)
 * @brief   vl53l0x general mode 通用设置
 * @version 0.1
 * @date 2023-04-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "vl53l0x_gen.h"

VL53L0X_RangingMeasurementData_t vl53l0x_data; // 测距测量结构体
vu16 Distance_data = 0;                        // 保存测距数据

// VL53L0X 测量模式配置
// dev:设备I2C参数结构体
// mode: 0:默认;1:高精度;2:长距离
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev, u8 mode)
{

    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;

    vl53l0x_reset(dev); // 复位vl53l0x(频繁切换工作模式容易导致采集距离数据不准，需加上这一代码)
    status = VL53L0X_StaticInit(dev);
    AjustOK = 0; // 未校准;
    /*
    if (AjustOK != 0) // 已校准好了,写入校准值---------> 不用校准
    {
        status = VL53L0X_SetReferenceSpads(dev, Vl53l0x_data.refSpadCount, Vl53l0x_data.isApertureSpads); // 设定Spads校准值
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
        status = VL53L0X_SetRefCalibration(dev, Vl53l0x_data.VhvSettings, Vl53l0x_data.PhaseCal); // 设定Ref校准值
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
        status = VL53L0X_SetOffsetCalibrationDataMicroMeter(dev, Vl53l0x_data.OffsetMicroMeter); // 设定偏移校准值
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
        status = VL53L0X_SetXTalkCompensationRateMegaCps(dev, Vl53l0x_data.XTalkCompensationRateMegaCps); // 设定串扰校准值
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
    }
    else
    {
        */
    status = VL53L0X_PerformRefCalibration(dev, &VhvSettings, &PhaseCal); // Ref参考校准
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isApertureSpads); // 执行参考SPAD管理
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    //}
    status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_SINGLE_RANGING); // 使能单次测量模式
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1); // 使能SIGMA范围检查
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1); // 使能信号速率范围检查
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, Mode_data[mode].sigmaLimit); // 设定SIGMA范围
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, Mode_data[mode].signalLimit); // 设定信号速率范围范围
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, Mode_data[mode].timingBudget); // 设定完整测距最长时间
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, Mode_data[mode].preRangeVcselPeriod); // 设定VCSEL脉冲周期
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, Mode_data[mode].finalRangeVcselPeriod); // 设定VCSEL脉冲周期范围

error: // 错误信息
    if (status != VL53L0X_ERROR_NONE)
    {
        print_pal_error(status);
        return status;
    }
    return status;
}

// VL53L0X 单次距离测量函数
// dev:设备I2C参数结构体
// pdata:保存测量数据结构体
VL53L0X_Error vl53l0x_start_single_test(VL53L0X_Dev_t *dev, VL53L0X_RangingMeasurementData_t *pdata)
{

    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint8_t RangeStatus;

    status = VL53L0X_PerformSingleRangingMeasurement(dev, pdata); // 执行单次测距并获取测距测量数据
    if (status != VL53L0X_ERROR_NONE)
        return status;

    Distance_data = pdata->RangeMilliMeter; // 保存最近一次测距测量数据

    return status;
}

// 启动普通测量
// dev：设备I2C参数结构体
// mode模式配置 0:默认;1:高精度;2:长距离
void vl53l0x_general_start(VL53L0X_Dev_t *dev, u8 mode)
{
    VL53L0X_Error Status = VL53L0X_ERROR_NONE; // 工作状态
    u8 i = 0;
    while (vl53l0x_set_mode(dev, mode)) // 配置测量模式
    {
        i++;
        printf("vl53l0x_set_mode error\r\n");
        delay_ms(100);
        if (i == 2)
            return;
    }
    if (Status == VL53L0X_ERROR_NONE)
    {
        Status = vl53l0x_start_single_test(dev, &vl53l0x_data); // 执行一次测量
        printf("d: %4imm\r\n", Distance_data);                  // 打印测量距离
    }
    else
    {
        printf("vl53l0x_start_single_test error\r\n");
    }
    delay_ms(500);
}

// vl53l0x普通测量模式测试
// dev:设备I2C参数结构体
void vl53l0x_general_test(VL53L0X_Dev_t *dev)
{
    u8 mode = 0;
    mode = Default_Mode; // 默认测量模式
    vl53l0x_general_start(dev, mode);
}

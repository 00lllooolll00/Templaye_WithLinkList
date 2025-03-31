#include "MyFilter.h"

/**
 * @brief 滤波器 使连续变化的数据更加平滑
 * 
 * @param dat 输入连续变化的待测数据
 * @param index 滤波系数 0-1 越大滤波力度越大
 * @return uint16_t 返回滤波后的值
 * 
 * @note 如果需要对float类型的数据进行滤波 请重写本函数 如下所示
 */
uint16_t Filter_int(uint16_t dat, float index)
{
    static uint16_t LastData_int = 0;
    uint16_t temp;
    if (!LastData_int) LastData_int = dat;
    temp = LastData_int * index + dat * (1 - index);
    LastData_int = temp;
    return temp;
}

/**
 * @brief 对浮点类型、连续变化的数据滤波
 * 
 * @param dat 输入连续变化的待测数据
 * @param index 滤波系数 0-1 越大滤波力度越大
 * @return float 返回滤波后的值
 */
float Filter_float(float dat, float index)
{
    static float LastData_float = 0;
    float temp;
    if (!LastData_float) LastData_float = dat;
    temp = LastData_float * index + dat * (1 - index);
    LastData_float = temp;
    return temp;
}

/**
 * @file Menu_App.c
 * @author 九千九百九十九
 * @brief 该文件为基于Menu.c的应用层，所有链表节点的建立和函数的声明都在此文件下
 * @version 0.2
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "Menu_App.h"
#include "MyFilter.h"

// 当前菜单指针，指向当前激活的菜单项
menu_node_t *current_menu = NULL;

// 预定义所有菜单节点，使用静态分配以节省内存
xdata menu_node_t menu_temp; // 温度菜单节点
xdata menu_node_t menu_adc_1; // ADC菜单节点1
xdata menu_node_t menu_adc_2; // ADC菜单节点2
xdata menu_node_t menu_clock; // 时钟菜单节点
xdata menu_node_t menu_ultrasonic; // 超声波菜单节点
xdata menu_node_t menu_ne555; //NE555节点

/**
 * @brief 温度菜单显示更新函数
 * 
 * @details 显示当前温度值，包括整数部分和小数部分，并添加菜单标识
 */
void temp_menu_update(void)
{
    float temp_filtered = temperature; // 获取当前温度值，也可以在这里添加滤波处理

    // 显示温度菜单标识
    Seg_Buf[0] = 1; // 温度菜单标识为"0"
    Seg_Buf[1] = 10; // 空白分隔符

    // 显示温度值(例如: 25.6°C)
    Seg_Buf[5] = (uint16_t)(temp_filtered * 10) / 100 % 10; // 温度十位数
    Seg_Buf[6] = (uint16_t)(temp_filtered * 10) / 10 % 10; // 温度个位数
    Seg_Buf[7] = (uint16_t)(temp_filtered * 10) / 1 % 10; // 温度小数点后一位
    Seg_Point[6] = 1; // 在个位数后显示小数点
    // 这里可以添加温度单位"C"的显示，需要自定义字符表支持
}

/**
 * @brief ADC菜单显示更新函数
 * 
 * @details 显示两路AD转换的值，分别是Adval1和Adval2
 */
void adc_menu_update_1(void)
{
    uint16_t Adval1_Show; //滤波后的展示值

    Adval1_Show = Filter_int(Adval1, 0.2f);

    // 显示菜单标识
    Seg_Buf[0] = 2; // ADC菜单标识为"2"
    Seg_Buf[1] = 10; // 空白分隔符

    Seg_Buf[3] = 1; //ADC1 标识

    // 显示ADC值1(通常为电位器)
    Seg_Buf[5] = Adval1_Show / 100 % 10; // 百位
    Seg_Buf[6] = Adval1_Show / 10 % 10; // 十位
    Seg_Buf[7] = Adval1_Show % 10; // 个位
}

void adc_menu_update_2(void)
{
    uint16_t Adval2_Show; //滤波后的展示值

    Adval2_Show = Filter_int(Adval2, 0.2f);

    Seg_Buf[0] = 2; // ADC菜单标识为"2"
    Seg_Buf[1] = 10; // 空白分隔符

    Seg_Buf[3] = 2; //ADC2 标识

    // 显示ADC值2(通常为光敏电阻)
    Seg_Buf[5] = Adval2_Show / 100 % 10; // 十位
    Seg_Buf[6] = Adval2_Show / 10 % 10; // 十位
    Seg_Buf[7] = Adval2_Show / 1 % 10; // 个位
}

/**
 * @brief 时钟菜单显示更新函数
 * 
 * @details 显示当前时间(小时:分钟:秒)，使用小数点模拟冒号
 */
void clock_menu_update(void)
{
    // 显示菜单标识
    Seg_Buf[0] = 3; // 时钟菜单标识为"3"
    Seg_Buf[1] = 10; // 空白分隔符

    // 显示时间 HH:MM:SS
    Seg_Buf[2] = ucRTC[0] / 10; // 小时十位
    Seg_Buf[3] = ucRTC[0] % 10; // 小时个位
    Seg_Point[3] = 11; // 显示小时冒号(使用小数点模拟)

    Seg_Buf[4] = ucRTC[1] / 10; // 分钟十位
    Seg_Buf[5] = ucRTC[1] % 10; // 分钟个位
    Seg_Point[5] = 11; // 显示分钟冒号(使用小数点模拟)

    Seg_Buf[6] = ucRTC[2] / 10; // 秒十位
    Seg_Buf[7] = ucRTC[2] % 10; // 秒个位
}

/**
 * @brief 超声波菜单显示更新函数
 * 
 * @details 显示超声波测距的相关信息
 */
void ultrasonic_menu_update(void)
{
    uint8_t distance_Show;

    distance_Show = (uint8_t)Filter_int(Distance, 0.2); //滤波

    // 显示菜单标识
    // 显示菜单标识
    Seg_Buf[0] = 4; // 时钟菜单标识为"3"
    Seg_Buf[1] = 10; // 空白分隔符

    Seg_Buf[2] = distance_Show / 100 % 10;
    Seg_Buf[3] = distance_Show / 10 % 10;
    Seg_Buf[4] = distance_Show % 10;
    // 注意：显示字母需要在Seg_Table中定义对应的段码
}

/**
 * @brief NE555菜单显示更新函数
 * 
 * @details 显示NE555的相关信息
 */
void ne555_menu_update(void)
{
    //显示数据
    //为避免第一次测量时没数据，先初始化为0，先显示第一次再开始测量
    uint16_t Freq_Show;
    Freq_Show = Filter_int(Freq, 0.2);

    Seg_Buf[0] = 5; // 时钟菜单标识为"3"
    Seg_Buf[1] = 10; // 空白分隔符

    Seg_Buf[2] = Freq_Show / 10000 % 10;
    Seg_Buf[3] = Freq_Show / 1000 % 10;
    Seg_Buf[4] = Freq_Show / 100 % 10;
    Seg_Buf[5] = Freq_Show / 10 % 10;
    Seg_Buf[6] = Freq_Show / 1 % 10;
}

/**
 * @brief 菜单系统初始化
 * 
 * @details 初始化所有菜单节点，建立它们之间的链接关系，设置初始菜单
 *          此函数应在系统启动时调用一次
 * @note 对于链表来说 不同级别的本模板采用的是父子关系，但是对于同级操作，采用的是next prev的关系
 */
void Menu_Init(void)
{
    // 初始化温度菜单
    Menu_Add(&menu_temp, 0x20, temp_menu_update, &menu_ne555, &menu_adc_1, NULL, NULL);

    // 初始化ADC菜单
    Menu_Add(&menu_adc_1, 0x30, adc_menu_update_1, &menu_temp, &menu_clock, &menu_adc_2, NULL);

    Menu_Add(&menu_adc_2, 0x31, adc_menu_update_2, NULL, NULL, &menu_adc_1, &menu_adc_1);

    // 初始化时钟菜单
    Menu_Add(&menu_clock, 0x40, clock_menu_update, &menu_adc_1, &menu_ultrasonic, NULL, NULL);

    // 初始化超声波菜单
    Menu_Add(&menu_ultrasonic, 0x50, ultrasonic_menu_update, &menu_clock, &menu_ne555, NULL, NULL);

    // 初始化ne555菜单
    Menu_Add(&menu_ne555, 0x60, ne555_menu_update, &menu_ultrasonic, &menu_temp, NULL, NULL);

    // 设置初始菜单为主菜单
    current_menu = &menu_temp;

    // 立即更新显示，使菜单内容显示出来
    Menu_Update();
}
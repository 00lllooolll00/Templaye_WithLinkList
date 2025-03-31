/**
 * @file main.c
 * @author 九千九百九十九
 * @brief  基于原来的模板，提供了用链表显示数码管的多级菜单
 * @version 0.2
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */
/*HEADER*/
#include "Sys.h"

/*Global Variable*/
uint32_t xdata Mysystick = 0; //系统心跳
uint8_t pdata Seg_Buf[8]; //数码管显示缓存
uint8_t pdata Seg_Point[8]; //数码管小数点显示缓存
uint8_t pdata ucLed[8]; //LED灯显示缓存
uint8_t pdata ucRTC[3] = {23, 59, 50}; //时间储存 时、分、秒
float temperature; //温度
uint8_t RxData[5]; //串口接收到的的数据
uint16_t Serial_Idle_Cnt; //串口空闲计时器
uint8_t index; //指示当前接收到数据的索引
uint8_t Adval1; //AD转换值
uint8_t Adval2; //AD转换值
uint8_t Distance; //距离
uint16_t Freq = 0; //NE555频率

/*MAIN*/
void main(void)
{
    Periph_Init(); //初始化LED和外设 尽量将该模块放到第一个
    /*Initialize*/
    Timer0_Init(); //初始化TIM0
    // Uart1_Init();//初始化串口1
    Timer2_Init(); //初始化心跳时钟源
    while (Read_temperature() > 80); //消除ds18b20首次转换为85的数据
    memset(Seg_Point, 0, 8); //初始化数码管小数点缓冲
    memset(Seg_Buf, 10, 8); //初始化数码管缓冲区
    memset(ucLed, 0, 8); //初始化LED缓冲区
    RTC_Set(ucRTC); //将需要的时间都写入RTC中
    Menu_Init(); //UI初始化

    /*Task Additon*/
    /*以下为各个模块的实例历程  取消注释即可观察对应模块的现象*/
    Task_Add(&Task_Key, 10);
    Task_Add(&Task_NE555, 1000);
    // Task_Add(&Task_Periph, 500);
    Task_Add(&Task_RTC, 500);
    Task_Add(&Task_Ds18b20, 750);
    Task_Add(&Task_Ul, 200);
    // Task_Add(&Task_Serial, 250);
    Task_Add(&Task_PCF8591, 200);
    Task_Add(&Task_Seg, 50);

    EA = 1; //启动定时器
    /*Loop*/
    while (1)
    {
        Task_Start();
    }
}

/*ISR*/
/*这里使用TIM2定时器作为系统心跳源*/
void Timer2_Isr(void) interrupt 12
{
    Mysystick++;
    Serial_Idle_Cnt++;
    Task_Disp();
}

/*串口中断*/
void Uart1_Isr(void) interrupt 4
{
    if (RI) //检测串口1接收中断
    {
        RI = 0; //清除串口1接收中断请求位

        Serial_Idle_Cnt = 0; //没有接收到数据就会一直自增，接收到数据后会保持0，通过这点可以达到空闲解析的效果

        RxData[index++] = SBUF;
    }
}

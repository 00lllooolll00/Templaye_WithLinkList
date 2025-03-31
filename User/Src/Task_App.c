/**
 * @file Task_App.c
 * @author 九千九百九十九
 * @brief 此为任务的声明和建立，用于优化主函数的视觉，让其更加简介，当然如果你愿意。可以将这些都写在main.c中
 * @version 0.2
 * @date 2025-03-31
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "Task_App.h"

/*Task1 Creation Key*/
void Task_Key(void)
{
    static uint8_t Key_Val;
    static uint8_t Key_Down;
    static uint8_t Key_Up;
    static uint8_t Key_Old = 0; //该值初始给0 避免警告
    static bit Test_Flag = 0;

    Key_Val = Key_Read();
    Key_Down = Key_Val & (Key_Val ^ Key_Old);
    Key_Up = ~Key_Val & (Key_Val ^ Key_Old);
    Key_Old = Key_Val;

    /*Test*/

    switch (Key_Down)
    {
        case 8:
            Test_Flag ^= 1;
            memset(ucLed, Test_Flag, 8);
            break;
        case 4: // 上一级菜单
            Menu_MoveParent();
            break;

        case 5: //下一级菜单
            Menu_MoveChild();
            break;

        case 6: //同一级 下一个菜单
            Menu_MoveNext();
            break;

        case 7: //同一级 上一个菜单
            Menu_MovePrev();
            break;
    }
}

/*Task2 Creation Seg Led*/
void Task_Disp(void)
{
    uint8_t Scan = Mysystick % 8; //显示扫描变量 只在0——7周期变

    Seg_Disp(Scan, Seg_Buf[Scan], Seg_Point[Scan]); //数码管显示
    LED_Disp(Scan, ucLed[Scan]); //LED显示
}

/*Task3 Creation NE555*/
void Task_NE555(void)
{
    TR0 = 0; //关闭TIM0计数
    ET1 = 0; //关闭TIM1中断允许
    Freq = TH0 << 8 | TL0; //获取频率
    TH0 = TL0 = 0; //手动清空计数值
    ET1 = 1; //打开TIM1中断允许
    TR0 = 1; //开启TIM0计数
}

/*Task4 Creation Buzz Motor Relay*/
void Task_Periph(void)
{
    static bit Flag_Buzz = 0;
    static bit Flag_Relay = 1;

    Periph_Set(BUZZ, Flag_Buzz);
    Periph_Set(RELAY, Flag_Relay);

    //500ms翻转一次继电器和蜂鸣器的电平
    Flag_Buzz ^= 1;
    Flag_Relay ^= 1;
}

/*Task5 Creation DS1302*/
void Task_RTC(void)
{
    RTC_Read(ucRTC);
}

/*Task6 Creation DS18B20*/
void Task_Ds18b20(void)
{
    temperature = Read_temperature(); //获取温度
}

/*Task7 Creation Ultrasonic*/
void Task_Ul(void)
{
    Distance = Distance_Get(); //获取距离
}

/*Task8 Creation Serial*/
void Task_Serial(void)
{
    static uint8_t psc = 0; //分频器 每1s发送一次

    /*模拟串口发送数据 这里是1s打印一个数据*/
    if (++psc == 4)
    {
        psc = 0;
        printf("systime:%d\r\n", (uint16_t)(Mysystick / 1000));
    }

    if (Serial_Idle_Cnt > 300) //300ms没接受到数据就会解析一次 这里可以自行调整
    {
        /*串口解析操作*/
        if (RxData[0] == '1')
        {
            static bit i = 0;
            index = 0;
            RxData[0] = 0;
            memset(ucLed, i, 8);
            i ^= 1;
        }
    }
}

/*Task9 Creation PCF8591*/
void Task_PCF8591(void)
{
    Adval1 = AD_Read(0x41); //电位器
    Adval2 = AD_Read(0x43); //光敏
}

void Task_Seg(void)
{
    memset(Seg_Buf, 10, 8); // 清空显示缓冲区
    memset(Seg_Point, 0, 8); // 清除所有小数点
    Menu_Update();
}
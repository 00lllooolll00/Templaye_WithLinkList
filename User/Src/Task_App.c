/**
 * @file Task_App.c
 * @author ��ǧ�Űپ�ʮ��
 * @brief ��Ϊ����������ͽ����������Ż����������Ӿ���������Ӽ�飬��Ȼ�����Ը�⡣���Խ���Щ��д��main.c��
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
    static uint8_t Key_Old = 0; //��ֵ��ʼ��0 ���⾯��
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
        case 4: // ��һ���˵�
            Menu_MoveParent();
            break;

        case 5: //��һ���˵�
            Menu_MoveChild();
            break;

        case 6: //ͬһ�� ��һ���˵�
            Menu_MoveNext();
            break;

        case 7: //ͬһ�� ��һ���˵�
            Menu_MovePrev();
            break;
    }
}

/*Task2 Creation Seg Led*/
void Task_Disp(void)
{
    uint8_t Scan = Mysystick % 8; //��ʾɨ����� ֻ��0����7���ڱ�

    Seg_Disp(Scan, Seg_Buf[Scan], Seg_Point[Scan]); //�������ʾ
    LED_Disp(Scan, ucLed[Scan]); //LED��ʾ
}

/*Task3 Creation NE555*/
void Task_NE555(void)
{
    TR0 = 0; //�ر�TIM0����
    ET1 = 0; //�ر�TIM1�ж�����
    Freq = TH0 << 8 | TL0; //��ȡƵ��
    TH0 = TL0 = 0; //�ֶ���ռ���ֵ
    ET1 = 1; //��TIM1�ж�����
    TR0 = 1; //����TIM0����
}

/*Task4 Creation Buzz Motor Relay*/
void Task_Periph(void)
{
    static bit Flag_Buzz = 0;
    static bit Flag_Relay = 1;

    Periph_Set(BUZZ, Flag_Buzz);
    Periph_Set(RELAY, Flag_Relay);

    //500ms��תһ�μ̵����ͷ������ĵ�ƽ
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
    temperature = Read_temperature(); //��ȡ�¶�
}

/*Task7 Creation Ultrasonic*/
void Task_Ul(void)
{
    Distance = Distance_Get(); //��ȡ����
}

/*Task8 Creation Serial*/
void Task_Serial(void)
{
    static uint8_t psc = 0; //��Ƶ�� ÿ1s����һ��

    /*ģ�⴮�ڷ������� ������1s��ӡһ������*/
    if (++psc == 4)
    {
        psc = 0;
        printf("systime:%d\r\n", (uint16_t)(Mysystick / 1000));
    }

    if (Serial_Idle_Cnt > 300) //300msû���ܵ����ݾͻ����һ�� ����������е���
    {
        /*���ڽ�������*/
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
    Adval1 = AD_Read(0x41); //��λ��
    Adval2 = AD_Read(0x43); //����
}

void Task_Seg(void)
{
    memset(Seg_Buf, 10, 8); // �����ʾ������
    memset(Seg_Point, 0, 8); // �������С����
    Menu_Update();
}
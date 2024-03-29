//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CINPUTDEVICE_H
#define CINPUTDEVICE_H

//-----------------------------------------------------------------------------------------------------
#include <stdint.h>

#include "Timer.h"

//-----------------------------------------------------------------------------------------------------
enum
{
    KEY_ONE = 1,
    KEY_TWO,
    KEY_THREE
};

// ������� ����������� �������.
enum
{
    KEY_EVENT_UNPRESSED = 0,
    KEY_EVENT_WAITING_PRESSED,
    KEY_EVENT_PRESSED_PUSH,
    KEY_EVENT_PRESSED_HOLD,
    KEY_EVENT_PRESSED_REPEAT
};

//-----------------------------------------------------------------------------------------------------
class CInputDevice
{
public:
    CInputDevice();
    CInputDevice(uint8_t (* )(void));
    virtual ~CInputDevice();
    void SetCapabilities(uint8_t (* )(void));
    uint8_t Report(uint8_t , uint8_t );
    uint8_t Report(uint8_t );
    uint8_t EventHappened(void);
    uint8_t CInputDevice::KeyEventHappened(uint8_t );
    void KeyScan(void);

    // ����� ��������� ������ � ������� ���������, ����������� ������� ���������.
    static const uint8_t KEY_PRESSED_TIME_PUSH = 100;
    static const uint8_t KEY_PRESSED_TIME_HOLD = 2000;
    // ����� ���������� ��� ������� ������.
    static const uint8_t KEY_PRESSED_HOLD_REPEAT_TIME = 50;

protected:
private:
    // ��������� �� ������� - �������� �������.
    uint8_t (*m_fpuiEventSource)(void);
    uint8_t m_uiEventCode;
    uint8_t m_uiFlowControl;
    CTimer m_xTimer;
};

#endif // CINPUTDEVICE_H

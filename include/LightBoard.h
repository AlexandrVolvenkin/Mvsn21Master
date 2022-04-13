//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CLIGHTBOARD_H
#define CLIGHTBOARD_H

#include <stdint.h>
#include "DM134.h"

//-----------------------------------------------------------------------------------------------------
class CLightBoard
{
public:
    typedef CDM134 CLedDriver;

    CLightBoard();
    virtual ~CLightBoard();
    void Init(void);
    void Set(void);
//    void Set(uint8_t __farflash * , uint8_t );
//    void Set(CLedDriver* , uint8_t __farflash * , uint8_t );

    static const uint8_t m_uiLedDriversNumber = 4;
    static const uint8_t m_uiBoardLedNumber = CLedDriver::m_uiLedDriverLedNumber * m_uiLedDriversNumber;

protected:
private:
    CLedDriver m_xLedDriver;
    uint8_t m_auiLedState[];
    static __flash uint8_t m_auiLedMap[];
};

#endif // CLIGHTBOARD_H

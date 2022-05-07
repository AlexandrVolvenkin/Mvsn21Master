//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
#include "Platform.h"
//#include "Config.h"
#include "Timer.h"
#include "Mvsn21.h"
#include "Modbus.h"
#include "ModbusRTU.h"
//#include "InputDevice.h"




////-----------------------------------------------------------------------------------------------------
//void MainCycle(void)
//{
//    if (xLedTimer.IsOverflow())
//    {
//        xLedTimer.Reset();
//    }
//}

//-----------------------------------------------------------------------------------------------------
void MainReboot()
{

}

//-----------------------------------------------------------------------------------------------------
int main()
{

    enum
    {
        MAIN_START,
        MAIN_FIRST_STEP,
        MAIN_SECOND_STEP,
        MAIN_THIRD_STEP,
        MAIN_FORTH_STEP,
        MAIN_THIRD_REBOOT,
        MAIN_STOP
    };

//    CPlatform::WatchdogEnable();
    CPlatform::Init();
    CPlatform::InterruptEnable();
//    CTimer xStatusLedTimer(500);
//    CTimer xLedTimer(500);
//    CTimer xPeripheryTimer(10);
    CUart::UartBind(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, 0, 0, 0, 0);
//    CPlatform::m_pxUart0 = &xUart0;

//    CModbusRTU xModbusRtuOne;
    CModbusRTU::Init(CPlatform::m_pxUart0,
                       MODBUS_RTU_BAUD_RATE,
                       'N',
                       8,
                       1,
                       CMvsn21::m_aui8ReceiveMessageBuff,
                       CMvsn21::m_aui8TransmitMessageBuff,
                       CMvsn21::m_aucRtuCoilsArray,
                       CMvsn21::m_aucRtuDiscreteInputsArray,
                       CMvsn21::m_aucRtuHoldingRegistersArray,
                       CMvsn21::m_aucRtuInputRegistersArray,
                       COILS_WORK_ARRAY_LENGTH,
                       DISCRETE_INPUTS_ARRAY_LENGTH,
                       HOLDING_REGISTERS_ARRAY_LENGTH,
                       INPUT_REGISTERS_ARRAY_LENGTH);
    CModbusRTU::SlaveSet(1);
    CModbusRTU::ReceiveEnable();
    CModbusRTU::FlowControlSet(CModbus::START_REQUEST);

    CSpi::Init(CSpi::m_auiSpiRxBuffer, CSpi::m_auiSpiTxBuffer);
    CMvsn21::SpiBusExchangeEnable();

    CAdc::Init();
    CMvsn21::MeasureFlowControlSet(CMvsn21::FSM_IDDLE);

    uint8_t uiMainFsmState = MAIN_START;
    uint8_t uiStatusLedState = 0;;
    uint8_t uiLedState = 0;
    uint8_t uiWatchDogStepID = 0;

    while(1)
    {
        CMvsn21::SpiFsm();
        CModbusRTU::Execution();
        CMvsn21::MeasureFsm();
//
//        if (xPeripheryTimer.IsOverflow())
//        {
//            xPeripheryTimer.Reset();
//        }

//        if (CSpi::DataExchangeIsOccur())
//        {
//            CSpi::DataExchangeIsOccurClear();
//        }

//        switch (uiMainFsmState)
//        {
//        case MAIN_START:
//            uiMainFsmState = MAIN_FIRST_STEP;
//            break;
//
//        case MAIN_FIRST_STEP:
//////            MainCycle();
////            if (xStatusLedTimer.IsOverflow())
////            {
////                xStatusLedTimer.Reset();
////                if (uiStatusLedState)
////                {
////        CPlatform::TxLedOff();
//////                    CPlatform::StatusLedOff();
////                    uiStatusLedState = 0;
////                }
////                else
////                {
////        CPlatform::TxLedOn();
//////                    CPlatform::StatusLedOn();
////                    uiStatusLedState = 1;
////                }
////                uiWatchDogStepID |= 0x03;
////            }
//
////            if (xLedTimer.IsOverflow())
////            {
////                xLedTimer.Reset();
////                if (uiLedState)
////                {
////                    CPlatform::TxLedOff();
////                    uiLedState = 0;
////                }
////                else
////                {
////                    CPlatform::TxLedOn();
////                    uiLedState = 1;
////                }
////            }
//
//        case MAIN_SECOND_STEP:
//            break;
//
//        case MAIN_THIRD_REBOOT:
//            MainReboot();
//            break;
//
//        case MAIN_STOP:
//            break;
//
//        default:
//            break;
//        }

//        // ��� ������ ��������� ��������?
//        if (uiWatchDogStepID == 0x03)
//        {
//            uiWatchDogStepID = 0;
        CPlatform::WatchdogReset();
//        }


    }

    return 0;
}

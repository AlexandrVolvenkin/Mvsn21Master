//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------

#include "Platform.h"
#include "Timer.h"
#include "Mvsn21.h"
#include "Modbus.h"
#include "ModbusRTU.h"

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

    CPlatform::WatchdogEnable();
    CPlatform::Init();
    CPlatform::InterruptEnable();
    CUart::UartBind(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0, 0, 0, 0, 0);
    CModbusRTU::Init(0,//CPlatform::m_pxUart0,
                     MODBUS_RTU_BAUD_RATE,
                     'N',
                     8,
                     1,
                     CMvsn21::m_aui8ReceiveMessageBuff,
                     CMvsn21::m_aui8TransmitMessageBuff,
                     0,//CMvsn21::m_aucRtuCoilsArray,
                     CMvsn21::m_aucRtuDiscreteInputsArray,
                     0,//CMvsn21::m_aucRtuHoldingRegistersArray,
                     0,//CMvsn21::m_aucRtuInputRegistersArray,
                     COILS_WORK_ARRAY_LENGTH,
                     DISCRETE_INPUTS_ARRAY_LENGTH,
                     HOLDING_REGISTERS_ARRAY_LENGTH,
                     INPUT_REGISTERS_ARRAY_LENGTH);
    CModbusRTU::SlaveSet(1);
    CModbusRTU::FlowControlSet(CModbus::IDDLE);

    CSpi::Init(CSpi::m_auiSpiRxBuffer, CSpi::m_auiSpiTxBuffer);
    CMvsn21::SpiBusExchangeEnable();
    // ��� ����,����� ���������� ������� ��������� ������ ����� ����������
    // ���������. ��� �������� ��������� ����� �� ������ ������.
    // ����� �� ���������� � ������ ������������� ���������� ������� ���������
    // ������ � ������ ������������� ������ ����� �������� � ���, ��� ��� �� ������.
    CSpi::m_uiErrorCode = CMvsn21::COMMAND_DATA_NOT_READY;

    CAdc::Init();
    memset((reinterpret_cast<uint8_t*>(CMvsn21::m_aucRtuDiscreteInputsArray)), 0, sizeof(CMvsn21::m_aucRtuDiscreteInputsArray));
    memset((reinterpret_cast<uint8_t*>(CMvsn21::m_axSameStateCheck)), 0, sizeof(CMvsn21::m_axSameStateCheck));
    CMvsn21::MeasureFlowControlSet(CMvsn21::FSM_START);

    uint8_t uiMainFsmState = MAIN_START;
    uint8_t uiStatusLedState = 0;;
    uint8_t uiLedState = 0;
    uint8_t uiWatchDogStepID = 0;


    CMvsn21::m_uiStartSkippedCyclesNumber = 0;

    while(1)
    {
        CMvsn21::SpiFsm();
        CModbusRTU::Execution();
        CMvsn21::MeasureFsm();
    }

    return 0;
}

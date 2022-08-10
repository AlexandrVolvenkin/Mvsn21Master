//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------

#include "Mvsn21.h"
#include "Platform.h"
#include "Crc.h"
#include "Modbus.h"

uint8_t CMvsn21::m_uiType;
//CDriver* CMvsn21::m_pxDriver;
uint8_t CMvsn21::m_uiFlowControl;
uint16_t CMvsn21::m_uiMessageLength;
uint8_t CMvsn21::m_uiChannel;
uint8_t CMvsn21::m_uiMeasureFlowControl;
// ������������� ����� ������.
__flash TChannelRemap CMvsn21::m_axMeasurementChannelRemap[] =
{
    // ��������� ������ ������ ����.
    // ������ X1.
    {0, 6},
    {0, 7},
    {0, 0},
    {0, 1},
    {0, 2},
    {0, 3},
    // ������ X2.
    {0, 4},
    {0, 5},
    {1, 6},
    {1, 7},
    {1, 0},
    {1, 1},
    // ������ X3.
    {1, 2},
    {1, 3},
    {1, 4},
    {1, 5},
    {2, 6},
    {2, 7},
    // ������ X4.
    {2, 0},
    {2, 1},
    {2, 2},
    {2, 3},
    {2, 4},
    {2, 5},
};

TSameStateCheck CMvsn21::m_axSameStateCheck[];
//uint8_t CMvsn21::m_aucRtuCoilsArray[];
uint8_t CMvsn21::m_aucRtuDiscreteInputsArray[];
//uint16_t CMvsn21::m_aucRtuHoldingRegistersArray[];
//uint16_t CMvsn21::m_aucRtuInputRegistersArray[];
uint8_t CMvsn21::m_aui8ReceiveMessageBuff[];
uint8_t CMvsn21::m_aui8TransmitMessageBuff[];
//uint8_t CMvsn21::m_auiSpiRxBuffer[];
//uint8_t CMvsn21::m_auiSpiTxBuffer[];

////-----------------------------------------------------------------------------------------------------
//CMvsn21::CMvsn21(uint8_t uiType, CDriver* pxDriver)// :
////    m_uiType(uiType),
////    m_pxDriver(pxDriver)
//{
//
//}

////-----------------------------------------------------------------------------------------------------
//CMvsn21::CMvsn21(uint8_t uiType)// :
////    m_uiType(uiType),
////    m_pxDriver(pxDriver)
//{
//
//}
//
////-----------------------------------------------------------------------------------------------------
//CMvsn21::~CMvsn21()
//{
//    //dtor
//}

//////-----------------------------------------------------------------------------------------------------
////void CMvsn21::SlaveSelectInterruptEnable(void)
////{
////
////}
////
//////-----------------------------------------------------------------------------------------------------
////void CMvsn21::SlaveSelectInterruptDisable(void)
////{
////
////}
//
//-----------------------------------------------------------------------------------------------------
void CMvsn21::SpiBusExchangeEnable(void)
{
    CPlatform::Int0InterruptEnable();
    m_uiFlowControl = FSM_START;
}

////-----------------------------------------------------------------------------------------------------
//void CMvsn21::SpiBusExchangeDisable(void)
//{
//    CPlatform::Int0InterruptDisable();
//}

////-----------------------------------------------------------------------------------------------------
//void CMvsn21::Reset(void)
//{
//    CSpi::Reset();
//}

////-----------------------------------------------------------------------------------------------------
//uint8_t CMvsn21::Select(void)
//{
//    return CSpi::ByteIsReceived();
//}

////-----------------------------------------------------------------------------------------------------
//int16_t CMvsn21::Exchange(void)
//{
//    return CSpi::Exchange();
//}

////-----------------------------------------------------------------------------------------------------------------
//uint8_t CMvsn21::FrameIsReceived(void)
//{
//    return CSpi::m_bfDataExchangeIsOccur;
//};
//
////-----------------------------------------------------------------------------------------------------------------
//uint16_t CMvsn21::GetFrameLength(void)
//{
//    return CSpi::GetFrameLength();
//}

////-----------------------------------------------------------------------------------------------------------------
//int8_t CMvsn21::FrameCheck(void)
//{
////    if (CUart::m_nuiRxBuffByteCounter < _MIN_MESSAGE_LENGTH)
////    {
////        return 0;
////    }
////
////    uint16_t uiCrc = ((static_cast<uint16_t>(m_puiRxBuffer[CUart::m_nuiRxBuffByteCounter - 2]) << 8) |
////                      (static_cast<uint16_t>(m_puiRxBuffer[CUart::m_nuiRxBuffByteCounter - 1])));
////
////    if (usCrc16(m_puiRxBuffer,
////                (CUart::m_nuiRxBuffByteCounter - _MVSN21_RTU_CHECKSUM_LENGTH)) == uiCrc)
////    {
////        return 1;
////    }
////    else
////    {
////        return 0;
////    }
//}
//
////-----------------------------------------------------------------------------------------------------------------
//int8_t CMvsn21::FrameCheck(uint8_t *puiSource, uint16_t uiLength)
//{
////    if (uiLength < _MIN_MESSAGE_LENGTH)
////    {
////        return 0;
////    }
////
////    uint16_t uiCrc = ((static_cast<uint16_t>(puiSource[uiLength - 2]) << 8) |
////                      (static_cast<uint16_t>(puiSource[uiLength - 1])));
////
////    if (usCrc16(puiSource,
////                (uiLength - _MVSN21_RTU_CHECKSUM_LENGTH)) == uiCrc)
////    {
////        return 1;
////    }
////    else
////    {
////        return 0;
////    }
//}

//-----------------------------------------------------------------------------------------------------------------
int16_t CMvsn21::ReportType(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    uiLength = 0;
    puiResponse[uiLength++] = 0;
    puiResponse[uiLength++] = MODULE_TYPE_MVSN21;
    puiResponse[uiLength++] = (COMMAND_REPORT_TYPE + MODULE_TYPE_MVSN21);

    return uiLength;
}

//-----------------------------------------------------------------------------------------------------------------
int16_t CMvsn21::ReadData(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiLength)
{
    uiLength = 0;
    // �������� ������ ������� �� ���� Spi.
    // ������ ���� - ������.
    puiResponse[uiLength++] = 0;

    // ��������� ����� ���� - ������ ��������� ���������� ������.
    // ������� ����������� ������ ��������� ������������� ������� ���� ����� � ���������� �����.
    // i - ������ ���������� ������.
    for (int8_t i = 0; i < DISCRETE_INPUT_NUMBER; )
    {
        uint8_t uiState = 0;
        // ������� ����������� ������ ��������� ������ ������������� ������� � ���� �� ������ ���������� �����.
        for (int8_t j= 0;
                j < CHANNELS_IN_BYTE;
                j += 1)
        {
            // ��������� ��� ���� � �����.
            // ��������� ��� ������ ��������� ������������� �������?
            if (i < DISCRETE_INPUT_NUMBER)
            {
                // ������� ����� �� ������� ����, ����� ������������� ������������� ������� �� ������� ������.
                uint8_t uiInputIndex = ((m_axMeasurementChannelRemap[i].uiChip *
                                         (MEASURE_CHANNEL_NUMBER * MEASURE_CHANNEL_STATE_BIT_NUMBER)) +
                                        // ������� ����� �� ������� ����� ������, ����� ������������� ������������� ������� �� ������� ������.
                                        (m_axMeasurementChannelRemap[i].uiChannel * MEASURE_CHANNEL_STATE_BIT_NUMBER));

                for (int8_t k = 0;
                        k < MEASURE_CHANNEL_STATE_BIT_NUMBER;
                        k++)
                {
                    uiState >>= 1;
                    // ������ � ����������� ������������� ������� �����.
                    // ��������� �������������� ������ �������� ��� ����.
                    if (m_aucRtuDiscreteInputsArray[uiInputIndex + k])
                    {
                        uiState |= 0x80;
                    }
                }
                // ��������� ���������� ����.
                i++;
            }
            else
            {
                // ��������� ��� ������ ��������� ������������� �������.
                goto EndStateDataPack;
            }
        }
        puiResponse[uiLength] = uiState;
        // ��������� ����.
        uiLength++;
    }

EndStateDataPack:

    // ��������� ���� - ����� ���� ���� ������� � ��������(������ ���� � "���" �� �����������).
    puiResponse[uiLength] =
        (usCrcSummOneByteCalculation(&puiResponse[0], (uiLength)) +
         // ��� �������.
         puiRequest[0]);

    return uiLength;
}

//-----------------------------------------------------------------------------------------------------------------
int16_t CMvsn21::SpiReply(uint8_t *puiRequest, uint8_t *puiResponse, uint16_t uiFrameLength)
{
    uint16_t uiLength = uiFrameLength;
    // ����� �������� ������?
    switch (puiRequest[0])
    {
    case COMMAND_REPORT_TYPE:
        uiLength = ReportType(puiRequest, puiResponse, uiLength);
        break;

    case COMMAND_READ_DATA:
        uiLength = ReadData(puiRequest, puiResponse, uiLength);
        break;

//    case COMMAND_READ_DATA_BASE:
////        uiLength = ReadHoldingRegisters(puiRequest, puiResponse, uiLength);
//        break;
//
//    case COMMAND_WRITE_DATA_BASE:
////        uiLength = ReadInputRegisters(puiRequest, puiResponse, uiLength);
//        break;

    default:
        break;
    }
    return uiLength;
}

//-----------------------------------------------------------------------------------------------------------------
void CMvsn21::SpiFsm(void)
{
    int16_t iReceivedCounter;

    switch (m_uiFlowControl)
    {
    case FSM_IDDLE:
        break;

    case FSM_START:
//        Reset();
        CSpi::Reset();
        m_uiMessageLength = 0;
        m_uiFlowControl = FSM_WAITING_MESSAGE;
        break;

    case FSM_WAITING_MESSAGE:
        if (CSpi::ByteIsReceived())
//        if (Select())
        {
//            iReceivedCounter = Exchange();
            iReceivedCounter = CSpi::Exchange();

            if (iReceivedCounter > 0)
            {
                m_uiMessageLength += iReceivedCounter;

                if (SpiReply(CSpi::m_puiRxBuffer, CSpi::m_puiTxBuffer, m_uiMessageLength))
                {
                    m_uiFlowControl = FSM_DATA_EXCHANGE_END_WAITING;
                }
                else
                {
                    m_uiFlowControl = FSM_IDDLE;
                }
            }
            else if (iReceivedCounter == -1)
            {
                m_uiFlowControl = FSM_IDDLE;
            }
        }

        break;

    case FSM_DATA_EXCHANGE_END_WAITING:
        break;

    case FSM_DATA_EXCHANGE_END:
        if (CModbus::m_uiSlaveAddress >= 2)
        {
            CModbus::m_uiSlaveAddress = 1;
        }
        else
        {
            CModbus::m_uiSlaveAddress += 1;
        }
        CModbus::ReadDiscreteInputsRequest((CModbus::m_uiSlaveAddress),
                                           0,
                                           (MEASURE_CHANNEL_NUMBER * MEASURE_CHANNEL_STATE_BIT_NUMBER));
        m_uiFlowControl = FSM_IDDLE;
        break;

    default:
        break;
    }

}

//-----------------------------------------------------------------------------------------------------
void CMvsn21::MeasureFsm(void)
{
    switch (m_uiMeasureFlowControl)
    {
    case FSM_IDDLE:
        CPlatform::WatchdogReset();
        break;

    case FSM_START:
        m_uiChannel = 0;
        CAdc::Enable();
        CAdc::ChannelSelect(m_uiChannel);
        CAdc::Start();
        m_uiMeasureFlowControl = FSM_CONTINUOUS_MEASURE;
        break;

    case FSM_CONTINUOUS_MEASURE:
        if (CAdc::MeasureIsComlete())
        {
            uint8_t uiState =
                CMeasurementChannel::StateCheck(CAdc::GetMeasureValue());

            // ���������� ��������� ����������� ����� ��������� � �������?
            if (m_axSameStateCheck[m_uiChannel].uiState == uiState)
            {
                // C�������� ����������� ����� ���������� �� � ���������� ������?
                if (m_axSameStateCheck[m_uiChannel].uiSameStateCounter >= SAME_STATE_CHECK_NUMBER)
                {
                    m_aucRtuDiscreteInputsArray[(m_uiChannel * 2) + (MEASURE_CHANNEL_NUMBER * MEASURE_CHANNEL_STATE_BIT_NUMBER * 2)] = (m_axSameStateCheck[m_uiChannel].uiState & 0x01);
                    m_aucRtuDiscreteInputsArray[(m_uiChannel * 2) + (MEASURE_CHANNEL_NUMBER * MEASURE_CHANNEL_STATE_BIT_NUMBER * 2) + 1] = ((m_axSameStateCheck[m_uiChannel].uiState >> 1) & 0x01);
                }
                else
                {
                    m_axSameStateCheck[m_uiChannel].uiSameStateCounter += 1;
                }
            }
            else
            {
                // ���������� ��������� ����������� ����� �� ��������� � �������.
                // �������� ����� ���������.
                m_axSameStateCheck[m_uiChannel].uiState = uiState;
                m_axSameStateCheck[m_uiChannel].uiSameStateCounter = 0;
            }

            m_uiChannel++;
            if (m_uiChannel < MEASURE_CHANNEL_NUMBER)
            {
                CAdc::Stop();
                CAdc::ChannelSelect(m_uiChannel);
                CAdc::Start();
            }
            else
            {
                CAdc::Disable();
                m_uiMeasureFlowControl = FSM_IDDLE;
            }
        }
        break;

    default:
        break;
    }
}







////-----------------------------------------------------------------------------------------------------
//CMvsn21Driver::CMvsn21Driver(uint8_t uiType) :
//
//    m_uiType(uiType),
//{
//
//}
//
////-----------------------------------------------------------------------------------------------------
//CMvsn21Driver::~CMvsn21Driver()
//{
//
//}
//
////-----------------------------------------------------------------------------------------------------
//uint16_t CMvsn21Driver::TypeReport(uint8_t *puiDestination, uint16_t uiLength)
//{
//
//}

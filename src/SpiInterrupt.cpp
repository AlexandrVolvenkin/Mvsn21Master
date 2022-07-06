
#include "Platform.h"

//-----------------------------------------------------------------------------------------------------
#pragma vector = SPI_STC_vect
__interrupt void SIG_SPI_STC(void)
{
    SPDR = CSpi::m_uiExchangeByte;
    CSpi::m_puiRxBuffer[CSpi::m_nuiBuffByteCounter] = SPDR;
    CSpi::m_uiExchangeByte = CSpi::m_puiTxBuffer[CSpi::m_nuiBuffByteCounter];
//    CSpi::m_uiExchangeByte = CSpi::m_puiTxBuffer[CSpi::m_nuiBuffByteCounter + 1];

    // ����� ����� �� ����������?
    if (CSpi::m_nuiBuffByteCounter <
            CSpi::BUFFER_LENGTH)
    {
        CSpi::m_nuiBuffByteCounter++;
        CSpi::m_bfByteIsReceived = 1;
    }
    else
    {
        // �� �������������� m_nuiBuffByteCounter, ����� �� ����� �� ������� ������.
        // ��������� ���� - ��������� ����� ������� �� SPI.
        CSpi::m_bfRxBuffOverflow = 1;
        CSpi::m_bfByteIsReceived = 1;
    }
}

#include "Platform.h"
#include "Mvsn21.h"

//-----------------------------------------------------------------------------------------------------
#pragma vector = SPI_STC_vect
__interrupt void SIG_SPI_STC(void)
{
    if (CSpi::m_nuiBuffByteCounter)
    {
        SPDR = CSpi::m_puiTxBuffer[CSpi::m_nuiBuffByteCounter];
        CSpi::m_puiRxBuffer[CSpi::m_nuiBuffByteCounter] = SPDR;
    }
    else
    {
        if (SPDR == CMvsn21::COMMAND_REPORT_TYPE)
        {
            SPDR = SPDR;
            CSpi::m_puiRxBuffer[CSpi::m_nuiBuffByteCounter] = SPDR;
        }
        else
        {
            if (CSpi::m_uiErrorCode)
            {
                SPDR = CSpi::m_uiErrorCode;
                CSpi::m_puiRxBuffer[CSpi::m_nuiBuffByteCounter] = SPDR;
            }
            else
            {
                SPDR = SPDR;
                CSpi::m_puiRxBuffer[CSpi::m_nuiBuffByteCounter] = SPDR;
            }
        }
    }

    // буфер приёма не переполнен?
    if (CSpi::m_nuiBuffByteCounter <
            CSpi::BUFFER_LENGTH)
    {
        CSpi::m_nuiBuffByteCounter++;
        CSpi::m_bfByteIsReceived = 1;
    }
    else
    {
        // не инкрементируем m_nuiBuffByteCounter, чтобы не выйти за границы буфера.
        // установим флаг - произошел обмен данными по SPI.
        CSpi::m_bfRxBuffOverflow = 1;
        CSpi::m_bfByteIsReceived = 1;
    }
}

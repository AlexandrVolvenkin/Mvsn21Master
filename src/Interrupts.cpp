//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#include "Platform.h"


//-----------------------------------------------------------------------------------------------------
// Прерывание по завершению приема UART
//#pragma vector = USART_RX_vect
__interrupt void CUart::__URXComplete(void)
//__interrupt void SIG_UART0_RECV(void)
{
//    CUart::RecvInterruptHandler();
    // промежуточный буфер приёма переполнен?
    if (CUart::m_nuiRxBuffByteCounter >=
            CUart::UART_MAX_BUFF_LENGTH)
    {
        CUart::m_bfRxBuffOverflow = 1;
        // не инкрементируем pucUsartRxBuff, чтобы не выйти за границы буфера.
        CUart::m_bfByteIsReceived = 1;
    }
    else
    {
        CUart::m_auiIntermediateBuff[CUart::m_nuiRxBuffByteCounter++] = *CUart::m_UDR;
        CUart::m_bfByteIsReceived = 1;
    }
}

//-----------------------------------------------------------------------------------------------------
// Прерывание по пустому регистру данных UDR
//#pragma vector = USART_UDRE_vect
__interrupt void CUart::__UDREmpty(void)
//__interrupt void SIG_UART0_DATA(void)
{
//    CUart::UdreInterruptHandler();
    if (CUart::m_nuiTxBuffByteCounter == 0)
    {
        *m_UCSRA &= ~(1 << UDRE0);
        *CUart::m_UCSRB &= ~((1 << UDRIE0));
//        *CUart::m_UCSRA |= (1 << TXC0);
        *CUart::m_UCSRB |= (1 << TXCIE0);
        return;
    }
    *CUart::m_UDR = *CUart::m_puiTxBuffer++;
    CUart::m_nuiTxBuffByteCounter--;
}

//-----------------------------------------------------------------------------------------------------
// Прерывание по завершению передачи UART
//#pragma vector = USART_TX_vect
//__interrupt void SIG_UART0_TXC(void)
__interrupt void CUart::__UTXComplete(void)
{
//    CUart::TxcInterruptHandler();
////    *CUart::m_UCSRB &= ~((1 << TXEN0) | (1 << TXCIE0));
//
////    if (m_rs485ddr)
////    {
////        Rs485RtsOff();
////    }
//    CMvsn21::MeasureFlowControlSet(CMvsn21::FSM_START);
////    *m_UCSRA |= (1 << RXC0);
////    *m_UCSRB |= (1 << RXEN0) | (1 << RXCIE0);
    CUart::m_bfFrameIsSended = 1;
}

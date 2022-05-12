//-----------------------------------------------------------------------------------------------------
//  Source      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef PLATFORM_H
#define PLATFORM_H

// ATMega48
//-----------------------------------------------------------------------------------------------------
#include <intrinsics.h>
#include <stdint.h>
#include <pgmspace.h>
#include <ioavr.h>
//#include  <iom128a.h>
#include <inavr.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <yfuns.h>

//-----------------------------------------------------------------------------------------------------
//#define F_CPU 14745600UL
#define F_CPU 7372800UL
// �������� �������� �������?


//-----------------------------------------------------------------------------------------------------
// delay
////  struct time.h
//struct tm {
//   int tm_sec;         /* seconds,  range 0 to 59          */
//   int tm_min;         /* minutes, range 0 to 59           */
//   int tm_hour;        /* hours, range 0 to 23             */
//   int tm_mday;        /* day of the month, range 1 to 31  */
//   int tm_mon;         /* month, range 0 to 11             */
//   int tm_year;        /* The number of years since 1900   */
//   int tm_wday;        /* day of the week, range 0 to 6    */
//   int tm_yday;        /* day in the year, range 0 to 365  */
//   int tm_isdst;       /* daylight saving time             */
//};
//typedef struct tm tm;

///*
// * Structure returned by gettimeofday(2) system call,
// * and used in other calls.
// */
//struct timeval
//{
//    int16_t	tv_sec;		/* seconds */
//    int16_t	tv_usec;	/* and microseconds */
//};
//
//struct timezone
//{
//    int16_t tz_minuteswest;     /* minutes west of Greenwich */
//    int16_t tz_dsttime;         /* type of DST correction */
//};

#define delay_us(usecs) __delay_cycles((F_CPU/1000000)  *(usecs))

#define _delay_us delay_us
#define _delay_ms delay_ms;
#define usleep delay_us

//void delay_ms(uint16_t millisecs);
//void delay_s(uint16_t secs);
//void delay_mins(uint16_t minutes);
//int16_t gettimeofday(struct timeval *tv, struct timezone *tz);

////-----------------------------------------------------------------------------------------------------
///* Access macros for `fd_set'.  */
//#define        FD_SET(fd, fdsetp)        _NOP()
//#define        FD_CLR(fd, fdsetp)        _NOP()
//#define        FD_ISSET(fd, fdsetp)      _NOP()
//#define        FD_ZERO(fdsetp)           _NOP()
//
//typedef struct fd_set
//{
//    uint16_t  fd_count;
////  SOCKET fd_array[FD_SETSIZE];
//} fd_set, FD_SET, *PFD_SET, *LPFD_SET;

//int16_t select(int32_t n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

//-----------------------------------------------------------------------------------------------------
// GPIO
#define   SetPinOut(reg, bit)          reg |= (1<<bit)
#define   SetPinIn(reg, bit)          reg &= (~(1<<bit))
#define   SetBit(reg, bit)          reg |= (1<<bit)
#define   ClearBit(reg, bit)       reg &= (~(1<<bit))
#define   InvBit(reg, bit)          reg ^= (1<<bit)
#define   BitIsSet(reg, bit)       ((reg & (1<<bit)) != 0)
#define   BitIsClear(reg, bit)    ((reg & (1<<bit)) == 0)

#define  SET_BIT(reg, bit)  SetBit(reg, bit)
#define  CLEAR_BIT(reg, bit)  ClearBit(reg, bit)
#define  SET_GPIO_PIN_OUTPUT(reg, bit)  SetPinOut(reg, bit)
#define  SET_GPIO_PIN_INPUT(reg, bit)  SetPinIn(reg, bit)
#define  SET_GPIO_PIN(reg, bit)  SetBit(reg, bit)
#define  CLEAR_GPIO_PIN(reg, bit)  ClearBit(reg, bit)
#define  BIT_IS_SET(reg, bit)       ((reg & (1<<bit)) != 0)
#define  BIT_IS_CLEAR(reg, bit)    ((reg & (1<<bit)) == 0)

#define	OUT_BYTE(addr, data)	addr = (data)
#define	IN_BYTE(addr)			(addr)

#define CheckBit(Address,Bit) (Address & (1<<Bit))
#define   Bit(bit)          (1<<bit)
#define   _Bit(bit)       (~(1<<bit))
#define   BIT(bit)          (1<<bit)
#define   _BIT(bit)       (~(1<<bit))
#define   _BV(bit)          (1<<bit)

#define RTS_LED_PIN_PORT  PORTD1
#define RTS_LED_PIN_DIRECTION  DDRD
#define RTS_LED_PIN  PORTD1


//-----------------------------------------------------------------------------------------------------
class CAdc
{
public:
    enum
    {
        DIV4		= 0x02,				// ADC_Clock = F_CPU/4
        DIV8		= 0x03,				// ADC_Clock = F_CPU/8
        DIV16		= 0x04,				// ADC_Clock = F_CPU/16
        DIV32		= 0x05,				// ADC_Clock = F_CPU/32
        DIV64		= 0x06,				// ADC_Clock = F_CPU/64
        DIV128	= 0x07,				// ADC_Clock = F_CPU/128
    };

//    enum
//    {
//        FSM_IDDLE = 0,
//        FSM_START,
//        FSM_CONTINUOUS_MEASURE,
//        FSM_RECEIVE_MESSAGE,
//        FSM_RECEIVE_CONFIRMATION,
//        FSM_REPLY,
//        FSM_FRAME_TRANSMIT,
//        FSM_FRAME_TRANSMIT_END_WAITING,
//        FSM_RESTART,
//        FSM_DATA_EXCHANGE_END_WAITING,
//    };

//    CAdc();
//    virtual ~CAdc();
    static void Init();
    static void ChannelSelect(uint8_t );
//    static void StartSingle(void);
    static void Start(void);
    static void Stop(void);
    static void Enable(void);
    static void Disable(void);
    static bool MeasureIsComlete(void)
    {
//        if (BitIsClear(ADCSRA, ADSC))
//        {
//            return 1;
//        }
//        else
//        {
//            return 0;
//        }
        if (BitIsSet(ADCSRA, ADIF))
        {
            SetBit(ADCSRA, ADIF);
            return 1;
        }
        else
        {
            return 0;
        }
    };
    static uint16_t GetMeasureValue(void);
//    static __interrupt void SIG_ADC(void);

protected:
private:
    static bool m_fbMeasureIsComlete;

};
//-----------------------------------------------------------------------------------------------------




//---------------------------------------- UART -------------------------------------------------------------------------------
//#define ttyO0 USART0
//#define ttyO1 USART1
//#define MODBUS_RTU_BAUD_RATE          9600
//#define DEBUG_PORT_BAUD_RATE          9600
#define UART_BAUD_CALC(MODBUS_RTU_BAUD_RATE,F_OSC) \
    ( ( F_OSC ) / ( ( MODBUS_RTU_BAUD_RATE ) * 16UL ) - 1 )
#define UART_BUFFER_LENGTH 260
#define UART_INTERMEDIATE_BUFFER_LENGTH 16
#define RTS_ENABLE

////extern uint8_t *pucUsartTxBuff;
////extern uint16_t nuiUsartTxBuffByteCounter;
////extern uint8_t *pucUsartRxBuff;
////extern uint16_t nuiUsartRxBuffByteCounter;
////extern uint8_t aucUsartIntermediateBufff[];
////extern bool bfModbusUartByteIsReceived;
////extern bool bfTaskIsAwakened;
////extern bool bfModbusUsartRxOverflow;
////
////int16_t iModbusUartInit(char *pucPORT,
////                        uint32_t ulBaudRate,
////                        uint8_t ucDataBits,
////                        uint8_t ucParity,
////                        uint8_t ucStopBit );
////void vModbusUartEnable(void);
////void vModbusUartDisable(void);
////
//////ssize_t write (int16_t fd, const void * buffer, size_t count);
//////#define write(Socket, Source, Length) iModbusUsartSend(Socket, Source, Length)
//////#define write CUart::Send
//////#define write Send
////
////int16_t iModbusUsartSend(int16_t iSocket, const uint8_t *puiSource, int16_t uiLength);
//////ssize_t read (int16_t fd, void * buffer, size_t count);
//////#define read(Socket, Destination, Length) iModbusUsartReceive(Socket, Destination, Length)
//////#define read CUart::Receive
//////#define read Receive
////int16_t iModbusUsartReceive(int8_t iSocket, uint8_t *pucDestination, int16_t uiLength);
//void vDebugUARTInit(void);
////int16_t MyLowLevelPutchar(int16_t sendchar);
//int16_t MyLowLevelPutchar(char sendchar);
//char MyLowLevelGetchar(void);
////int16_t PutString(char *pcSource);
////int16_t PutString(const char __farflash *pcSource);
////int16_t R_printf_P(const char __farflash* ccSource, int iData);
////const char* ccFlashToRamStringRead(const char __farflash* pccfSource);


//-----------------------------------------------------------------------------------------------------
class CUart
{
public:
    enum
    {
        UART_MAX_BUFF_LENGTH = 64,
        UART_INTERMEDIATE_BUFF_LENGTH = 16
    };

//    CUart();
//    CUart(volatile uint8_t * , volatile uint8_t * ,
//          volatile uint8_t * , volatile uint8_t * ,
//          volatile uint8_t * , volatile uint8_t * ,
//          volatile uint8_t * , volatile uint8_t ,
//          volatile uint8_t * , volatile uint8_t );
//    virtual ~CUart();

    static void UartBind(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
                         volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
                         volatile uint8_t *ucsrc, volatile uint8_t *udr,
                         volatile uint8_t *rs485ddr, volatile uint8_t rs485ddpin,
                         volatile uint8_t *rs485port, volatile uint8_t rs485pin);
//-----------------------------------------------------------------------------------------------------
//    static void UdreInterruptHandler(void);
//    static void TxcInterruptHandler(void);
//    static void RecvInterruptHandler(void);
    static void Init(uint32_t ,
                     uint8_t ,
                     uint8_t ,
                     uint8_t  ,
                     uint8_t *,
                     uint8_t *);
    static void Reset(void);
    static void ReceiveEnable(void);
    static void ReceiveDisable(void);
    static void TransmitEnable(void);
    static void TransmitDisable(void);
//    static void Rs485RtsOn(void);
//    static void Rs485RtsOff(void);
    static int16_t Write(uint8_t * , uint16_t );
    static int16_t Read(uint8_t * , uint16_t );
//    static int16_t Read(void);
    static uint8_t ByteIsReceived(void)
    {
        return m_bfByteIsReceived;
    }
    static uint8_t FrameIsSended(void)
    {
        return m_bfFrameIsSended;
    };
    static uint16_t GetFrameLength(void)
    {
        return m_nuiRxBuffByteCounter;
    };

// #pragma vector = USART_RX_vect		// ���������� �� ���������� ������ UART
// static __interrupt void __URXComplete(void);
//
// #pragma vector = USART_UDRE_vect	// ���������� �� ������� �������� ������ UDR
// static __interrupt void __UDREmpty(void);
//
// #pragma vector = USART_TX_vect		// ���������� �� ���������� �������� UART
// static __interrupt void __UTXComplete(void);
//-----------------------------------------------------------------------------------------------------
//private:
    static volatile uint8_t* m_UBRRH;
    static volatile uint8_t* m_UBRRL;
    static volatile uint8_t* m_UCSRA;
    static volatile uint8_t* m_UCSRB;
    static volatile uint8_t* m_UCSRC;
    static volatile uint8_t* m_UDR;
    static volatile uint8_t* m_rs485ddr;
    static volatile uint8_t m_rs485ddpin;
    static volatile uint8_t* m_rs485port;
    static volatile uint8_t m_rs485pin;

    static uint8_t* m_puiTxBuffer;
    static uint16_t m_nuiTxBuffByteCounter;
    static uint8_t* m_puiRxBuffer;
    static uint16_t m_nuiRxBuffByteCounter;
    static uint8_t m_auiIntermediateBuff[UART_INTERMEDIATE_BUFF_LENGTH];
    static bool m_bfByteIsReceived;
    static bool m_bfFrameIsSended;
    static bool m_bfRxBuffOverflow;

//protected:

};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
class CEeprom
{
public:
//    CEeprom();
//    virtual ~CEeprom();
    static uint8_t Read(uint8_t * , uint16_t , uint16_t );
    static uint8_t Write(uint16_t , uint8_t * , uint16_t );
    static uint8_t ReadByte(uint16_t );
    static void WriteByte(uint16_t , uint8_t );

private:

};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
enum
{
    SPI_SS5 = 2,     // PD2 - ���� ���������� �� Master
    SPI_MOSI = 3,    // PB2 - ���� SPI
    SPI_MISO = 4,    // PB3 - ����� SPI
    SPI_SCK = 5     // PB5 - ���� ��� ���������������
};

enum
{
    SPI_MAX_BUFF_LENGTH = 64,
};

class CSpi
{
public:
    CSpi();
    virtual ~CSpi();

    static void Init(uint8_t * , uint8_t * );
    static void Enable(void);
    static void Disable(void);
//    static void RecvInterruptHandler(void);
    static int16_t Exchange(void);
//    static uint8_t Read(uint8_t * , uint16_t , uint16_t );
//    static uint8_t Write(uint16_t , uint8_t * , uint16_t );
    static void Reset(void);
//-----------------------------------------------------------------------------------------------------
    static uint8_t DataExchangeInProgress(void)
    {
        return m_bfDataExchangeInProgress;
    }
//-----------------------------------------------------------------------------------------------------
    static void DataExchangeInProgressClear(void)
    {
        m_bfDataExchangeInProgress = 0;
    }
//-----------------------------------------------------------------------------------------------------
    static void DataExchangeInProgressSet(void)
    {
        m_bfDataExchangeInProgress = 1;
    }
//-----------------------------------------------------------------------------------------------------
    static uint8_t DataExchangeIsOccur(void)
    {
        return m_bfDataExchangeIsOccur;
    }
//-----------------------------------------------------------------------------------------------------
    static void DataExchangeIsOccurSet(void)
    {
        m_bfDataExchangeIsOccur = 1;
    };
//-----------------------------------------------------------------------------------------------------
    static void DataExchangeIsOccurClear(void)
    {
        m_bfDataExchangeIsOccur = 0;
    }
//-----------------------------------------------------------------------------------------------------
    static uint8_t ByteIsReceived(void)
    {
        return m_bfByteIsReceived;
    }
//-----------------------------------------------------------------------------------------------------
    static void ByteIsReceivedClear(void)
    {
        m_bfByteIsReceived = 0;
    }
//-----------------------------------------------------------------------------------------------------
    static uint16_t GetFrameLength(void)
    {
        return m_nuiBuffByteCounter;
    }

    static const uint8_t BUFFER_LENGTH = 16;
//-----------------------------------------------------------------------------------------------------
//private:
    static uint8_t m_uiExchangeByte;
    static uint8_t* m_puiRxBuffer;
    static uint8_t* m_puiTxBuffer;
    static uint16_t m_nuiBuffByteCounter;
    static uint16_t m_uiReceivedByteCounter;
    static bool m_bfByteIsReceived;
    static bool m_bfByteIsTransmited;
    static bool m_bfDataExchangeInProgress;
    static bool m_bfDataExchangeIsOccur;
    static bool m_bfRxBuffOverflow;
    static uint8_t m_auiSpiRxBuffer[BUFFER_LENGTH];
    static uint8_t m_auiSpiTxBuffer[BUFFER_LENGTH];

};
//-----------------------------------------------------------------------------------------------------





//-----------------------------------------------------------------------------------------------------
class CPlatform
{
public:
//    CPlatform();
//    virtual ~CPlatform();

//    static const uint32_t F_CPU = 7372800;

//-----------------------------------------------------------------------------------------------------
    static void WatchdogReset(void)
    {
        __watchdog_reset();
    }

//-----------------------------------------------------------------------------------------------------
    static void WatchdogEnable(void)
    {
        enum
        {
            WDTO_15MS = 0,
            WDTO_30MS = 1,
            WDTO_60MS = 2,
            WDTO_120MS = 3,
            WDTO_250MS = 4,
            WDTO_500MS = 5,
            WDTO_1S = 6,
            WDTO_2S = 7
        };
        __watchdog_reset();
        /* Write logical one to WDCE and WDE */
        WDTCSR |= (1<<WDCE) | (1<<WDE);
        WDTCSR |= (WDTO_2S & 0x07);
    }

//-----------------------------------------------------------------------------------------------------
    static void InterruptEnable(void)
    {
        __enable_interrupt();
    }

//-----------------------------------------------------------------------------------------------------
    static void InterruptDisable(void)
    {
        __disable_interrupt();
    }

//-----------------------------------------------------------------------------------------------------
    static void IncrementSystemTick(void)
    {
        m_uiSystemTick++;
    }

//-----------------------------------------------------------------------------------------------------
    static uint16_t GetSystemTick(void)
    {
        return m_uiSystemTick;
    }

////-----------------------------------------------------------------------------------------------------
//    static void StatusLedSetPinOutput(void)
//    {
//        DDRD |=  BIT(DDD3);
//    };
//
////-----------------------------------------------------------------------------------------------------
//    static void StatusLedOn(void)
//    {
//        PORTD &= ~BIT(PD3);
//    };
//
////-----------------------------------------------------------------------------------------------------
//    static void StatusLedOff(void)
//    {
//        PORTD |=  BIT(PD3);
//    };
//-----------------------------------------------------------------------------------------------------------------
    static void TxLedSetPinOutput(void)
    {
        DDRD |=  BIT(DDD3);
    };

//-----------------------------------------------------------------------------------------------------------------
    static void TxLedOn(void)
    {
        PORTD &= ~BIT(PD3);
    };

//-----------------------------------------------------------------------------------------------------------------
    static void TxLedOff(void)
    {
        PORTD |=  BIT(PD3);
    };


//-----------------------------------------------------------------------------------------------------
    static void Init(void);
    static void Int0InterruptEnable(void);
    static void Int0InterruptDisable(void);

//    static CUart* m_pxUart0;
//    static CUart* m_pxUart1;
//    static CSpi* m_pxSpi;
    static uint8_t uiSlaveSelectIsHigh;

//-----------------------------------------------------------------------------------------------------
private:
    static uint16_t m_uiSystemTick;

protected:
};
//-----------------------------------------------------------------------------------------------------
#endif // PLATFORM_H

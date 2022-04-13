#ifndef CADC_H
#define CADC_H

#include <stdint.h>

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

    enum
    {
        FSM_IDDLE = 0,
        FSM_START,
        FSM_WAITING_MESSAGE,
        FSM_RECEIVE_MESSAGE,
        FSM_RECEIVE_CONFIRMATION,
        FSM_REPLY,
        FSM_FRAME_TRANSMIT,
        FSM_FRAME_TRANSMIT_END_WAITING,
        FSM_RESTART,
        FSM_DATA_EXCHANGE_END_WAITING,
    };

    CAdc();
    virtual ~CAdc();
    static void Init();
    static void ChannelSelect(uint8_t );
    static void StartSingle(void);
    static void ContinuousMeasure(void);
    static void Stop(void);
    static void Fsm(void);
    static bool MeasureIsComlete(void)
    {
        return m_fbMeasureIsComlete;
    };
    static __interrupt void SIG_ADC(void);

    static const uint8_t HANDLED_CHANNEL_NUMBER = 8;
protected:
private:
    static uint8_t m_uiFlowControl;
    static bool m_fbMeasureIsComlete;

};



#include <stdint.h>

//-----------------------------------------------------------------------------------------------------
#endif // CADC_H

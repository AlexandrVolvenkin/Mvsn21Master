
    NAME SpiInterrupt
#include <ioavr.h>

    PUBLIC SpiInterrupt

    RSEG   CODE

SpiInterrupt:

`?<Name CSpi 0>`    SYMBOL "CSpi"
??m_bfByteIsReceived SYMBOL "m_bfByteIsReceived", `?<Name CSpi 0>`
??m_bfRxBuffOverflow SYMBOL "m_bfRxBuffOverflow", `?<Name CSpi 0>`
??m_nuiBuffByteCounter SYMBOL "m_nuiBuffByteCounter", `?<Name CSpi 0>`
??m_puiRxBuffer     SYMBOL "m_puiRxBuffer", `?<Name CSpi 0>`
??m_puiTxBuffer     SYMBOL "m_puiTxBuffer", `?<Name CSpi 0>`
??m_uiExchangeByte  SYMBOL "m_uiExchangeByte", `?<Name CSpi 0>`

SIG_SPI_STC         SYMBOL "SIG_SPI_STC"
`??SIG_SPI_STC::??INTVEC 34` SYMBOL "??INTVEC 34", SIG_SPI_STC

        EXTERN ??m_bfByteIsReceived
        EXTERN ??m_bfRxBuffOverflow
        EXTERN ??m_nuiBuffByteCounter
        EXTERN ??m_puiRxBuffer
        EXTERN ??m_puiTxBuffer
        EXTERN ??m_uiExchangeByte

        PUSH R16
        LDS     R16, ??m_uiExchangeByte
        OUT     0x2E, R16
        POP R16



        ST      -Y, R31
        ST      -Y, R30
        ST      -Y, R20
        ST      -Y, R19
        ST      -Y, R18
        ST      -Y, R17
        ST      -Y, R16
        IN      R20, 0x3F
//    8     SPDR = CSpi::m_uiExchangeByte;
;        LDS     R16, ??m_uiExchangeByte
;        OUT     0x2E, R16
//    9     CSpi::m_puiRxBuffer[CSpi::m_nuiBuffByteCounter] = SPDR;
        IN      R16, 0x2E
        LDS     R18, ??m_nuiBuffByteCounter
        LDS     R19, (??m_nuiBuffByteCounter + 1)
        LDS     R30, ??m_puiRxBuffer
        LDS     R31, (??m_puiRxBuffer + 1)
        ADD     R30, R18
        ADC     R31, R19
        ST      Z, R16
//   10     CSpi::m_uiExchangeByte = CSpi::m_puiTxBuffer[CSpi::m_nuiBuffByteCounter + 1];
        LDS     R16, ??m_nuiBuffByteCounter
        LDS     R17, (??m_nuiBuffByteCounter + 1)
        LDS     R30, ??m_puiTxBuffer
        LDS     R31, (??m_puiTxBuffer + 1)
        ADD     R30, R16
        ADC     R31, R17
        LDD     R16, Z+1
        STS     ??m_uiExchangeByte, R16
//   11
//   12     // Р±СѓС„РµСЂ РїСЂРёС‘РјР° РЅРµ РїРµСЂРµРїРѕР»РЅРµРЅ?
//   13     if (CSpi::m_nuiBuffByteCounter <
//   14             CSpi::BUFFER_LENGTH)
        LDS     R30, ??m_nuiBuffByteCounter
        LDS     R31, (??m_nuiBuffByteCounter + 1)
        SBIW    R31:R30, 16
        BRCC    ??SIG_SPI_STC_1
//   15     {
//   16         CSpi::m_nuiBuffByteCounter++;
        LDI     R30, LOW(??m_nuiBuffByteCounter)
        LDI     R31, (??m_nuiBuffByteCounter) >> 8
        LD      R16, Z
        SUBI    R16, 255
        SBCI    R17, 255
        ST      Z, R16
        STD     Z+1, R17
//   17         CSpi::m_bfByteIsReceived = 1;
        RJMP    ??SIG_SPI_STC_2
//   18     }
//   19     else
//   20     {
//   21         // РЅРµ РёРЅРєСЂРµРјРµРЅС‚РёСЂСѓРµРј m_nuiBuffByteCounter, С‡С‚РѕР±С‹ РЅРµ РІС‹Р№С‚Рё Р·Р° РіСЂР°РЅРёС†С‹ Р±СѓС„РµСЂР°.
//   22         // СѓСЃС‚Р°РЅРѕРІРёРј С„Р»Р°Рі - РїСЂРѕРёР·РѕС€РµР» РѕР±РјРµРЅ РґР°РЅРЅС‹РјРё РїРѕ SPI.
//   23         CSpi::m_bfRxBuffOverflow = 1;
??SIG_SPI_STC_1:
        LDI     R16, 1
        STS     ??m_bfRxBuffOverflow, R16
//   24         CSpi::m_bfByteIsReceived = 1;
??SIG_SPI_STC_2:
        LDI     R16, 1
        STS     ??m_bfByteIsReceived, R16
//   25     }
//   26 }
        OUT     0x3F, R20
        LD      R16, Y+
        LD      R17, Y+
        LD      R18, Y+
        LD      R19, Y+
        LD      R20, Y+
        LD      R30, Y+
        LD      R31, Y+
        RETI


COMMON INTVEC(1)            ; Code in interrupt vector segment

ORG SPI_STC_vect               ; Place code at interrupt vector
    JMP   SpiInterrupt           ; Jump to assembler interrupt function

ENDMOD
END

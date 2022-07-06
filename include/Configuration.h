//-----------------------------------------------------------------------------------------------------
//  Sourse      : FileName.cpp
//  Created     : 01.06.2022
//  Author      : Alexandr Volvenkin
//  email       : aav-36@mail.ru
//  GitHub      : https://github.com/AlexandrVolvenkin
//-----------------------------------------------------------------------------------------------------
#ifndef CONFIGURATION_H_INCLUDED
#define CONFIGURATION_H_INCLUDED

// "�������" � "������������" � ������� 7 ������ ���������� ��������� ��������� �� ������� �����.

//-----------------------------------------------------------------------------------------------------


enum
{
    MASTER_DATA_EXCHANGE_MAX_MESSAGE_LENGTH = 64,
    MEASURE_CHANNEL_NUMBER = 8,
    MEASURE_CHANNEL_STATE_BIT_NUMBER = 2,
    CHANNELS_IN_BYTE = 4,
    CHIP_NUMBER = 3,
    DISCRETE_INPUT_NUMBER = (MEASURE_CHANNEL_NUMBER * CHIP_NUMBER),
    SLAVE_1_CHIP_ADDRESS = 0,
    SLAVE_2_CHIP_ADDRESS = 1,
    MASTER_CHIP_ADDRESS = 2,
};

enum
{
    MODBUS_RTU_BAUD_RATE = 9600UL,
    MODBUS_RTU_MAX_ADU_LENGTH = 32,
    SPI_BUFFER_LENGTH = 64,
    SPI_MAX_BUS_ADDRESS = 16,
// 2 - �������� �� ������ � ������� RX SPI. 0 - ������� �������, 1 - "���" ������� �������.
    SPI_DATA_BYTE_OFFSET = 3,
// 1 - �������� �� ������� � ������� RX SPI.
    SPI_COMMAND_BYTE_OFFSET = 2,
    SPI_COMMAND_BYTE_LENGTH = 1,
    ONE_BYTE_CRC_LENGTH = 1,
    TWO_BYTE_CRC_LENGTH = 2,

    COILS_WORK_ARRAY_LENGTH = 8,
    DISCRETE_INPUTS_ARRAY_LENGTH = ((MEASURE_CHANNEL_NUMBER * MEASURE_CHANNEL_STATE_BIT_NUMBER) * CHIP_NUMBER),
    HOLDING_REGISTERS_ARRAY_LENGTH = 8,
    INPUT_REGISTERS_ARRAY_LENGTH = 8,
};

typedef enum
{
    // ����� ��������� ���������� ���������.
    HEADER_LENGTH = 3,
    CRC_LENGTH = 2,
    TAIL_LENGTH = CRC_LENGTH,
    MESSAGE_BAG_MAX_LENGTH = 32,
    MIN_MESSAGE_LENGTH = 4,
};


//-----------------------------------------------------------------------------------------------------
#endif // CONFIGURATION_H_INCLUDED

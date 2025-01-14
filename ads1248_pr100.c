/*
 * ads1248.c
 *
 *  Created on: 2025. 1. 11.
 *      Author: alllite
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "ads1248_reg.h"
#include "ads1248_pt100.h"

#define ONE_REGISTER_READ_WRITE     0
#define TWO_REGISTER_READ_WRITE     1
#define THREE_REGISTERS_READ_WRITE  2  // 3-1


uint16_t Spi_write_data[8]= {0,};

void Spi_Read_write_8bit(uint16_t * Pbuff,uint16_t Length,uint16_t uiBase);
/*
1.4 ADC Initialization for RTD sampling (Ratiometric)
�궥 When ADC is initialized for RTD sampling, the excitation currents should be turned ON and routed
through IEXC1 and 2.
�궥 The excitation current of 500 uA is chosen.
�궥 Current swapping by routing the same current through the different excitation source and averaging them
will cancel out errors.
�궥 Internal reference is always ON.
�궥 The external reference of REF1 pair is selected.
�궥 Gain of 16 and sampling rate of 20 samples per second settings are chosen.
�궥 During initialization self-offset calibration process is initiated, followed by a settling time delay.
*/
void ADS1248_Init(void)
{
    //SPI pins are already configured
    //Reset low is done already
    //chip select
    GPIO_writePin(ADC_CS_1, 0);
    DEVICE_DELAY_US(5);
    //configure the ADC registers
    Spi_write_data[0] = (WREG | IDAC0) << 8;
    Spi_write_data[1] = ONE_REGISTER_READ_WRITE; //ONE_REGISTER_READ_WRITE;
    Spi_write_data[2] = 0x04U << 8;//excitation current 500uA (RTD)
    Spi_Read_write_8bit(Spi_write_data, 3, SPIC_BASE);
    DEVICE_DELAY_US(5);
    GPIO_writePin(ADC_CS_1, 1);

    DEVICE_DELAY_US(100000);

#if 1
    GPIO_writePin(ADC_CS_1, 0);
    DEVICE_DELAY_US(10);
    Spi_write_data[0] = (WREG | IDAC1) << 8;
    Spi_write_data[1] = ONE_REGISTER_READ_WRITE;
    Spi_write_data[2] = 0x89U << 8;//0xFF - to disconnect //select IDAC output pins
    Spi_Read_write_8bit(Spi_write_data, 3, SPIC_BASE);
    DEVICE_DELAY_US(10);
    GPIO_writePin(ADC_CS_1, 1);

    DEVICE_DELAY_US(100000);


    GPIO_writePin(ADC_CS_1, 0);
    DEVICE_DELAY_US(10);
    Spi_write_data[0] = (WREG | MUX1) << 8;
    Spi_write_data[1] = ONE_REGISTER_READ_WRITE;
    Spi_write_data[2] = 0x28 << 8;//internal osc, internal reference ON, REF1 input pair
    Spi_Read_write_8bit(Spi_write_data, 3, SPIC_BASE);
    DEVICE_DELAY_US(10);
    GPIO_writePin(ADC_CS_1, 1);

    DEVICE_DELAY_US(100000);

    GPIO_writePin(ADC_CS_1, 0);
    DEVICE_DELAY_US(10);
    Spi_write_data[0] = (WREG | SYS0) << 8;
    Spi_write_data[1] = ONE_REGISTER_READ_WRITE;
    Spi_write_data[2] = 0x42 << 8;//Select gain 16 and sampling rate 20sps
    Spi_Read_write_8bit(Spi_write_data, 3, SPIC_BASE);
    DEVICE_DELAY_US(10);
    GPIO_writePin(ADC_CS_1, 1);

    DEVICE_DELAY_US(10000);

    GPIO_writePin(ADC_CS_1, 0);
    DEVICE_DELAY_US(10);
    Spi_write_data[0] = (WREG | OFC0) << 8;
    Spi_write_data[1] = THREE_REGISTERS_READ_WRITE << 8;
    Spi_write_data[2] = 0x00;//OFFSET calibration
    Spi_write_data[3] = 0x00;
    Spi_write_data[4] = 0x00;
    Spi_Read_write_8bit(Spi_write_data, 5, SPIC_BASE);
    DEVICE_DELAY_US(10);
    GPIO_writePin(ADC_CS_1, 1);

    DEVICE_DELAY_US(10);

    GPIO_writePin(ADC_CS_1, 0);
    Spi_write_data[0] = 0x62 << 8;
    Spi_write_data[1] = ONE_REGISTER_READ_WRITE;
    Spi_Read_write_8bit(Spi_write_data, 2, SPIC_BASE);
    DEVICE_DELAY_US(10);
    GPIO_writePin(ADC_CS_1, 1);

#endif
    DEVICE_DELAY_US(802000);

}


/*2.6 RTD channel switching
The following code snippet, switches external multiplexer through I2C bus and the ADC internal multiplexer
settings through SPI bus.
For ADC, to process AIN0 as positive and AIN1 as negative, MUX0 register is chosen as 0x01.*/
void select_Channel(uint16_t ChnNum)
{
    GPIO_writePin(ADC_CS_1, 0);
    DEVICE_DELAY_US(10);
    switch (ChnNum)
    {
        case 0:
            GPIO_writePin(RTD_Sel0, 0);
            GPIO_writePin(RTD_Sel1, 0);
            //first take default readings for AIN0+ and AIN1- channel1
            Spi_write_data[0] = (WREG | MUX0) << 8;
            Spi_write_data[1] = ONE_REGISTER_READ_WRITE << 8;
            Spi_write_data[2] = 0x01U << 8;//AIN0 positive and AIN1 negative
            Spi_Read_write_8bit(Spi_write_data, 3, SPIC_BASE);
            break;
        case 1:
            GPIO_writePin(RTD_Sel0, 1);
            GPIO_writePin(RTD_Sel1, 0);
            //second channel
            Spi_write_data[0] = (WREG | MUX0) << 8;
            Spi_write_data[1] = ONE_REGISTER_READ_WRITE << 8;
            Spi_write_data[2] = 0x13U << 8;//AIN2 positive and AIN3 negative
            Spi_Read_write_8bit(Spi_write_data, 3, SPIC_BASE);
            break;
        case 2:
            GPIO_writePin(RTD_Sel0, 0);
            GPIO_writePin(RTD_Sel1, 1);
            //third channel
            Spi_write_data[0] = (WREG | MUX0) << 8;
            Spi_write_data[1] = ONE_REGISTER_READ_WRITE << 8;
            Spi_write_data[2] = 0x25U << 8;//AIN4 positive and AIN5 negative
            Spi_Read_write_8bit(Spi_write_data, 3, SPIC_BASE);
            break;
        case 3:
            GPIO_writePin(RTD_Sel0, 1);
            GPIO_writePin(RTD_Sel1, 1);
            //fourth channel
            Spi_write_data[0] = (WREG | MUX0) << 8;
            Spi_write_data[1] = ONE_REGISTER_READ_WRITE << 8;
            Spi_write_data[2] = 0x37U << 8;//AIN6 positive and AIN7 negative
            Spi_Read_write_8bit(Spi_write_data, 3, SPIC_BASE);
        break;
    }

    DEVICE_DELAY_US(10);
    GPIO_writePin(ADC_CS_1, 1);
}


/*2.7 Read ADC Data
Wait for /DRDY to be asserted. Then issue a read data command on SPI bus. This is followed by sending 0xFF
thrice to send 24 clock cycles to ADC.*/
uint32_t spi_read_data(void)
{
    uint32_t Rtd_data;
    uint16_t temp_array[4];
    uint16_t temp;
    uint16_t data;

    temp_array[0] = RDATA << 8;//0x20;
    temp_array[1] = 0xFFU << 8;
    temp_array[2] = 0xFFU << 8;
    temp_array[3] = 0xFFU << 8;

    GPIO_writePin(ADC_CS_1, 0);
    DEVICE_DELAY_US(10);

    Spi_Read_write_8bit(temp_array, 4, SPIC_BASE);

    // Get 24 bit data from ADC
    for(temp=0;temp<3;temp++)
    {
        data = temp_array[temp];
        Rtd_data = (Rtd_data<<8) | data;
    }

    DEVICE_DELAY_US(10);
    GPIO_writePin(ADC_CS_1, 1);

    return Rtd_data;
}

uint32_t spi_read_id(void)
{
    uint32_t Rtd_data;
    uint16_t temp_array[4];
    uint16_t temp;
    uint16_t data;

    temp_array[0] = (RREG | IDAC0) << 8;
    temp_array[1] = TWO_REGISTER_READ_WRITE << 8; //TWO_REGISTER_READ_WRITE;
    temp_array[2] = 0;
    temp_array[3] = 0;

    GPIO_writePin(ADC_CS_1, 0);
    DEVICE_DELAY_US(5);

    Spi_Read_write_8bit(temp_array, 4, SPIC_BASE);

    // Get 24 bit data from ADC
    for(temp=0;temp<4;temp++)
    {
        data = temp_array[temp];
        Rtd_data = (Rtd_data<<8) | data;
    }

    DEVICE_DELAY_US(5);
    GPIO_writePin(ADC_CS_1, 1);

    return Rtd_data;
}

/******************************************************************************
* @name Spi_Read_write_8bit
* @brief Send/ reseive data in the Specified ssi base for the specified length
* @param Pbuff : Pointer to send the data and to keep the received data
* @param Length : Length of the data need to be send/ receive
* @param uiBase : Base address of the ssi
* @return None
*****************************************************************************/
void Spi_Read_write_8bit(uint8_t * Pbuff,uint16_t Length,uint16_t uiBase)
{
    uint32_t temp=0;
    uint16_t temp_count;

    for(temp_count=0;temp_count<Length;temp_count++)
    {
        SPI_writeDataNonBlocking(SPIC_BASE, *(Pbuff));             // 0xFF00 �쑝濡� 8 bit �뜲�씠�꽣 �벐湲� �룞�옉 !!!
        temp = SPI_readDataBlockingNonFIFO(SPIC_BASE);
        *(Pbuff++) = temp;
    }
}

uint16_t testCnt=0;
int32_t ADS1248_readRegister()
{
    int32_t ret=0;
    int32_t regValue = 0;
    uint16_t wrBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t wrxBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t i;

    wrBuf[0] = (WREG | IDAC0) << 8;
    wrBuf[1] = ONE_REGISTER_READ_WRITE;
    wrBuf[2] = 0;
    wrBuf[3] = 0;

    GPIO_writePin(ADC_CS_1, 0);
    testCnt = 0;
    for (i=0; i< 3 ; i++)
    {
        SPI_writeDataNonBlocking(SPIC_BASE, (wrBuf[i]));             // 0xFF00 으로 8 bit 데이터 쓰기 동작 !!!
        wrxBuf[i] = SPI_readDataBlockingNonFIFO(SPIC_BASE);
        testCnt++;
    }

    GPIO_writePin(ADC_CS_1, 1);

    return ret;
}

int32_t ADS1248_writeRegister()
{
    int32_t ret=0;
    int32_t regValue = 0;
    uint16_t wrBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t wrxBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t i;

    wrBuf[0] = (RREG | IDAC0) << 8;
    wrBuf[1] = ONE_REGISTER_READ_WRITE;
    wrBuf[2] = 0x04U << 8;
    wrBuf[3] = 0;

    GPIO_writePin(ADC_CS_1, 0);
    testCnt = 0;
    for (i=0; i< 3 ; i++)
    {
        SPI_writeDataNonBlocking(SPIC_BASE, (wrBuf[i]));             // 0xFF00 으로 8 bit 데이터 쓰기 동작 !!!
        wrxBuf[i] = SPI_readDataBlockingNonFIFO(SPIC_BASE);
        testCnt++;
    }

    GPIO_writePin(ADC_CS_1, 1);

    return ret;
}

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
//#include "Communication.h"

#include "ads1248_reg.h"
#include "driverlib.h"
#include "device.h"
#include "stdint.h"
//#include "driverlib.h"
//#include "fpu_filter.h"         // Main include file
#include "math.h"
#include "ads1248.h"
//#include "leastSquares.h"


#ifdef _FLASH
#pragma CODE_SECTION(ADS1248_ReadData, ".TI.ramfunc");
#pragma CODE_SECTION(ADS1248_ReadRegister, ".TI.ramfunc");
#pragma CODE_SECTION(ADS1248_WriteRegister, ".TI.ramfunc");
#pragma CODE_SECTION(Send_Data, ".TI.ramfunc");
#pragma CODE_SECTION(SPI_writeDataNonBlocking, ".TI.ramfunc");
#pragma CODE_SECTION(SPI_readDataBlockingNonFIFO, ".TI.ramfunc");
#pragma CODE_SECTION(doubleToString, ".TI.ramfunc");
#pragma CODE_SECTION(moving_average, ".TI.ramfunc");
#pragma CODE_SECTION(intToString, ".TI.ramfunc");
#pragma CODE_SECTION(itoa, ".TI.ramfunc");



#endif

#define  ADC_SMAPLE_CNT     1024
#define WINDOW_SIZE 10
//#define ALPHA1 0.998
//#define ALPHA2 0.95


#define PI  3.14159265359

#define _16BIT_DATA     //  spi 통신 16bit 사이즈로 사용
//#define _FIR_LPF
#define EN_TEMP


// moving average
#define MOV_FILTERSIZE 128
#define CHANNEL     3


#define ACCEL_TYPE  0
#define GYRO_TYPE   1
#define FLUX_TYPE   2

#define AVR_NUM             (1000) //(1000) //(256) //256    // 데이터 개수
#define MASK_LENGTH         (10)
#define THRESHOLD_FACTOR    (1.5)

int32_t exponentialMovingAverage2(double adc, double alpha);

double alpha2 = 0.98;

//int16_t gSend_data[8] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03};


uint8_t upperByte(uint16_t uint16_Word)
{
    uint8_t msByte;
    msByte = (uint8_t) ((uint16_Word >> 8) & 0x00FF);

    return msByte;
}


uint8_t lowerByte(uint16_t uint16_Word)
{
    uint8_t lsByte;
    lsByte = (uint8_t) (uint16_Word & 0x00FF);

    return lsByte;
}


//#define AVR_EN
int32_t ADS1248_ReadRegister(st_reg* pReg)
{
    int32_t ret;
    uint16_t i;
    uint16_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t wrxBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    /* Build the Command word */
    buffer[0] = RREG | (pReg->addr << 3);

    GPIO_writePin(my_GPIO0_CS, 0);

    DEVICE_DELAY_US(1);

    for (i=0; i< pReg->size; i++)
    {
        SPI_writeDataNonBlocking(SPIA_BASE, (buffer[i]));             // 0xFF00 으로 8 bit 데이터 쓰기 동작 !!!
        wrxBuf[i] = SPI_readDataBlockingNonFIFO(SPIA_BASE);
    }

    DEVICE_DELAY_US(1);

    GPIO_writePin(my_GPIO0_CS, 1);

    pReg->value = 0;
    pReg->value = (long)((unsigned long)wrxBuf[1] << 8U);
    pReg->value = (long)((unsigned long)pReg->value | wrxBuf[0]);

    return ret;
}

int32_t ADS1248_WriteRegister(st_reg* reg)
{
    int32_t ret=0;
    int32_t regValue = 0;
    uint16_t wrBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t wrxBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t i;

    /* Build the Command word */
    wrBuf[0] = WREG | (reg->addr << 3);
    wrBuf[1] = reg->value << 16;
    wrBuf[2] = reg->value;

    GPIO_writePin(my_GPIO0_CS, 0);

    for (i=0; i< reg->size ; i++)
    {
        SPI_writeDataNonBlocking(SPIA_BASE, (wrBuf[i]));             // 0xFF00 으로 8 bit 데이터 쓰기 동작 !!!
        wrxBuf[i] = SPI_readDataBlockingNonFIFO(SPIA_BASE);
    }

    GPIO_writePin(my_GPIO0_CS, 1);

    return ret;
}


int32_t ADS1248_Reset(void)
{
    int32_t ret;
    int i;

//    unsigned char wrBuf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint16_t wrBuf[8] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
    uint16_t wrxBuf[8];

//    ESTOP0;
    GPIO_writePin(my_GPIO0_CS, 0);
    for (i=0; i<1; i++)
    {
        SPI_writeDataNonBlocking(SPIA_BASE, wrBuf[i]);
        wrxBuf[i] = SPI_readDataBlockingNonFIFO(SPIA_BASE);
    }

    GPIO_writePin(my_GPIO0_CS, 1);

    return ret;
}

int32_t ADS1248_WaitForReady(uint32_t timeout)
{
//        ESTOP0;
    return 0;//timeout ? 0 : -1;
}

long ADS1248_ReadData(long* pData)
{

    int32_t ret;

    setFlag = 1;
#if 0
//    /* Read the value of the Status Register */
    ret = ADS1248_ReadRegister(&ADS1248_regs[STATUS]);
    *pData = ADS1248_regs[STATUS].value;
#endif

#if 0
    ret = ADS1248_ReadRegister(&ADS1248_regs[ID]);
    *pData = ADS1248_regs[ID].value;
#endif

#if 0
    ret = ADS1248_ReadRegister(&ADS1248_regs[CLOCK]);
    *pData = ADS1248_regs[CLOCK].value;
#endif

#if 1
    ret = ADS1248_ReadRegister(&ADS1248_regs[GAIN]);
    *pData = ADS1248_regs[GAIN].value;
#endif
    return ret;
}


int32_t ADS1248_Setup(int a)
{
    int32_t ret;
    enum ADS1248_registers regNr;
    long adcdata=0;
    ret = 0;
//    while(1)

        if(a == 1){
            ret = ADS1248_WriteRegister(&ADS1248_regs[THRSHLD_LSB]);
        }

        if(a == 2)
        {
            ret = ADS1248_WriteRegister(&ADS1248_regs[CLOCK]);
//                    ret = readSingleRegister(&ADS1248_regs[CLOCK]);
        }

        if(a == 3)
        {
            ret = ADS1248_WriteRegister(&ADS1248_regs[GAIN]);
        }

        return 0;
}


uint16_t readSingleRegister(st_reg* pReg)
{
    int32_t ret=0;
    uint16_t i;
    uint16_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t wrxBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint16_t index         = 0;

    /* Build the Command word */
    buffer[0] = OPCODE_RREG | (pReg->addr << 7);

        GPIO_writePin(my_GPIO0_CS, 0);

    DEVICE_DELAY_US(1);

    for (i=0; i< pReg->size; i++)
    {
        // Transmit data
        SPI_writeDataNonBlocking(SPIA_BASE, (buffer[i]));             // 0xFF00 으로 8 bit 데이터 쓰기 동작 !!!
        wrxBuf[i] = SPI_readDataBlockingNonFIFO(SPIA_BASE);
    }

    DEVICE_DELAY_US(1);

        GPIO_writePin(my_GPIO0_CS, 1);


    gADCBuffer_A0 = wrxBuf[0];

    return ret;
}

int32_t exponentialMovingAverage2(double adc, double alpha) {


    avrsum_x = (1 - alpha) * adc + alpha * (double)avrsum_x;

    return avrsum_x;
}

//   return done;


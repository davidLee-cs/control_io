//#############################################################################
//
// FILE:   spi_ex1_loopback.c
//
// TITLE:  SPI Digital Loopback
//
//! \addtogroup driver_example_list
//! <h1>SPI Digital Loopback</h1>
//!
//! This program uses the internal loopback test mode of the SPI module. This
//! is a very basic loopback that does not use the FIFOs or interrupts. A
//! stream of data is sent and then compared to the received stream.
//! The pinmux and SPI modules are configure through the sysconfig file.
//!
//! The sent data looks like this: \n
//!  0000 0001 0002 0003 0004 0005 0006 0007 .... FFFE FFFF 0000
//!
//! This pattern is repeated forever.
//!
//! \b External \b Connections \n
//!  - None
//!
//! \b Watch \b Variables \n
//!  - \b sData - Data to send
//!  - \b rData - Received data
//!
//
//#############################################################################
//
// 
// $Copyright:
// Copyright (C) 2014-2024 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "ads1248_pt100.h"

uint32_t sData = 0;                  // Send data
uint32_t rData = 0;                  // Receive data

//
// Main
//
void main(void)
{

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Disable pin locks and enable internal pullups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Board initialization
    //
    Board_init();

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;

    GPIO_writePin(ADC_Reset, 1);
    GPIO_writePin(ADC_CS_1, 1);
    GPIO_writePin(ADC_Start_1, 1);
    DEVICE_DELAY_US(100);

    ADS1248_Init();
    select_Channel(2);

    while(1)
    {
//        GPIO_writePin(ADC_Start_1, 0);
//        DEVICE_DELAY_US(100);
//        GPIO_writePin(ADC_Start_1, 1);
//        DEVICE_DELAY_US(100);
//        GPIO_writePin(ADC_Start_1, 0);

        uint32_t dr = GPIO_readPin(ADC_DRDY_1);
        if(dr == 0)
        {
            rData = spi_read_data();
//            rData = spi_read_id();
    //        rData = ADS1248_readRegister();
            DEVICE_DELAY_US(100000);
        }

    }
}

//
// End File
//

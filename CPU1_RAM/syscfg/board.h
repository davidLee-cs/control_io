/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************
//
// GPIO55 - GPIO Settings
//
#define ADC_CS_1_GPIO_PIN_CONFIG GPIO_55_GPIO55
//
// GPIO56 - GPIO Settings
//
#define ADC_Start_1_GPIO_PIN_CONFIG GPIO_56_GPIO56
//
// GPIO9 - GPIO Settings
//
#define ADC_Reset_GPIO_PIN_CONFIG GPIO_9_GPIO9
//
// GPIO64 - GPIO Settings
//
#define RTD_Sel0_GPIO_PIN_CONFIG GPIO_64_GPIO64
//
// GPIO65 - GPIO Settings
//
#define RTD_Sel1_GPIO_PIN_CONFIG GPIO_65_GPIO65
//
// GPIO49 - GPIO Settings
//
#define ADC_DRDY_1_GPIO_PIN_CONFIG GPIO_49_GPIO49
//
// GPIO53 - GPIO Settings
//
#define LED_RED_GPIO_PIN_CONFIG GPIO_53_GPIO53

//
// SPIC -> mySPI0 Pinmux
//
//
// SPIC_PICO - GPIO Settings
//
#define GPIO_PIN_SPIC_PICO 50
#define mySPI0_SPIPICO_GPIO 50
#define mySPI0_SPIPICO_PIN_CONFIG GPIO_50_SPISIMOC
//
// SPIC_POCI - GPIO Settings
//
#define GPIO_PIN_SPIC_POCI 51
#define mySPI0_SPIPOCI_GPIO 51
#define mySPI0_SPIPOCI_PIN_CONFIG GPIO_51_SPISOMIC
//
// SPIC_CLK - GPIO Settings
//
#define GPIO_PIN_SPIC_CLK 52
#define mySPI0_SPICLK_GPIO 52
#define mySPI0_SPICLK_PIN_CONFIG GPIO_52_SPICLKC
//
// SPIC_PTE - GPIO Settings
//
#define GPIO_PIN_SPIC_PTE 72
#define mySPI0_SPIPTE_GPIO 72
#define mySPI0_SPIPTE_PIN_CONFIG GPIO_72_SPISTEC

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
#define ADC_CS_1 55
void ADC_CS_1_init();
#define ADC_Start_1 56
void ADC_Start_1_init();
#define ADC_Reset 9
void ADC_Reset_init();
#define RTD_Sel0 64
void RTD_Sel0_init();
#define RTD_Sel1 65
void RTD_Sel1_init();
#define ADC_DRDY_1 49
void ADC_DRDY_1_init();
#define LED_RED 53
void LED_RED_init();

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define mySPI0_BASE SPIC_BASE
#define mySPI0_BITRATE 2000000
#define mySPI0_DATAWIDTH 8
void mySPI0_init();

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	GPIO_init();
void	SPI_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition

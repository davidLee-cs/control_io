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

#include "board.h"

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void Board_init()
{
	EALLOW;

	PinMux_init();
	GPIO_init();
	SPI_init();

	EDIS;
}

//*****************************************************************************
//
// PINMUX Configurations
//
//*****************************************************************************
void PinMux_init()
{
	//
	// PinMux for modules assigned to CPU1
	//
	
	// GPIO55 -> ADC_CS_1 Pinmux
	GPIO_setPinConfig(GPIO_55_GPIO55);
	// GPIO56 -> ADC_Start_1 Pinmux
	GPIO_setPinConfig(GPIO_56_GPIO56);
	// GPIO9 -> ADC_Reset Pinmux
	GPIO_setPinConfig(GPIO_9_GPIO9);
	// GPIO64 -> RTD_Sel0 Pinmux
	GPIO_setPinConfig(GPIO_64_GPIO64);
	// GPIO65 -> RTD_Sel1 Pinmux
	GPIO_setPinConfig(GPIO_65_GPIO65);
	// GPIO49 -> ADC_DRDY_1 Pinmux
	GPIO_setPinConfig(GPIO_49_GPIO49);
	// GPIO53 -> LED_RED Pinmux
	GPIO_setPinConfig(GPIO_53_GPIO53);
	//
	// SPIC -> mySPI0 Pinmux
	//
	GPIO_setPinConfig(mySPI0_SPIPICO_PIN_CONFIG);
	GPIO_setPadConfig(mySPI0_SPIPICO_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPI0_SPIPICO_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(mySPI0_SPIPOCI_PIN_CONFIG);
	GPIO_setPadConfig(mySPI0_SPIPOCI_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPI0_SPIPOCI_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(mySPI0_SPICLK_PIN_CONFIG);
	GPIO_setPadConfig(mySPI0_SPICLK_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPI0_SPICLK_GPIO, GPIO_QUAL_SYNC);

	GPIO_setPinConfig(mySPI0_SPIPTE_PIN_CONFIG);
	GPIO_setPadConfig(mySPI0_SPIPTE_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(mySPI0_SPIPTE_GPIO, GPIO_QUAL_ASYNC);


}

//*****************************************************************************
//
// GPIO Configurations
//
//*****************************************************************************
void GPIO_init(){
	ADC_CS_1_init();
	ADC_Start_1_init();
	ADC_Reset_init();
	RTD_Sel0_init();
	RTD_Sel1_init();
	ADC_DRDY_1_init();
	LED_RED_init();
}

void ADC_CS_1_init(){
	GPIO_setPadConfig(ADC_CS_1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(ADC_CS_1, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(ADC_CS_1, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(ADC_CS_1, GPIO_CORE_CPU1);
}
void ADC_Start_1_init(){
	GPIO_setPadConfig(ADC_Start_1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(ADC_Start_1, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(ADC_Start_1, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(ADC_Start_1, GPIO_CORE_CPU1);
}
void ADC_Reset_init(){
	GPIO_setPadConfig(ADC_Reset, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(ADC_Reset, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(ADC_Reset, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(ADC_Reset, GPIO_CORE_CPU1);
}
void RTD_Sel0_init(){
	GPIO_setPadConfig(RTD_Sel0, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(RTD_Sel0, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(RTD_Sel0, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(RTD_Sel0, GPIO_CORE_CPU1);
}
void RTD_Sel1_init(){
	GPIO_setPadConfig(RTD_Sel1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(RTD_Sel1, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(RTD_Sel1, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(RTD_Sel1, GPIO_CORE_CPU1);
}
void ADC_DRDY_1_init(){
	GPIO_setPadConfig(ADC_DRDY_1, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(ADC_DRDY_1, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(ADC_DRDY_1, GPIO_DIR_MODE_IN);
	GPIO_setControllerCore(ADC_DRDY_1, GPIO_CORE_CPU1);
}
void LED_RED_init(){
	GPIO_setPadConfig(LED_RED, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(LED_RED, GPIO_QUAL_SYNC);
	GPIO_setDirectionMode(LED_RED, GPIO_DIR_MODE_OUT);
	GPIO_setControllerCore(LED_RED, GPIO_CORE_CPU1);
}

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
void SPI_init(){
	mySPI0_init();
}

void mySPI0_init(){
	SPI_disableModule(mySPI0_BASE);
	SPI_setConfig(mySPI0_BASE, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
				  SPI_MODE_CONTROLLER, mySPI0_BITRATE, mySPI0_DATAWIDTH);
	SPI_setPTESignalPolarity(mySPI0_BASE, SPI_PTE_ACTIVE_LOW);
	SPI_disableFIFO(mySPI0_BASE);
	SPI_disableLoopback(mySPI0_BASE);
	SPI_setEmulationMode(mySPI0_BASE, SPI_EMULATION_FREE_RUN);
	SPI_enableModule(mySPI0_BASE);
}


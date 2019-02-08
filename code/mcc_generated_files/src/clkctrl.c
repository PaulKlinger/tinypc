/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "../include/clkctrl.h"
/**
 * \brief Initialize clkctrl interface
 */
int8_t CLKCTRL_init()
{
	
	//  ccp_write_io((void*)&CLKCTRL.OSC32KCTRLA,0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */);
        
	//  ccp_write_io((void*)&(CLKCTRL.XOSC32KCTRLA),CLKCTRL_CSUT_1K_gc /* Crystal startup time: 1K cycles */
	// 		 | 0 << CLKCTRL_ENABLE_bp /* Enable: disabled */
	// 		 | 0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */
	// 		 | 0 << CLKCTRL_SEL_bp /* Select: disabled */);

	//  ccp_write_io((void*)&(CLKCTRL.OSC20MCTRLA),0 << CLKCTRL_RUNSTDBY_bp /* Run standby: disabled */);

	 ccp_write_io((void*)&(CLKCTRL.MCLKCTRLB),CLKCTRL_PDIV_2X_gc /* Prescaler division: 2X */
			 | 0 << CLKCTRL_PEN_bp /* Prescaler enable: disabled */);

	//  ccp_write_io((void*)&(CLKCTRL.MCLKCTRLA),0 << CLKCTRL_CLKOUT_bp /* System clock out: disabled */
	// 		 | CLKCTRL_CLKSEL_OSC20M_gc /* clock select: 20MHz internal oscillator */);

	//  ccp_write_io((void*)&(CLKCTRL.MCLKLOCK),0 << CLKCTRL_LOCKEN_bp /* lock ebable: disabled */);

	return 0;
}
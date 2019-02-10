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

#include "../include/rtc.h"

/**
 * \brief Initialize RTC interface
 */
int8_t RTC_0_init()
{
    while (RTC.STATUS > 0) { /* Wait for all register to be synchronized */
	}
        
    // RTC.CLKSEL = RTC_CLKSEL_INT32K_gc; /* Clock Select: Internal 32kHz OSC */
    // RTC.CMP = 0; /* Compare */
    // RTC.CNT = 0; /* Counter */

    // RTC.PER = 65535;

    // 	RTC.DBGCTRL = 0 << RTC_DBGRUN_bp; /* Run in debug: disabled */
    	RTC.INTCTRL = 0 << RTC_CMP_bp /* Compare Match Interrupt enable: disabled */
    				| 1 << RTC_OVF_bp; /* Overflow Interrupt enable: enabled */

    // 	RTC.INTFLAGS = 0 << RTC_CMP_bp /* Compare Match Interrupt: disabled */
    // 				| 0 << RTC_OVF_bp; /* Overflow Interrupt Flag: disabled */
	       
    // while (RTC.PITSTATUS > 0) { /* Wait for all register to be synchronized */
    // }
    // 	RTC.PITCTRLA = RTC_PERIOD_OFF_gc /* Period: Off */
    // 				| 0 << RTC_PITEN_bp; /* Enable: disabled */

    // 	RTC.PITDBGCTRL = 0 << RTC_DBGRUN_bp; /* Run in debug: disabled */

    // 	RTC.PITINTCTRL = 0 << RTC_PI_bp; /* Periodic Interrupt: disabled */

    // 	RTC.PITINTFLAGS = 0 << RTC_PI_bp; /* Periodic Interrupt: disabled */
    
    // 	RTC.TEMP = 0; /* Temporary */

    	RTC.CTRLA = RTC_PRESCALER_DIV32_gc /* Prescaling Factor: RTC Clock / 32 */
    				| 1 << RTC_RTCEN_bp /* Enable: enabled */
    				| 0 << RTC_RUNSTDBY_bp; /* Run In Standby: disabled */
	
    return 0;
}


ISR(RTC_PIT_vect)
{

    /* Insert your RTC PIT interrupt handling code here */

    /* TRIGB interrupt flag has to be cleared manually */
    RTC.PITINTFLAGS = RTC_PI_bm;
}

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

#ifndef RSTCTRL_INCLUDED
#define RSTCTRL_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "ccp.h"

static inline void RSTCTRL_reset(void)
{
	/* SWRR is protected with CCP */
//	ccp_write_io((void *)&RSTCTRL.SWRR, 0x1);
}

static inline uint8_t RSTCTRL_get_reset_cause(void)
{
	return RSTCTRL.RSTFR;
}

static inline void RSTCTRL_clear_reset_cause(void)
{
	RSTCTRL.RSTFR
	    = RSTCTRL_UPDIRF_bm | RSTCTRL_SWRF_bm | RSTCTRL_WDRF_bm | RSTCTRL_EXTRF_bm | RSTCTRL_BORF_bm | RSTCTRL_PORF_bm;
}

#ifdef __cplusplus
}
#endif

#endif /* RSTCTRL_INCLUDED */
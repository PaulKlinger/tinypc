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

#ifndef UTILS_COMPILER_H
#define UTILS_COMPILER_H

/**
 * \defgroup doc_driver_utils_compiler Compiler abstraction
 * \ingroup doc_driver_utils
 *
 * Compiler abstraction layer and code utilities for 8-bit AVR.
 * This module provides various abstraction layers and utilities
 * to make code compatible between different compilers.
 *
 * \{
 */

#if defined(__GNUC__)
#include <avr/io.h>
#include <avr/builtins.h>
#elif defined(__ICCAVR__)
#define ENABLE_BIT_DEFINITIONS 1
#include <ioavr.h>
#include <intrinsics.h>

#ifndef CCP_IOREG_gc
#define CCP_IOREG_gc 0xD8 /* CPU_CCP_IOREG_gc */
#endif
#ifndef CCP_SPM_gc
#define CCP_SPM_gc 0x9D /* CPU_CCP_SPM_gc */
#endif

#else
#error Unsupported compiler.
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "interrupt_avr8.h"

/**
 * \def UNUSED
 * \brief Marking \a v as a unused parameter or value.
 */
#define UNUSED(v) (void)(v)

#endif /* UTILS_COMPILER_H */

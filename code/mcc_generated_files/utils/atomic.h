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

#ifndef ATOMIC_H
#define ATOMIC_H

/**
 * \defgroup doc_driver_utils_atomic Atomic memory access and critical sections
 * \ingroup doc_driver_utils
 *
 * Atomic memory access and critical sections
 *
 * \{
 */

/* clang-format off */

#if defined(__GNUC__) || defined (__DOXYGEN__)

/**
 * \brief Enter a critical region
 * 
 * Saves the contents of the status register, including the Global 
 * Interrupt Enable bit, so that it can be restored upon leaving the 
 * critical region. Thereafter, clears the Global Interrupt Enable Bit.
 * This macro takes a parameter P that is unused for the GCC compiler,
 * but necessary for code compatibility with the IAR compiler. The IAR
 * compiler declares a variable with the name of the parameter for
 * holding the SREG value. Since a variable is declared in the macro,
 * this variable must have a name that is unique within the scope
 * that the critical region is declared within, otherwise compilation 
 * will fail.
 *
 * \param[in] UNUSED(GCC)/P(IAR) Name of variable storing SREG
 *
 */

#define ENTER_CRITICAL(UNUSED) __asm__ __volatile__ (   \
   "in __tmp_reg__, __SREG__"                    "\n\t" \
   "cli"                                         "\n\t" \
   "push __tmp_reg__"                            "\n\t" \
   ::: "memory"                                         \
   )

/**
 * \brief Exit a critical region
 * 
 * Restores the contents of the status register, including the Global 
 * Interrupt Enable bit, as it was when entering the critical region.
 * This macro takes a parameter P that is unused for the GCC compiler,
 * but necessary for code compatibility with the IAR compiler. The IAR
 * compiler uses this parameter as the name of a variable that holds 
 * the SREG value. The parameter must be identical to the parameter 
 * used in the corresponding ENTER_CRITICAL().
 *
 * \param[in] UNUSED(GCC)/P(IAR) Name of variable storing SREG
 *
 */

#define EXIT_CRITICAL(UNUSED)  __asm__ __volatile__ (   \
   "pop __tmp_reg__"                             "\n\t" \
   "out __SREG__, __tmp_reg__"                   "\n\t" \
   ::: "memory"                                         \
   )

#define DISABLE_INTERRUPTS()        __asm__ __volatile__ ( "cli" ::: "memory")
#define ENABLE_INTERRUPTS()         __asm__ __volatile__ ( "sei" ::: "memory")

#elif defined(__ICCAVR__)

#define ENTER_CRITICAL(P)  unsigned char P = __save_interrupt();__disable_interrupt();
#define EXIT_CRITICAL(P)  __restore_interrupt(P);

#define DISABLE_INTERRUPTS()   __disable_interrupt();
#define ENABLE_INTERRUPTS()    __enable_interrupt();

#else
#  error Unsupported compiler.
#endif

/* clang-format on */

#endif /* ATOMIC_H */

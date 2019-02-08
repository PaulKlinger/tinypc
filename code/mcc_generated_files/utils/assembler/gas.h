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

#ifndef ASSEMBLER_GAS_H_INCLUDED
#define ASSEMBLER_GAS_H_INCLUDED

#ifndef __DOXYGEN__

/* clang-format off */

        /* IAR doesn't accept dots in macro names */
        .macro  ld_addr, reg, sym
        lda.w   \reg, \sym
        .endm

        /* Define a function \a name that is either globally visible or only
         * file-local.
         */
        .macro gas_begin_func name, is_public
                .if \is_public
                .global \name
                .endif
                .section .text.\name, "ax", @progbits
                .type \name, @function
        \name :
        .endm

        /* Define a function \a name that is either globally visible or only
         * file-local in a given segment.
         */
        .macro gas_begin_func_segm name, is_public, segment
                .if \is_public
                .global \name
                .endif
                .section .\segment, "ax", @progbits
                .type \name, @function
        \name :
        .endm

        /* Define \a name as a weak alias for the function \a strong_name */
        .macro gas_weak_function_alias name, strong_name
        .global \name
        .weak   \name
        .type   \name, @function
        .set    \name, \strong_name
        .endm

        /* Define a weak function called \a name */
        .macro gas_weak_function name
        .weak   \name
        gas_begin_func \name 1
        .endm

#define REPEAT(count)           .rept   count
#define END_REPEAT()            .endr
#define FILL_BYTES(count)       .fill   count
#define SET_LOC(offset)         .org    offset
#define L(name)                 .L##name
#define EXTERN_SYMBOL(name)

#define TEXT_SECTION(name)                              \
        .section name, "ax", @progbits
#define RODATA_SECTION(name)                            \
        .section name, "a", @progbits
#define DATA_SECTION(name)                              \
        .section name, "aw", @progbits
#define BSS_SECTION(name)                               \
        .section name, "aw", @nobits

#define FUNCTION(name) gas_begin_func name 0
#define PUBLIC_FUNCTION(name)   gas_begin_func name 1
#define PUBLIC_FUNCTION_SEGMENT(name, segment)          \
        gas_begin_func_segm name 1 segment
#define WEAK_FUNCTION(name) gas_weak_function name
#define WEAK_FUNCTION_ALIAS(name, strong_name) \
        gas_weak_function_alias name strong_name
#define END_FUNC(name)                                  \
        .size   name, . - name

#define END_FILE()

/* clang-format on */

#endif /* __DOXYGEN__ */

#endif /* ASSEMBLER_GAS_H_INCLUDED */

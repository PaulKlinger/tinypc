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

#ifndef ASSEMBLER_IAR_H_INCLUDED
#define ASSEMBLER_IAR_H_INCLUDED

/* clang-format off */

ld_addr MACRO   reg, sym
	mov     reg, LWRD sym
	orh     reg, HWRD sym
	ENDM

call    MACRO   sym
	rcall   sym
	ENDM

iar_begin_func  MACRO   name, sect, is_public, is_weak
	MODULE  name
	RSEG    CODE:CODE:NOROOT(1)
	IF      is_weak == 1
	PUBWEAK name
	ELSEIF  is_public
	PUBLIC  name
	ENDIF
name:
	ENDM

iar_begin_func_segm  MACRO   name, sect, is_public, is_weak, segment
	MODULE  name
	RSEG    segment:CODE:NOROOT(1)
	IF      is_weak == 1
	PUBWEAK name
	ELSEIF  is_public
	PUBLIC  name
	ENDIF
name:
	ENDM

iar_weak_alias  MACRO   name, strong_name
	PUBWEAK name
name:
	rjmp    strong_name
	ENDM

#define lo(x)   LWRD x
#define hi(x)   HWRD x

#define REPEAT(count)           REPT    count
#define END_REPEAT()            ENDR
#define SET_LOC(offset)         ORG     offset
#define END_FILE()              END

#define FILL_BYTES(count)       DS8     count

#define L(name)                 name
#define EXTERN_SYMBOL(name)             EXTERN  name
#define FUNCTION(name)          iar_begin_func name, text_##name, 0, 0
#define PUBLIC_FUNCTION(name)   iar_begin_func name, text_##name, 1, 0
#define PUBLIC_FUNCTION_SEGMENT(name, segment)          \
		iar_begin_func_segm name, text_##name, 1, 0, segment
#define WEAK_FUNCTION(name)     iar_begin_func name, text_##name, 1, 1
#define WEAK_FUNCTION_ALIAS(name, strong_name)          \
		iar_weak_alias  name, strong_name
#define END_FUNC(name)          ENDMOD

#define TEXT_SECTION(name)      RSEG    name:CODE:NOROOT
#define RODATA_SECTION(name)    RSEG    name:CONST:NOROOT
#define DATA_SECTION(name)      RSEG    name:DATA:NOROOT
#define BSS_SECTION(name)       RSEG    name:DATA:NOROOT

/* clang-format on */

#endif /* ASSEMBLER_IAR_H_INCLUDED */

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

/**
 * \defgroup doc_driver_system_protected_io Protected IO
 * \ingroup doc_driver_system
 *
 * \section doc_driver_protected_io_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */

#ifndef PROTECTED_IO_H
#define PROTECTED_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__DOXYGEN__)
//! \name IAR Memory Model defines.
//@{

/**
 * \def CONFIG_MEMORY_MODEL_TINY
 * \brief Configuration symbol to enable 8 bit pointers.
 *
 */
#define CONFIG_MEMORY_MODEL_TINY

/**
 * \def CONFIG_MEMORY_MODEL_SMALL
 * \brief Configuration symbol to enable 16 bit pointers.
 * \note If no memory model is defined, SMALL is default.
 *
 */
#define CONFIG_MEMORY_MODEL_SMALL

/**
 * \def CONFIG_MEMORY_MODEL_LARGE
 * \brief Configuration symbol to enable 24 bit pointers.
 *
 */
#define CONFIG_MEMORY_MODEL_LARGE

//@}
#endif

/**
 * \brief Write to am 8-bit I/O register protected by CCP or a protection bit
 *
 * \param addr Address of the I/O register
 * \param magic CCP magic value or Mask for protection bit
 * \param value Value to be written
 *
 * \note Using IAR Embedded workbench, the choice of memory model has an impact
 *       on calling convention. The memory model is not visible to the
 *       preprocessor, so it must be defined in the Assembler preprocessor directives.
 */
extern void protected_write_io(void *addr, uint8_t magic, uint8_t value);

/** @} */

#endif /* PROTECTED_IO_H */

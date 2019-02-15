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
#ifndef PINS_H_INCLUDED
#define PINS_H_INCLUDED

#include "port.h"


/**
 * \brief Get level on IO_PA2
 *
 * Reads the level on a pin
 */
static inline bool IO_PA2_get_level()
{
	return PORTA_get_pin_level(2);
}

/**
 * \brief Set IO_PB2 level
 *
 * Sets output level on a pin
 *
 * \param[in] level true  = Pin level set to "high" state
 *                  false = Pin level set to "low" state
 */
static inline void IO_PB2_set_level(const bool level)
{
	PORTB_set_pin_level(2, level);
}

/**
 * \brief Get level on IO_PB2
 *
 * Reads the level on a pin
 */
static inline bool IO_PB2_get_level()
{
	return PORTB_get_pin_level(2);
}

/**
 * \brief Get level on IO_PA1
 *
 * Reads the level on a pin
 */
static inline bool IO_PA1_get_level()
{
	return PORTA_get_pin_level(1);
}

/**
 * \brief Get level on IO_PA4
 *
 * Reads the level on a pin
 */
static inline bool IO_PA4_get_level()
{
	return PORTA_get_pin_level(4);
}

/**
 * \brief Get level on IO_PA3
 *
 * Reads the level on a pin
 */
static inline bool IO_PA3_get_level()
{
	return PORTA_get_pin_level(3);
}

/**
 * \brief Get level on IO_PA5
 *
 * Reads the level on a pin
 */
static inline bool IO_PA5_get_level()
{
	return PORTA_get_pin_level(5);
}

/**
 * \brief Get level on IO_PB1
 *
 * Reads the level on a pin
 */
static inline bool IO_PB1_get_level()
{
	return PORTB_get_pin_level(1);
}

/**
 * \brief Get level on IO_PB0
 *
 * Reads the level on a pin
 */
static inline bool IO_PB0_get_level()
{
	return PORTB_get_pin_level(0);
}

void PIN_MANAGER_Initialize();
#endif /* PINS_H_INCLUDED */

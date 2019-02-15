/* Simple I2C code by fgras78
 * from https://www.avrfreaks.net/comment/2568566#comment-2568566
 */

#include "mcc_generated_files/config/clock_config.h"
#define TWI0_BAUD(F_SCL)      ((((float)F_CPU / (float)F_SCL)) - 10 )

void TWI_init()
{
	TWI0.MBAUD = (uint8_t)TWI0_BAUD(800000);	        // set MBAUD register for 800kHz
	TWI0.MCTRLA = 1 << TWI_ENABLE_bp			/* Enable TWI Master: enabled */
	| 0 << TWI_QCEN_bp					/* Quick Command Enable: disabled */
	| 0 << TWI_RIEN_bp					/* Read Interrupt Enable: disabled */
	| 1 << TWI_SMEN_bp					/* Smart Mode Enable: enabled */
	| TWI_TIMEOUT_DISABLED_gc				/* Bus Timeout Disabled */
	| 0 << TWI_WIEN_bp;					/* Write Interrupt Enable: disabled */

	TWI0.MCTRLB |= TWI_FLUSH_bm ;				/* Purge MADDR and MDATA */
	TWI0.MSTATUS |= TWI_BUSSTATE_IDLE_gc ;		        //Force TWI state machine into IDLE state
	TWI0.MSTATUS |= (TWI_RIF_bm | TWI_WIF_bm) ;
}

uint8_t TWI_start(uint8_t deviceAddr)
{
	if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_BUSY_gc)		//Verify Bus is not busy
	{
		TWI0.MCTRLB &= ~(1 << TWI_ACKACT_bp);
		TWI0.MADDR = deviceAddr ;
		if (deviceAddr&0x01)	{while(!(TWI0.MSTATUS & TWI_RIF_bm));}  //si addressRead
		else			{while(!(TWI0.MSTATUS & TWI_WIF_bm));}  //si addressWrite
		return 0;
	}
	else return 1;	                                                        //Bus is busy
}

uint8_t TWI_read(uint8_t ACK)							// ACK=1 send ACK ; ACK=0 send NACK
{

	if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) == TWI_BUSSTATE_OWNER_gc)		//Verify Master owns the bus
	{
		while(!(TWI0.MSTATUS & TWI_RIF_bm));				// Wait until RIF set
		uint8_t data=TWI0.MDATA;
		if	(ACK==1)	{TWI0.MCTRLB &= ~(1<<TWI_ACKACT_bp);}		// si ACK=1 mise à 0 ACKACT => action send ack
		else			{TWI0.MCTRLB |= (1<<TWI_ACKACT_bp);	}	// sinon (ACK=0) => mise à 1 ACKACT => nack préparé pour actionstop

		return data ;
	}
	else
	return 1;	//Master does not own the bus

}
uint8_t TWI_WRITE(uint8_t write_data)
{
	if ((TWI0.MSTATUS&TWI_BUSSTATE_gm) == TWI_BUSSTATE_OWNER_gc)			                //Verify Master owns the bus
	{
		TWI0.MDATA = write_data;
		while (!((TWI0.MSTATUS & TWI_WIF_bm) | (TWI0.MSTATUS & TWI_RXACK_bm))) ;		//Wait until WIF set and RXACK cleared
		return 0;
	}
	else
	return 1;	//Master does not own the bus
}

void TWI_STOP(void)
{
	TWI0.MCTRLB |= TWI_ACKACT_NACK_gc;
	TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
}

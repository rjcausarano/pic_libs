/* 
 * File:   i2c.h
 * Author: rodrigo
 *
 * Created on April 4, 2021, 7:58 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include<xc.h>

#define I2C_START SSPSTATbits.S
#define I2C_STOP SSPSTATbits.P
#define I2C_READ SSPSTATbits.R_nW
#define I2C_DATA SSPSTATbits.D_nA
#define SSPIF SSP1IF
#define SSPIE SSP1IE
       
void setup_i2c(char master, char address, 
        void (* on_byte_write)(char offset, char byte),
        void (* on_byte_read)(char offset));
void process_interrupt_i2c(void);
void write_byte_i2c(char data);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */


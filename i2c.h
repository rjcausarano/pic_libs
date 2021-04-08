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
    
void setup_i2c(char master, char address, void (* on_byte_received)(char byte));
void process_interrupt_i2c();
void write_byte_i2c(char data);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */


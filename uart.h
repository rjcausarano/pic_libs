/* 
 * File:   uart.h
 * Author: rodriC
 *
 * Created on October 9, 2019, 1:31 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include<xc.h>
    
void setup_uart(unsigned char high_speed, char spbrg);
char data_received_uart();
char data_transmited_uart();
void write_uart(char*);
char read_uart();

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */


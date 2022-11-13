/* 
 * File:   i2c.h
 * Author: rodrigo
 *
 * Created on April 4, 2021, 7:58 PM
 */

#ifndef I2C_H
#define	I2C_H
    
#include<xc.h>

#define I2C_START SSPSTATbits.S
#define I2C_STOP SSPSTATbits.P
#define I2C_READ SSPSTATbits.R_nW
#define I2C_DATA SSPSTATbits.D_nA
#define SSPIF SSP1IF
#define SSPIE SSP1IE
       
static void (* on_byte_write_)(char offset, char byte) = 0;
static char (* on_byte_read_)(char offset) = 0;
static char byte_index_ = 0;
static char selected_offset_ = 0;

static void map_pins(){
    // set RC4 as SCL
    SSPCLKPPS = 0b10100;
    RC4PPS = 0b10000;
    // set RC3 as SDA
    SSPDATPPS = 0b10011;
    RC3PPS = 0b10001;
}

static void pins_setup(){
    // RC3 and RC4 are digital pins
    ANSC3 = 0;
    ANSC4 = 0;
    // RC3 and RC4 are inputs
    TRISC4 = 1;
    TRISC3 = 1;
}

void setup_i2c(char master, char address, 
        void (* on_byte_write)(char offset, char byte),
        char (* on_byte_read)(char offset)){
    map_pins();
    on_byte_write_ = on_byte_write;
    on_byte_read_ = on_byte_read;
    if(master){
        // 7 bit address
        /*
         * TODO: Finish off master configuration, should RC3 and RC4 be outputs?
        SSPM3 = 0;
        SSPM2 = 1;
        SSPM1 = 1;
        SSPM0 = 0;
        */
    } else { // configured as slave
        pins_setup();
        // set the slave address
        SSPADD = (char) (address << 1);
        // 100 khz
        SMP = 1;
        // 7 bit address, no start and stop bit interrupts
        SSPM3 = 0;
        SSPM2 = 1;
        SSPM1 = 1;
        SSPM0 = 0;
        // disable general call
        GCEN = 0;
        // clock stretching only on transmit
        SEN = 0;
        // ensure no overflow condition
        SSPOV = 0;
    }
    // Enable interrupts
    GIE = 1;
    PEIE = 1;
    SSPIE = 1;
    SSPIF = 0;
    // Enable i2c
    SSPEN = 1;
    // Release the clock
    CKP = 1;
}

static void write_byte_i2c(char data){
    SSPBUF = data;
    CKP = 1;
}

static char is_byte_address(){
    return !I2C_DATA;
}

static char is_read_instruction(){
    return I2C_READ;
}

static char is_write_instruction(){
    return !I2C_READ;
}

char stop_bit_detected(){
    return I2C_STOP;
}

static void on_byte_received(char byte){
    // byte_index 0 is the offset
    if(byte_index_ == 0){
        selected_offset_ = byte;
    }
    else {
        on_byte_write_(selected_offset_ + byte_index_ - 1, byte);
    }
    byte_index_++;
}

void process_interrupt_i2c(){
    SSPIF = 0;
    
    char byte = SSPBUF;
    if(is_byte_address()){
        if(is_read_instruction()){
            // we need to send data
            write_byte_i2c(on_byte_read_(selected_offset_));
        }
        // reset receive buffer after receiving address
        byte_index_ = 0;
    } else{
        if(is_write_instruction()){
            // we received data
            on_byte_received(byte);
        } else{
            // increase offset by one
            selected_offset_++;
            write_byte_i2c(on_byte_read_(selected_offset_));
        }
    }
}

#endif	/* I2C_H */


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
       
static void (* on_write_data_)(char offset, char data[]) = 0;
static void (* on_read_data_)(char offset, char data[]) = 0;
// Can receive up to 5 bytes of data in a transaction. Increase this as needed.
// index 0 is the offset address
static char transmitted_bytes_[5] = {0};
static char bytes_buffer_index_ = 0;

//static void map_pins(){
//    // set RC4 as SCL
//    SSPCLKPPS = 0b10100;
//    RC4PPS = 0b10000;
//    // set RC3 as SDA
//    SSPDATPPS = 0b10011;
//    RC3PPS = 0b10001;
//}

static void pins_setup(){
    // RB4 and RB6 are digital pins
    char change_mask = 0b01010000;
    ANSELB = (ANSELB & ~change_mask) | 0b00000000;
    // RB4 is SDA
    TRISB4 = 1;
    // RB6 is SCL
    TRISB6 = 1;
    // Disable pull ups
    WPUB4 = 0;
    WPUB6 = 0;
}

void set_transaction_callbacks_i2c(void (* on_begin)(void), 
        void (* on_end)(void));

void setup_i2c(char master, char address, 
        void (* on_write_data)(char offset, char data[]),
        void (* on_read_data)(char offset, char data[])){
//    map_pins();
    on_write_data_ = on_write_data;
    on_read_data_ = on_read_data;
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
        SSP1CON1bits.SSPM = 0b0110;
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
    // byte_index 0 is the address offset
    transmitted_bytes_[bytes_buffer_index_] = byte;
    bytes_buffer_index_++;
}

void process_interrupt_i2c(){
    SSPIF = 0;
    
    char byte = SSPBUF;
    if(is_byte_address()){
        // reset receive buffer after receiving address
        bytes_buffer_index_ = 0;
        if(is_read_instruction()){
            // we will send first byte of data
            on_read_data_(transmitted_bytes_[0], transmitted_bytes_ + 1);
            bytes_buffer_index_++;
            write_byte_i2c(transmitted_bytes_[bytes_buffer_index_]);
        }
    } else{
        if(is_write_instruction()){
            // we received data
            on_byte_received(byte);
            if(stop_bit_detected() && bytes_buffer_index_ > 1){
                on_write_data_(transmitted_bytes_[0], transmitted_bytes_ + 1);
            }
        } else{ // It is a read instruction
            bytes_buffer_index_++;
            write_byte_i2c(transmitted_bytes_[bytes_buffer_index_]);
        }
    }
}

#endif	/* I2C_H */


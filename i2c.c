#include "i2c.h"

char address_received_ = 0;
char data_received_[10] = {0};
char data_received_index_ = 0;

void setup_i2c(char master, char address){
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
        // RC3 and RC4 are inputs
        TRISC3 = 1;
        TRISC4 = 1;
        // set the slave address
        SSPADD = address << 1;
        // 100 khz
        SMP = 1;
        
        // 7 bit address, no start and stop bit interrupts
        SSPM3 = 0;
        SSPM2 = 1;
        SSPM1 = 1;
        SSPM0 = 0;
        /*
        // 7 bit address, with start and stop bit interrupts
        SSPM3 = 1;
        SSPM2 = 1;
        SSPM1 = 1;
        SSPM0 = 0;
        */
        
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

void write_byte_i2c(char data){
    SSPBUF = data;
    CKP = 1;
}

char is_byte_data(){
    return I2C_DATA;
}

char is_byte_address(){
    return !is_byte_data();
}

char is_read_instruction(){
    return I2C_READ;
}
char stop_bit_detected(){
    return I2C_STOP;
}

char start_bit_detected(){
    return I2C_START;
}

char is_buffer_full(){
    return BF;
}

char is_colliding(){
    return WCOL;
}

char is_receive_overflow(){
    return SSPOV;
}

char process_interrupt_i2c(){
    SSPIF = 0;
    
    /*if(!is_buffer_full()){
        // no data yet, just return
        return 0;
    }*/
    char byte = SSPBUF;
    if(is_byte_address()){
        address_received_ = byte;
        data_received_index_ = 0;
    } else{ 
        // we received data
        data_received_[data_received_index_] = byte;
        data_received_index_++;
    }
    return 0;
}
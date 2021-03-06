#include "i2c.h"

static char address_received_ = 0;
static void (* on_byte_write_)(char offset, char byte) = 0;
static char (* on_byte_read_)(char offset) = 0;
static void (* on_begin_)(void) = 0;
static void (* on_end_)(void) = 0;
// Can receive up to 3 bytes in a transaction. Increase this as needed.
static char bytes_received_[3] = {0};
static char byte_index_ = 0, in_transaction_ = 0;

static void map_pins(){
    // set SCL
    SSPCLKPPS = 0b10010;
    RC2PPS = 0b10000;
    // set SDA
    SSPDATPPS = 0b10001;
    RC1PPS = 0b10001;
}

static void pins_setup(){
    // RC1 and RC2 are inputs
    TRISC1 = 1;
    TRISC2 = 1;
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

void set_transaction_callbacks_i2c(void (* on_begin)(void), 
        void (* on_end)(void)){
    on_begin_ = on_begin;
    on_end_ = on_end;
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
    bytes_received_[byte_index_] = byte;
    // byte_index 0 is the offset
    if(byte_index_ > 0){
        on_byte_write_(bytes_received_[0] + byte_index_ - 1, 
                bytes_received_[byte_index_]);
    }
    byte_index_++;
}

void process_interrupt_i2c(){
    SSPIF = 0;
    
    char byte = SSPBUF;
    if(is_byte_address()){
        if(!in_transaction_ && on_begin_ != 0){
            in_transaction_ = 1;
            on_begin_();
        }
        address_received_ = byte;
        if(is_read_instruction()){
            // we need to send data
            write_byte_i2c(on_byte_read_(bytes_received_[0]));
        }
        // reset receive buffer after receiving address
        byte_index_ = 0;
    } else{
        if(is_write_instruction()){
            // we received data
            on_byte_received(byte);
        } else{
            bytes_received_[0]++;
            write_byte_i2c(on_byte_read_(bytes_received_[0]));
        }
    }
    
    if(stop_bit_detected() && on_end_ != 0){
        in_transaction_ = 0;
        on_end_();
    }
}
#include <pic16f1705.h>

#include "speed.h"

// 720 counts per wheel revolution

static void (* on_speed_interrupt_)(char hi, char lo);

static void map_pins(){
    // RC0 to capture channel A (CCP2)
    CCP2PPS = 0b10000;
}

static void pins_setup(){
    // RC0 is an input
    TRISC0 = 1;
    // RA2 is an input
    TRISA2 = 1;
}

void setup_speed(void (* on_speed_interrupt)(char hi, char lo)){
    on_speed_interrupt_ = on_speed_interrupt;
    pins_setup();
    map_pins();
    // capture on every 16th rising edge
    /*CCP2M3 = 0;
    CCP2M2 = 1;
    CCP2M1 = 1;
    CCP2M0 = 1;*/
    CCP2CONbits.CCP2M = 0b0111;
    
    //Interrupts
    GIE = 1;
    PEIE = 1;
    TMR1IE = 0;
    CCP2IE = 1;
    CCP2IF = 0;
    
    // TMR1 clock source
    T1CONbits.TMR1CS = 0b00; // Increment on every FOSC/4
    //Turn on TMR1
    TMR1ON = 1;
}

void process_interrupts_speed(){
    CCP2IF = 0;
    char hi_byte = CCPR2H;
    char lo_byte = CCPR2L;
    on_speed_interrupt_(hi_byte, lo_byte);
}
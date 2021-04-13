#include "pwm.h"
#include "byte_macros.h"

static unsigned int pwm_period_us_ = 0;
static char freq_Mhz_ = 0;

static void map_pins(){
    // RA4 is PWM output
    RA4PPS = 0b01100; // RA4 mapped to CCP1
}

static void pins_setup(){
    TRISA4 = 0; // CCP1 as output
}

void set_duty_cycle_pwm(unsigned int duty){
    duty = duty << 6;
    unsigned char low_byte = LOWBYTE(duty);
    CCPR1L = HIGHBYTE(duty);
    CCP1X = 0x01 & (low_byte >> 7);
    CCP1Y = 0x01 & (low_byte >> 6);
}

void set_duty_percent_pwm(unsigned char duty_p){
    unsigned int duty = duty_p * (__uint24) 1023 / 100;
    set_duty_cycle_pwm(duty);
}

void set_prescaler_pwm(char prescaler){
    //    11 = Prescaler is 64
    //    10 = Prescaler is 16
    //    01 = Prescaler is 4
    //    00 = Prescaler is 1
    
    // only apply if prescaler is one of the allowed values
    if(prescaler <= 0b11){
        T2CONbits.T2CKPS = prescaler;
    }
}

char get_prescaler(){
    char prescaler = 0;
    switch(T2CONbits.T2CKPS){
        case 0b00:
            prescaler = 1;
            break;
        case 0b01:
            prescaler = 4;
            break;
        case 0b10:
            prescaler = 16;
            break;
        case 0b11:
            prescaler = 64;
            break;
    }
    return prescaler;
}

void calculate_period(){
    //unsigned int pr_2 = PR2 + 1;
    pwm_period_us_ = (unsigned int) 4 * (PR2 + 1) * get_prescaler() / 
            (unsigned int) freq_Mhz_;
}

unsigned int get_period_us_pwm(){
    return pwm_period_us_;
}

void setup_pwm(char freq_Mhz){
    freq_Mhz_ = freq_Mhz;
    PR2 = 0xFF; // set the frequency
    set_duty_cycle_pwm(0);
    // TMR2 prescaler set to 1
    set_prescaler_pwm(0b00);
    // Configure CCP1 for PWM
    CCP1M3 = 1;
    CCP1M2 = 1;
    pins_setup();
    map_pins();
    TMR2IF = 0;
    TMR2ON = 1; // turn on timer 2
    calculate_period();
}

void period_interrupt_pwm(char enable){
    TMR2IE = (__bit) enable;
}
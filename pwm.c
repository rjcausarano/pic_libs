#include "pwm.h"

static void map_pins(){
    // RA4 is PWM output
    RA4PPS = 0b01100; // RA4 mapped to CCP1
}

static void pins_setup(){
    TRISA4 = 0; // CCP1 as output
}

void set_duty_cycle(unsigned int duty){
    duty = duty << 6;
    unsigned char low_byte = LOWBYTE(duty);
    CCPR1L = HIGHBYTE(duty);
    CCP1X = 0x01 & (low_byte >> 7);
    CCP1Y = 0x01 & (low_byte >> 6);
}

void set_duty_percent(unsigned char duty_p){
    unsigned int duty = duty_p * (__uint24) 1023 / 100;
    set_duty_cycle(duty);
}

void setup_pwm(){
    PR2 = 255; // set the frequency
    set_duty_cycle(0);
    // TMR2 prescaler set to 1
    T2CKPS1 = 0;
    T2CKPS0 = 0;
    // Configure CCP1 for PWM
    CCP1M3 = 1;
    CCP1M2 = 1;
    pins_setup();
    map_pins();
    TMR2IF = 0;
    TMR2ON = 1; // turn on timer 2
}
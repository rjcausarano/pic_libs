#include "pwm.h"

void set_duty(unsigned int duty){
    duty = duty << 6;
    unsigned char low_byte = LOWBYTE(duty);
    CCPR1L = HIGHBYTE(duty);
    CCP1X = 0x01 & (low_byte >> 7);
    CCP1Y = 0x01 & (low_byte >> 6);
}

void set_duty_percent(unsigned char duty_p){
    unsigned int duty = duty_p * (__uint24) 1023 / 100;
    set_duty(duty);
}

void setup_pwm(){
    PR2 = 255; // set the frequency
    set_duty(0);
    // TMR2 prescaler set to 1
    T2CKPS1 = 0;
    T2CKPS0 = 0;
    // Configure CCP1 for PWM
    CCP1M3 = 1;
    CCP1M2 = 1;
    TRISC2 = 0; // CCP1 as output
    TMR2ON = 1; // turn on timer 2
}
/* 
 * File:   pwm.h
 * Author: rodrigo
 *
 * Created on March 21, 2021, 12:51 PM
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include<xc.h>
    
void set_duty_cycle_pwm(unsigned int duty);
void set_duty_percent_pwm(unsigned char duty_p);
void set_prescaler_pwm(char prescaler);
unsigned int get_period_us_pwm();
void setup_pwm(char freq_Mhz);
void period_interrupt_pwm(char enable);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */


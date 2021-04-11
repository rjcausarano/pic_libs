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
#include "byte_macros.h"
    
void set_duty_cycle(unsigned int duty);
void set_duty_percent(unsigned char duty_p);
void setup_pwm(void);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */


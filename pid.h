/* 
 * File:   pid.h
 * Author: rodrigo
 *
 * Created on May 3, 2021, 2:54 PM
 */

#ifndef PID_H
#define	PID_H

#ifdef	__cplusplus
extern "C" {
#endif

int do_pid(int current, int set_point);

#ifdef	__cplusplus
}
#endif

#endif	/* PID_H */


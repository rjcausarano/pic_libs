/* 
 * File:   byte_macros.h
 * Author: rodrigo
 *
 * Created on March 21, 2021, 12:49 PM
 */

#ifndef BYTE_MACROS_H
#define	BYTE_MACROS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LOWBYTE(v)   ((unsigned char) (v))
#define HIGHBYTE(v)  ((unsigned char) (((unsigned int) (v)) >> 8))


#ifdef	__cplusplus
}
#endif

#endif	/* BYTE_MACROS_H */


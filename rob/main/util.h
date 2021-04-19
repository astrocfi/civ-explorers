/*******************************************************************************
 * Author: Robert S. French <rfrench@rfrench.org>
 * Derived from work by John Rowling
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative
 * Commons, PO Box 1866, Mountain View, CA 94042, USA.
 ******************************************************************************/

#ifndef __UTIL_H__
#define __UTIL_H__

#include <Arduino.h>

void printDigits(int digits);
byte token_2_mode(char mode_token);
char mode_2_token(byte icom_mode);
byte high_nibble_unpacker(byte high_target);
byte low_nibble_unpacker(byte low_target);
void bit_bash(byte control);

void util_init();
void host_print();
void host_print(unsigned long x, int format=DEC);
void host_print(const char *s);
void host_println();
void host_println(unsigned long x, int format=DEC);
void host_println(const char *s);

#endif __UTIL_H__

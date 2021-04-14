#ifndef __UTIL_H__
#define __UTIL_H__

void printDigits(int digits);
byte token_2_mode(char mode_token);
char mode_2_token(byte icom_mode);
byte high_nibble_unpacker(byte high_target);
byte low_nibble_unpacker(byte low_target);
void bit_bash(byte control);

#endif __UTIL_H__

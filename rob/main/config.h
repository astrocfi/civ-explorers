/*******************************************************************************
 * Author: Robert S. French <rfrench@rfrench.org>
 * Derived from work by John Rowling
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative
 * Commons, PO Box 1866, Mountain View, CA 94042, USA.
 ******************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define REV_STRING "RGB16_SMeter_V0.001"
// #define REV_STRING "0123456789"

// #define MEGA2560
#define PRO_MINI 1

// #define LEDARC_NONE
// #define LEDARC_8_RGB
#define LEDARC_16_RGB

#define LOOP_DELAY  1

#define RADIO_8600  0x96
#define RADIO_7300  0x94
#define RADIO       0x00

#define HOST_SERIAL_PIN_TX     10
#define HOST_SERIAL_PIN_RX     11

#define LSB_LED_PIN 5
#define AM_LED_PIN  4
#define USB_LED_PIN 3
#define CW_LED_PIN  2

#endif /* __CONFIG_H__ */

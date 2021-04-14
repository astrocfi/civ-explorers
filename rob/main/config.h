#ifndef __CONFIG_H__
#define __CONFIG_H__

#define REV_STRING "RGB16_-_FBB_rev_02_or_03pcb_net-0_MEGorPRO_MAR_06_2021" //     <------

// #define MEGA2560
#define PRO_MINI 1

// #define LED_NONE
// #define LED_8_MONO
// #define LED_16_MONO
// #define LED_8_RGB
#define LED_16_RGB

#define LOOP_DELAY  1

#define CONTROLLER_ID   0xE0

#define RADIO_8600  0x96                                  //                <------
#define RADIO_7300  0x94
#define RADIO       0x00

#define CI_V_TX_pin     10
#define CI_V_RX_pin     11

#define  PWM_PIN    9
#define  STORE_PIN  7
#define  CLOCK_PIN  8
#define  DATA_PIN   6

#define LSB_LED_PIN 5
#define AM_LED_PIN  4
#define USB_LED_PIN 3
#define CW_LED_PIN  2

#endif /* __CONFIG_H__ */

/*******************************************************************************
 * The LEDArcBoad class
 *
 * This class defines the interface for the serial LED Arc Board. There can
 * be a single board with 8 LEDs, or two boards with 16 LEDs. The LEDs are
 * RGB. The basic functionality is to draw an arc of LEDs from the low
 * value to the high value. Optionally the moving average and/or peak value
 * can be displayed automatically.
 *
 * Author: Robert S. French <rfrench@rfrench.org>
 * Derived from work by John Rowling
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative
 * Commons, PO Box 1866, Mountain View, CA 94042, USA.
 ******************************************************************************/

#ifndef __LEDARC_ARC_BOARD_H__
#define __LEDARC_ARC_BOARD_H__

#include <Arduino.h>

#define LEDARC_PWM_PIN    9
#define LEDARC_STORE_PIN  7
#define LEDARC_CLOCK_PIN  8
#define LEDARC_DATA_PIN   6

#define LEDARC_DEFAULT_PWM 240 // 220

#define LEDARC_DEFAULT_ARC_SMOOTHING_SPEED 20 // ms

#define LEDARC_DEFAULT_MOVING_AVERAGE_SPEED 100 // ms
#define LEDARC_MAX_AVERAGE_HISTORY 20 // Samples @ given interval
#define LEDARC_DEFAULT_AVERAGE_HISTORY 20 // Samples @ given interval

#define LEDARC_DEFAULT_PEAK_TTL 4000 // ms

class LEDArcBoard
{
  public:
    // The id is purely for external use if you need to identify which instance
    // of the class you're looking at
    LEDArcBoard(byte id,
                bool enable_moving_average=true,
                bool enable_peak=true,
                bool smooth_arc=true);
    byte get_id();
    byte set_id(byte id);

    // Start operation
    void begin();

    // Update the LEDs with the current values
    // Until this is called nothing changes on the hardware
    void update_leds();

    void set_current_val(byte cur_val);
    byte get_current_val();

    void enable_arc_smoothing();
    void disable_arc_smoothing();
    void set_arc_smoothing_speed(unsigned int);
    unsigned int get_arc_smoothing_speed();

    void set_led_brightness(byte pwm);
    byte get_led_brightness();

    void enable_moving_average();
    void disable_moving_average();
    void set_moving_average_size(byte size);
    void set_moving_average_speed(unsigned int);
    unsigned int get_moving_average_speed();

    void enable_peak();
    void disable_peak();
    void set_peak_ttl(unsigned int ttl);

  private:
    byte _convert_val_to_index(byte val);

    byte _id; // Unique ID of this board
    byte _cur_val; // The most recently set value to display
    byte _displayed_index; // The index of the highest LED actually displayed
    bool _smooth_arc; // True if we want to smooth out changes in the arc
    unsigned int _arc_smoothing_speed; // Number of ms between arc changes
    unsigned long _last_led_update_time; // The time of the last arc update
    byte _brightness; // The PWM of the LEDs
    byte _last_brightness; // So we don't update the pin unnecessarily

    bool _ave_enabled; // True if we want to keep a moving average
    byte _ave_val_history[LEDARC_MAX_AVERAGE_HISTORY]; // Round robin buffer
    byte _ave_history_pos; // Position in buffer
    byte _ave_history_size; // How much of the buffer we actually want to use
    unsigned int _ave_speed; // Number of ms between updates to the buffer
    unsigned long _last_ave_time; // The time of the last buffer update

    bool _peak_enabled; // True if we want to have a maximum-value LED
    byte _peak_index; // The index of the peak LED
    unsigned int _peak_ttl; // How long the peak LED stays lit (ms)
    unsigned long _last_peak_update_time; // The time of the last update of the peak LED
};

#endif // __LEDARC_ARC_BOARD_H__

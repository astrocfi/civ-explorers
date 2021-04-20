/*******************************************************************************
 * The LEDArcBoad class
 *
 * This class defines the interface for the serial LED Arc Board. There can
 * be a single board with 8 LEDs, or two boards with 16 LEDs. The LEDs are
 * RGB. The basic functionality is to draw an arc of LEDs from the lowest
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

#define LEDARC_DEFAULT_PWM 240 // 0-255, higher is dimmer

// When moving the arc to a higher or lower value, optionally the arc can move
// one LED at a time to make it look smoother. If this feature is enabled, this
// is the delay between adjacent LEDs.
#define LEDARC_DEFAULT_ARC_SMOOTHING_SPEED 20 // ms

// If enabled, a moving average is calculated at a regular interval based on
// the current arc value. This is the delay between samples used to compute the
// moving average.
#define LEDARC_DEFAULT_MOVING_AVERAGE_SPEED 100 // ms
// The moving average is calculated by keeping samples in a round-robin buffer.
// This is the maximum and default number of samples to keep. The moving average
// is calculated over a time period of
// LEDARC_DEFAULT_MOVING_AVERAGE_SPEED * LEDARC_MAX_AVERAGE_HISTORY
#define LEDARC_MAX_AVERAGE_HISTORY 20 // Samples @ given interval
#define LEDARC_DEFAULT_AVERAGE_HISTORY 20 // Samples @ given interval

// If enabled, a "sticky" peak value is stored and displayed for the given
// amount of time. If the arc value increases, the peak value is set to the new
// value, but if the arc value decreases, the old peak value is maintained.
// After the time has elapsed, the peak value is forgotten and a new peak is
// stored.
#define LEDARC_DEFAULT_PEAK_TTL 4000 // ms

class LEDArcBoard
{
  public:
    // The id is purely for external use if you need to identify which instance
    // of the class you're looking at. It is not currently used for anything
    // else.
    // The bool arguments say whether or not to turn on the given feature.
    // These can be changed in the future with the methods below.
    LEDArcBoard(byte id,
                bool enable_moving_average=true,
                bool enable_peak=true,
                bool smooth_arc=true);
    byte get_id();
    byte set_id(byte id);

    // Initialize operation. Must be called before calling update_leds the
    // first time.
    void begin();

    // Update the LEDs with the current value (and moving average and peak).
    // Until this is called nothing changes on the hardware. This method
    // should be called as often as possible. If the proper time hasn't elapsed,
    // nothing will be done.
    void update_leds();

    // Set the current value to display on the arc. This value is in "source
    // units", which will be mapped later in some manner to the particular LED
    // positions.
    void set_current_val(int16_t cur_val);
    uint16_t get_current_val();

    // Change the LED brightness (0-255). Higher values are dimmer.
    void set_led_brightness(byte pwm);
    byte get_led_brightness();

    // Arc smoothing gradually adds/removes one LED at a time when the displayed
    // value changes.
    void enable_arc_smoothing();
    void disable_arc_smoothing();
    void set_arc_smoothing_speed(unsigned long);
    unsigned long get_arc_smoothing_speed();

    // The moving average calculates the average and displays it as a special
    // color. See comments above for details.
    void enable_moving_average();
    void disable_moving_average();
    void set_moving_average_size(uint16_t size);
    uint16_t get_moving_average_size();
    void set_moving_average_speed(unsigned long);
    unsigned long get_moving_average_speed();

    // The peak displays a "sticky" peak value which eventually disappears.
    // See comments above for details.
    void enable_peak();
    void disable_peak();
    void set_peak_ttl(unsigned long ttl);
    unsigned long get_peak_ttl();

  private:
    byte _convert_val_to_index(byte val);

    byte _id; // Unique ID of this board
    int16_t _cur_val; // The most recently set value to display
    byte _displayed_index; // The index of the highest LED actually displayed
    bool _smooth_arc; // True if we want to smooth out changes in the arc
    unsigned long _arc_smoothing_speed; // Number of ms between arc changes
    unsigned long _last_led_update_time; // The time of the last arc update
    byte _brightness; // The PWM of the LEDs
    byte _last_brightness; // So we don't update the pin unnecessarily

    bool _ave_enabled; // True if we want to keep a moving average
    byte _ave_val_history[LEDARC_MAX_AVERAGE_HISTORY]; // Round robin buffer
    uint16_t _ave_history_pos; // Position in buffer
    uint16_t _ave_history_size; // How much of the buffer we actually want to use
    unsigned long _ave_speed; // Number of ms between updates to the buffer
    unsigned long _last_ave_time; // The time of the last buffer update

    bool _peak_enabled; // True if we want to have a maximum-value LED
    byte _peak_index; // The index of the peak LED
    unsigned long _peak_ttl; // How long the peak LED stays lit (ms)
    unsigned long _last_peak_update_time; // The time of the last update of the peak LED
};

#endif // __LEDARC_ARC_BOARD_H__

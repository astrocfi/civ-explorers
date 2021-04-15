/*******************************************************************************
 * The LEDArcBoad class
 *
 * This class defines the interface for the serial LED Arc Board. There can
 * be a single board with 8 LEDs, or two boards with 16 LEDs. The LEDs are
 * RGB. The basic functionality is to draw an arc of LEDs from the low
 * value to the high value. Optionally the moving average and/or peak value
 * can be displayed automatically.
 ******************************************************************************/

#ifndef __LED_ARC_BOARD_H__
#define __LED_ARC_BOARD_H__

#define LED_8_ATTACK      0   // ms
#define LED_8_SUSTAIN     2   // ms
#define LED_8_DECAY       0   // ms

#define LED_16_ATTACK     0   // ms
#define LED_16_SUSTAIN    1   // ms
#define LED_16_DECAY      2   // ms

// How long the peak LED will be lit by default
#define LED_DEFAULT_PEAK_TTL 7

#define LED_MAX_AVERAGE_HISTORY 20
#define LED_DEFAULT_AVERAGE_HISTORY 20

class LEDArcBoard
{
  public:
    // The id is purely for external use if you need to identify which instance
    // of the class you're looking at
    LEDArcBoard(byte id,
                bool enable_moving_average=false,
                bool enable_peak=false);
    byte get_id();
    byte set_id(byte id);

    // Start operation
    void begin();

    // Update the LEDs with the current values
    void update_leds();

    // Set and retrieve the values that define which LEDs will be lit
    // XXX Describe them
    void set_current_val(byte cur_val);
    byte get_current_val();
    void set_low_val(byte low_val);
    byte get_low_val();
    void set_high_val(byte high_val);
    byte get_high_val();

    void enable_moving_average();
    void disable_moving_average();
    void set_moving_average_size(byte size);
    void enable_peak();
    void disable_peak();
    void set_peak_ttl(byte ttl);

    // XXX byte calculate_current_MAPPED_value(byte S_METER_from_RADIO);

  private:
    byte convert_val_to_index(byte val);

    byte _id;
    byte _cur_val;
    byte _low_val;
    byte _high_val;

    bool _ave_enabled;
    byte _ave_val_history[LED_MAX_AVERAGE_HISTORY];
    byte _ave_history_pos;
    byte _ave_history_size;

    bool _peak_enabled;
    byte _peak_index;
    byte _peak_ttl = LED_DEFAULT_PEAK_TTL;
    byte _peak_ttl_count;
};

#endif // __LED_ARC_BOARD_H__

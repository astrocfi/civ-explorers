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

#include "led_arc_board.h"

LEDArcBoard::LEDArcBoard(byte id,
                         bool enable_moving_average=true,
                         bool enable_peak=true,
                         bool smooth_arc=true)
{
  _id = id;
  _cur_val = 0;
  _displayed_index = 0;
  _smooth_arc = smooth_arc;
  _arc_smoothing_speed = LEDARC_DEFAULT_ARC_SMOOTHING_SPEED;
  _last_led_update_time = 0;
  _brightness = LEDARC_DEFAULT_PWM;
  _last_brightness = 0;

  _ave_enabled = enable_moving_average;
  memset(_ave_val_history, 0, sizeof(_ave_val_history));
  _ave_history_pos = 0;
  _ave_history_size = sizeof(_ave_val_history);
  _ave_speed = LEDARC_DEFAULT_MOVING_AVERAGE_SPEED;
  _last_ave_time = 0;

  _peak_enabled = enable_peak;
  _peak_index = 0;
  _peak_ttl = LEDARC_DEFAULT_PEAK_TTL;
  _last_peak_update_time = 0;
}

void LEDArcBoard::begin()
{
  unsigned long cur_milli = millis();
  _last_led_update_time = cur_milli;
  _last_ave_time = cur_milli;
  _last_peak_update_time = cur_milli;
  pinMode(LEDARC_PWM_PIN, OUTPUT);
  pinMode(LEDARC_STORE_PIN, OUTPUT);
  pinMode(LEDARC_CLOCK_PIN, OUTPUT);
  pinMode(LEDARC_DATA_PIN, OUTPUT);
  analogWrite(LEDARC_PWM_PIN, _brightness);
  _last_brightness = _brightness;
  digitalWrite(LEDARC_STORE_PIN, HIGH);
}

//
// General accessors for private member variables
//

// The ID is used in case you want to instantiate more than one LEDArcBoard
// and need an easy way to distinguish among them
byte LEDArcBoard::set_id(byte id)
{
  _id = id;
}

byte LEDArcBoard::get_id()
{
  return _id;
}

//

void LEDArcBoard::set_current_val(int16_t cur_val)
{
  _cur_val = cur_val;
}

int16_t LEDArcBoard::get_current_val()
{
  return _cur_val;
}

//

void LEDArcBoard::set_led_brightness(byte pwm)
{
  _brightness = pwm;
}

byte LEDArcBoard::get_led_brightness()
{
  return _brightness;
}

//

void LEDArcBoard::enable_arc_smoothing()
{
  _smooth_arc = true;
}

void LEDArcBoard::disable_arc_smoothing()
{
  _smooth_arc = false;
}

void LEDArcBoard::set_arc_smoothing_speed(unsigned long speed)
{
  _arc_smoothing_speed = speed;
}

unsigned long LEDArcBoard::get_arc_smoothing_speed()
{
  return _arc_smoothing_speed;
}

//

void LEDArcBoard::enable_moving_average()
{
  _ave_enabled = true;
}

void LEDArcBoard::disable_moving_average()
{
  _ave_enabled = false;
}

void LEDArcBoard::set_moving_average_size(uint16_t size)
{
    _ave_history_size = size;
    // When changing the size, we start the moving average over from the
    // beginning to make sure we're not running off the end of the new buffer
    memset(_ave_val_history, 0, sizeof(_ave_val_history));
    _ave_history_pos = 0;
}

uint16_t LEDArcBoard::get_moving_average_size()
{
  return _ave_history_size;
}

void LEDArcBoard::set_moving_average_speed(unsigned long speed)
{
  _ave_speed = speed;
}

unsigned long LEDArcBoard::get_moving_average_speed()
{
  return _ave_speed;
}

//

void LEDArcBoard::enable_peak()
{
  _peak_enabled = true;
}

void LEDArcBoard::disable_peak()
{
  _peak_enabled = false;
}

void LEDArcBoard::set_peak_ttl(unsigned long ttl)
{
  _peak_ttl = ttl;
}

unsigned long LEDArcBoard::get_peak_ttl()
{
  return _peak_ttl;
}

//
// Start of code to actually manipulate the LEDs
//

// Convert a "source value" to a LED index. This allows the "source value"
// to be in whatever units the caller wants, and we only pay attention to LED
// index internally.
byte LEDArcBoard::_convert_val_to_index(int16_t val)
{
  // This is a placeholder for now until I figure out how I want to convert
  // S-meter values to index value
  if (val > 16)
    return 16;
  return val;
}

// Convert an index (0=off, 1-8 or 1-16=LED number from low to high)
// to a bitmask that has all the bits from the index down to 0 set.
uint16_t _index_to_arc_bitmask(byte index)
{
  if (index >= 16) // Separate case since 1<<index might overflow on 16
    return 0xffff;
  else if (index > 0)
    return (1 << index)-1;
  return 0;
}

// Convert an index (0=off, 1-8 or 1-16=LED number from low to high)
// to a bitmask with a single bit set for that index.
uint16_t _index_to_single_bitmask(byte index)
{
  if (index > 0)
    return 1 << (index-1);
  return 0;
}

void LEDArcBoard::update_leds()
{
  unsigned long cur_milli = millis();
  byte cur_index = _convert_val_to_index(_cur_val);

  // If enabled, and the proper time has elapsed, update the moving average
  // buffer
  if (_ave_enabled)
  {
    // We might have to do this more than once if it has been a long time since
    // the LEDs were last updated, so we play catch up here to make sure the
    // moving average represents the proper number of samples (even if they're
    // the same value over and over)
    while (cur_milli >= _last_ave_time+_ave_speed)
    {
      _last_ave_time += _ave_speed;
      // We store the historical values in a round robin buffer
      _ave_val_history[_ave_history_pos] = _cur_val;
      _ave_history_pos++;
      if (_ave_history_pos >= _ave_history_size)
          _ave_history_pos = 0;
    }
  }

  // All updates to the LEDs, including the moving average and peak, happen
  // at the rate of the arc smoothing speed, if smoothing is enabled.
  if (_smooth_arc && cur_milli < _last_led_update_time+_arc_smoothing_speed)
    return;
  _last_led_update_time = cur_milli;

  if (!_smooth_arc)
    _displayed_index = cur_index;
  else
    // We "smooth" the arc by only adding or removing one LED at a time
    if (cur_index > _displayed_index)
      _displayed_index++;
    else if (cur_index < _displayed_index)
      _displayed_index--;

  // We're starting with "0" meaning off just to make the bit manipulation
  // prettier; we flip it later
  uint16_t red = 0;
  uint16_t green = 0;
  uint16_t blue = 0;

  // Make the current value blue in a complete arc
  blue |= _index_to_arc_bitmask(_displayed_index);

  // If enabled, calculate the actual moving average and mark it yellow
  if (_ave_enabled)
  {
    long sum = 0; // Use long since sum can overflow a single int16
    for (int i=0; i<_ave_history_size; i++)
        sum += _ave_val_history[i];
    // Computing the LED index uses integer division, which rounds down.
    // It might be nicer to round up or down as appropriate.
    byte ave_index = _convert_val_to_index(sum / _ave_history_size);

    uint16_t bitmask = _index_to_single_bitmask(ave_index);
    red |= bitmask;
    green |= bitmask;
    blue &= ~bitmask;
  }

  // If enabled, compute the new peak value and mark it white.
  // We do this after average so the white overrides the yellow.
  if (_peak_enabled)
  {
    if (_displayed_index >= _peak_index)
    {
      _peak_index = _displayed_index;
      _last_peak_update_time = cur_milli;
    }
    else if (_peak_index != 0 && cur_milli >= _last_peak_update_time+_peak_ttl)
        _peak_index = 0;

    uint16_t bitmask = _index_to_single_bitmask(_peak_index);
    red |= bitmask;
    green |= bitmask;
    blue |= bitmask;
  }

  // Left half of arc
  byte red1   = ~(red & 0xff00) >> 8;
  byte green1 = ~(green & 0xff00) >> 8;
  byte blue1  = ~(blue & 0xff00) >> 8;
  // Right half of arc
  byte red2   = ~(red & 0xff);
  byte green2 = ~(green & 0xff);
  byte blue2  = ~(blue & 0xff);

  if (_brightness != _last_brightness)
  {
    analogWrite(LEDARC_PWM_PIN, _brightness);
    _last_brightness = _brightness;
  }

  digitalWrite(LEDARC_STORE_PIN, LOW);
#ifdef LEDARC_16_RGB
  shiftOut(LEDARC_DATA_PIN, LEDARC_CLOCK_PIN, MSBFIRST, red1);
  shiftOut(LEDARC_DATA_PIN, LEDARC_CLOCK_PIN, MSBFIRST, green1);
  shiftOut(LEDARC_DATA_PIN, LEDARC_CLOCK_PIN, MSBFIRST, blue1);
#endif
  shiftOut(LEDARC_DATA_PIN, LEDARC_CLOCK_PIN, LSBFIRST, red2);
  shiftOut(LEDARC_DATA_PIN, LEDARC_CLOCK_PIN, LSBFIRST, green2);
  shiftOut(LEDARC_DATA_PIN, LEDARC_CLOCK_PIN, LSBFIRST, blue2);
  digitalWrite(LEDARC_STORE_PIN, HIGH);
}







#ifdef XXX
byte LEDArcBoard::calculate_current_MAPPED_value(byte S_METER_from_RADIO)    // pass  s meter value (from radio ) in here
{
 #ifdef LEDARC_8_RGB
  if (_s_meter_low  <= 120)
  {
    _low_index = map(_s_meter_low, 0, 120, 0, 5);        // map
  }

  if (_s_meter_low  >= 121)
  {
    _low_index = map(_s_meter_low, 121, 240, 0, 3);        // map
    _low_index = _low_index + 9;
  }
  if (_s_meter_high  <= 120)
  {
    _high_index = map(_s_meter_high, 0, 120, 0, 5);        // map
  }
  if (_s_meter_high  >= 121)
  {
    _high_index = map(_s_meter_high, 121, 240, 0, 3);        // map
    _high_index = _high_index + 9;
  }
  if (_ave_val  <= 120)
  {
    _ave_index = map(_ave_val, 0, 120, 0, 5);             // map
  }
  if (_ave_val  >= 121)
  {
    _ave_index = map(_ave_val, 121, 240, 0, 3);             // map
    _ave_index =_ave_index + 9;
  }
#endif

#ifdef LEDARC_16_RGB
  if (_s_meter_low  <= 120)
  {
    _low_index = map(_s_meter_low, 0, 120, 0, 9);        // map
  }
  if (_s_meter_low  >= 121)
  {
    _low_index = map(_s_meter_low, 121, 241, 1, 6);        // map
    _low_index = _low_index + 9;
  }
  if (_s_meter_high  <= 120)
  {
    _high_index = map(_s_meter_high, 0, 120, 0, 9);        // map
  }
  if (_s_meter_high  >= 121)
  {
    _high_index = map(_s_meter_high, 121, 241, 1, 6);        // map
    _high_index = _high_index + 9;
  }
  if (_ave_val  <= 120)
  {
    _ave_index = map(_ave_val, 0, 120, 0, 9);             // map
  }
  if (_ave_val  >= 121)
  {
    _ave_index = map(_ave_val, 121, 241, 1, 6);             // map
    _ave_index =_ave_index + 9;
  }
#endif

}
#endif

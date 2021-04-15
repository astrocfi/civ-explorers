#include "led_arc_board.h"

LEDArcBoard::LEDArcBoard(byte id,
                         bool enable_moving_average=false,
                         bool enable_peak=false)
{
  _id = id;
  _cur_val = 0;
  _low_val = 0;
  _high_val = 0;

  _ave_enabled = enable_moving_average;
  memset(_ave_val_history, 0, LED_MAX_AVERAGE_HISTORY);
  _ave_history_pos = 0;
  _ave_history_size = LED_DEFAULT_AVERAGE_HISTORY;

  _peak_enabled = enable_peak;
  _peak_index = 0;
  _peak_ttl = LED_DEFAULT_PEAK_TTL;
  _peak_ttl_count = 0;
}

void LEDArcBoard::begin()  // initialises the device
{
}

byte LEDArcBoard::set_id(byte id)
{
  _id = id;
}

byte LEDArcBoard::get_id()
{
  return _id;
}

void LEDArcBoard::set_current_val(byte cur_val)
{
  _cur_val = cur_val;
}

byte LEDArcBoard::get_current_val()
{
  return _cur_val;
}

void LEDArcBoard::set_low_val(byte low_val)
{
  _low_val = low_val;
}

byte LEDArcBoard::get_low_val()
{
  return _low_val;
}

void LEDArcBoard::set_high_val(byte high_val)
{
  _high_val = high_val;
}

byte LEDArcBoard::get_high_val()
{
  return _high_val;
}

void LEDArcBoard::enable_moving_average()
{
  _ave_enabled = true;
}

void LEDArcBoard::disable_moving_average()
{
  _ave_enabled = false;
}

void LEDArcBoard::set_moving_average_size(byte size)
{
    _ave_history_size = size;
    memset(_ave_val_history, 0, LED_MAX_AVERAGE_HISTORY);
    _ave_history_pos = 0;
}

void LEDArcBoard::enable_peak()
{
  _peak_enabled = true;
}

void LEDArcBoard::disable_peak()
{
  _peak_enabled = false;
}

// byte LEDArcBoard::get_peak_TTL_count ()
// {
//   return _peak_TTL_count;;
// }

#ifdef XXX
byte LEDArcBoard::calculate_current_MAPPED_value(byte S_METER_from_RADIO)    // pass  s meter value (from radio ) in here
{

  _s_meter_current =  S_METER_from_RADIO;    // get S meter data

  if (_s_meter_current >= _s_meter_peak )
  {
    _s_meter_peak = _s_meter_current;            // PEAK  but must be zeroed after 600ms  PEAK PEAK PEAK
  }

  // TEMP

  //_s_meter_peak = 170 ;


  _ave_val =   (_ave_val + _s_meter_current+0 ) / 2;           // AVERAGE
  // add old average to new value and divide by 2


  if (_s_meter_current  <  _s_meter_low  )
  // new is lower than old low
  {
    _s_meter_low = _s_meter_current;              // LOW
    // make new the lowest
  }

  if (_s_meter_current  >  _s_meter_high )
  // new is higher than old high
  {
    _s_meter_high = _s_meter_current;            // HIGH
    // make new the highest
  }
// do I need a new max which I hold the turn off after 600ms ?

  if (_s_meter_current  <  _s_meter_high )
  // new is lower than highest
  {
    _s_meter_high = (_s_meter_high  -8);
    // reduce highest by -8  ...  some decay in meter value ?

    if (_s_meter_high < 0)
    {
      _s_meter_high = 0;
      // dont allow highest below zero
    }
    if (_s_meter_high > 242)
    //  stop is rolling round to 255 or there abouts
    {
      _s_meter_high =0;
      // dont allow highest below zero
    }
  }

  if (_s_meter_current  >  _s_meter_low  )
  // if new greater than lowest
  {
    _s_meter_low = (_s_meter_low  +10);
    //   incresse lowest by + 10 ... keeps on lower LEDs ... stops flikering

    if (_s_meter_low > _s_meter_current)
    {
      _s_meter_high = _s_meter_current;                      // WHY ?
    }
  }

  if (_s_meter_low  >  _s_meter_high  )
  {
    _s_meter_low  = _s_meter_high;
  }

  if (_s_meter_high  <  _s_meter_low  )
  {
    _s_meter_high = _s_meter_low;
  }

  if (_s_meter_current  <  _ave_val )
  {
    _ave_val = (_ave_val * 0.75);
  }


 #ifdef LED_8_RGB
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

#ifdef LED_16_RGB
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

  if (_s_meter_peak  <= 120)
  {
    _peak_index = map(_s_meter_peak, 0, 120, 0, 9);             // map
  }
  if (_s_meter_peak  >= 121)
  {
    _peak_index = map(_s_meter_peak, 121, 241, 1, 6);            // map
    _peak_index = _peak_index + 9;
  }

  _peak_TTL_count++;


  if (_peak_TTL_count >= LED_PEAK_TTL)
  {
    _peak_index = 0;
    _s_meter_peak = 0;            // clear peak
    _peak_TTL_count =0;            // clear count

  }


 switch (_peak_index)
 {
 case 0:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0xFF;      // high byte  was FF TEMP
   break;

 case 1:
   _peak_Index_LO =0x7F;      // low byte
   _peak_Index_HI =0xFF;      // high byte
   break;

 case 2:
   _peak_Index_LO =0xBF;      // low byte
   _peak_Index_HI =0xFF;      // high byte
   break;

 case 3:
   _peak_Index_LO =0xDF;      // low byte
   _peak_Index_HI =0xFF;      // high byte
   break;

 case 4:
   _peak_Index_LO =0xEF;      // low byte
   _peak_Index_HI =0xFF;      // high byte
   break;

 case 5:
   _peak_Index_LO =0xF7;      // low byte
   _peak_Index_HI =0xFF;      // high byte
   break;

 case 6:
   _peak_Index_LO =0xFB;      // low byte
   _peak_Index_HI =0xFF;      // high byte
   break;

 case 7:
   _peak_Index_LO =0xFD;      // low byte
   _peak_Index_HI =0xFF;      // high byte
   break;

 case 8:
   _peak_Index_LO =0xFE;      // low byte
   _peak_Index_HI =0xFF;      // high byte
   break;

 case 9:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0x7F;      // high byte
   break;

 case 10:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0xBF;      // high byte
   break;

 case 11:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0xDF;      // high byte
   break;

 case 12:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0xEF;      // high byte
   break;

 case 13:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0xF7;      // high byte
   break;

 case 14:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0xFB;      // high byte
   break;

 case 15:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0xFD;

 case 16:
   _peak_Index_LO =0xFF;      // low byte
   _peak_Index_HI =0xFE;      // high byte
   break;
 }

 return _ave_val;
}
#endif

byte LEDArcBoard::convert_val_to_index(byte val)
{
  if (val > 16)
  {
    return 16;
  }
  return val;
}

word _index_to_arc_bitmask(byte index)
{
  word ret = 0;
  if (index >= 16)
  {
    ret = 0xffff;
  }
  else if (index > 0)
  {
    ret |= (1 << index)-1;
  }
  return ret;
}

word _index_to_single_bitmask(byte index)
{
  word ret = 0;
  if (index > 0)
  {
    ret = 1 << (index-1);
  }
  return ret;
}

void LEDArcBoard::update_leds()
{
  byte cur_index = convert_val_to_index(_cur_val);

  // We're starting with "0" meaning off just to make the big manipulation
  // prettier
  word red = 0;
  word green = 0;
  word blue = 0;

  // Make the current value blue
  blue |= _index_to_arc_bitmask(cur_index);

  // If enabled, mark the average value yellow
  if (_ave_enabled)
  {
    // We store the historical values in a round robin buffer
    _ave_val_history[_ave_history_pos] = _cur_val;
    _ave_history_pos++;
    if (_ave_history_pos >= _ave_history_size)
    {
        _ave_history_pos = 0;
    }
    int sum = 0;
    for (int i=0; i<_ave_history_size; i++)
    {
        sum += _ave_val_history[i];
    }
    byte ave_index = convert_val_to_index(sum / _ave_history_size);

    word bitmask = _index_to_single_bitmask(ave_index);
    red |= bitmask;
    green |= bitmask;
    blue &= ~bitmask;
  }

  // If enabled, compute the new peak value and mark
  // We do this after average so the white overrides the yellow
  if (_peak_enabled)
  {
    if (cur_index >= _peak_index)
    {
      _peak_index = cur_index;
      _peak_ttl_count = 0;
    }
    else if (_peak_index != 0)
    {
      _peak_ttl_count++;
      if (_peak_ttl_count >= _peak_ttl)
      {
        _peak_index = 0;
      }
    }

    word bitmask = _index_to_single_bitmask(_peak_index);
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

  digitalWrite(STORE_PIN, LOW);
#ifdef LED_16_RGB
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, red1);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, green1);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, blue1);
#endif
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, red2);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, green2);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, blue2);
  digitalWrite(STORE_PIN, HIGH);
}

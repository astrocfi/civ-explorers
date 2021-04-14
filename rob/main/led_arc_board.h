#ifndef __LED_METER_H__
#define __LED_METER_H__

#define LED_8_ATTACK      0   // ms  was 10
#define LED_8_SUSTAIN     2   // ms  was 10
#define LED_8_DECAY       0   // ms  was 30

#define LED_16_ATTACK     0   // ms
#define LED_16_SUSTAIN    1   // ms  was 10
#define LED_16_DECAY      2
#define LED_PEAK_TTL     20

class LEDArcBoard
{
  public:
    LEDArcBoard(byte id);

    void begin();
    byte get_id();
    byte set_id(byte id);
    void set_current_s_meter(byte s_meter_current);
    byte get_current_s_meter();
    byte get_s_meter_mov_ave();
    void set_s_meter_mov_ave(byte s_meter_average);                  // average
    void set_s_meter_low(byte s_meter_low);                   // low
    byte get_s_meter_low();
    void set_s_meter_high(byte s_meter_high);                   // high
    byte get_s_meter_high();
    byte get_s_meter_peak_Index_LO();             // for LED
    byte get_s_meter_peak_Index_HI();             // for LED

    byte get_peak_TTL_count ();
    byte get_s_meter_high_index();                   // "    "
    byte get_s_meter_ave_index();             // "    "
    byte get_s_meter_low_index();              // "    "

    byte calculate_current_MAPPED_value(byte S_METER_from_RADIO);

  private:
    byte _id;
    char _button_pressed_NOW_TOKEN;
    byte _s_meter_current;
    byte _s_meter_mov_ave;        //was volatile
    byte _s_meter_low;         //was volatile
    byte _s_meter_high;         //was volatile
    byte _s_meter_peak;          //was volatile

    byte _low_index;
    byte _high_index;
    byte _ave_index;
    byte _peak_index = 0;
    //byte _last_s_meter_peak_index ;
    byte _peak_Index_LO;
    byte _peak_Index_HI;
    byte _peak_TTL_count;          // is this a Time to Live  counter ?
};

#endif __LED_METER_H__

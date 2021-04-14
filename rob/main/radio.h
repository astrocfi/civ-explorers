#ifndef __RADIO_H__
#define __RADIO_H__

class Radio
{
  public:
    Radio(byte id);

    void begin();
    byte get_id();
    byte set_id(byte id);
    void set_RADIO_connected (bool radio);
    bool get_RADIO_connected ();
    byte get_current_band();          //
    void set_current_band(byte current_band);
    byte get_current_rx_mode();            // maybe change this to a TOKEN i.e. a char rather than byte ... and remove lookup tables
    void set_current_rx_mode(byte current_receive_mode);
    unsigned long get_current_frequency();
    boolean set_current_frequency(unsigned long current_frequency);
    int get_current_TS();
    bool set_current_TS(int tuning_step);

    // s-meter
    byte get_current_s_meter();
    void set_current_s_meter(byte s_meter_current);          // current (NOW)

    // CI-V message status
    void set_FE1_flag(bool flag);
    bool get_FE1_flag();
    void set_FE2_flag(bool flag);
    bool get_FE2_flag();
    void set_to_flag(bool flag);
    bool get_to_flag();
    void set_from_flag(bool flag);
    bool get_from_flag();
    void set_RX_Message_validity(byte flag);
    byte get_RX_Message_validity();
    void set_FA_flag(bool flag);
    bool get_FA_flag();
    void set_FB_flag(bool flag);
    bool get_FB_flag();
    void set_FD_flag(bool flag);
    bool get_FD_flag();

  private:
    byte _id;

    unsigned long _current_frequency;          // might use a union for the frequency ?
    unsigned long _min_frequency;          // this is radio specific ... should I look this up ?
    unsigned long _max_frequency;          // ditto
    bool _freq_request_is_out_of_bounds_flag;
    // i.e. shall set this if last frequency change request went beyound high or low limit ... roll around ?
    int _current_TS;          // tuning step i.e. size (KHz) of incr of decr step
    int _min_TS;          // tuning step i.e. size (KHz) of incr of decr step
    int _max_TS;          // tuning step i.e. size (KHz) of incr of decr step
    volatile byte _s_meter_current;

    // no mapped value ?
    byte _current_band;          //
    byte _current_receive_mode;          //

    bool _FE1_flag;
    bool _FE2_flag;
    bool _FD_flag;
    bool _FB_flag;
    bool _FA_flag;
    bool _to_flag;
    bool _from_flag;
    bool _RX_Message_validity;
    bool _RADIO_connected;
};



#endif /* __RADIO_H__ */

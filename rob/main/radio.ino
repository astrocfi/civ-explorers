/*******************************************************************************
 * Author: Robert S. French <rfrench@rfrench.org>
 * Derived from work by John Rowling
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative
 * Commons, PO Box 1866, Mountain View, CA 94042, USA.
 ******************************************************************************/

Radio::Radio(byte id)
{
  _id = id;
}

void Radio::begin()      // initialises Radio
{

  _current_frequency = 1;             //    1 MHz
  _current_band = 0;
  // i.e. the out of bound 'bucket'  max value is 10
  _current_receive_mode = 99;
  //  modes are 0:LSB, 1:USB, 2:AM, 3:CW, 4:(different 7300 / 8600), 5:FM
  _min_frequency =      100000;             //  100 KHz
  _max_frequency =    3000000000;             //   70 MHz for 7300
  _current_TS = 1000;                 //   tuning step 1 KHz
  _min_TS = 100;
  _max_TS = 50000;
  // _s_meter_hundreds_value = 0 ;
  //_s_meter_tens_value=0 ;
  //_s_meter_units_value=0 ;

  _s_meter_current = 0;


  _FE1_flag = false;
  _FE2_flag = false;
  _FD_flag = false;
  _FB_flag = false;
  _FA_flag = false;
  _to_flag = false;
  _from_flag = false;
  _RX_Message_validity = 0;
  _RADIO_connected = false;

}


byte Radio::set_id(byte id)
{
  if(id <= 240 && id >0)               // acceptable value  is from  1 to 240
  {
    _id = id;
  }
  return _id;
}
byte Radio::get_id()
{
  return _id;
}
/*
   // new tweakables start
        byte Radio::NR_get_off_on () // 00 or 01
                  {
                  return _NR_off_on ;// 00 or 01
                  }

        void Radio::NR_set_off_on (byte on_off)
                {
                 _NR_off_on = on_off ;
                }

        byte Radio::NR_get_level ()  // 0000 - 0255
                {
                  return _NR__level  ; // 0000 - 0255
                }

        void Radio::NR_set_level (byte on_off)  // 0000 - 0255
                {
                  _NR__level = on_off; // 0000 - 0255
                }

        byte Radio::NB_get_off_on () // 00 or 01
                  {
              return _NB_off_on  ;// 00 or 01
                  }


        void Radio::NB_set_off_on (byte on_off)
                {
                  _NB_off_on = on_off ;// 00 or 01
                }


        byte Radio::NB_get_depth ()  // 00 - 09
                {
                return _NB_depth  ; // 00 - 09
                }
        void Radio::NB_set_depth (byte depth)  // 00 - 09
                {
                _NB_depth  = depth ; // 00 - 09
                }
        byte Radio::NB_get_width ()  // 0000 - 0255
                {
                return _NB_width  ; // 0000 - 0255
                }
        void Radio::NB_set_width (byte width)  // 0000 - 0255
                {
                  _NB_width = width ; // 0000 - 0255
                }
        byte Radio::NB_get_level ()  // 0000 - 0255
                {
                return _NB_level  ; // 0000 - 0255
                }
        void Radio::NB_set_level (byte level)  // 0000 - 0255
                {
                  _NB_level = level ; // 0000 - 0255
                }


        byte Radio::Notch_get_auto_off_on () // 00 or 01
                    {
                    _Notch_auto_off_on  ;// 00 or 01
                    }
        void Radio::Notch_set_auto_off_on (byte on_off)
                  {
                   _Notch_auto_off_on = on_off ;// 00 or 01
                  }
        byte Radio::Notch_get_manual_off_on ()
                {
                     return _Notch_manual_off_on  ;// 00 or 01 // 00 or 01
                }
        void Radio::Notch_set_manual_off_on (byte on_off)
                    {
                      _Notch_manual_off_on = on_off ;// 00 or 01
                    }
        byte Radio::Notch_get_width ()
              {
              return _Notch_width  ; // 0000 - 0255
              } // 0000 - 0255

        void Radio::Notch_set_width (byte width)
                  {
                    _Notch_width = width ; // 0000 - 0255
                  } // 0000 - 0255

        byte Radio::Notch_get_position ()
                {
                return _Notch_position  ; // 0000 - 0255
                } // 0000 - 0255

        void Radio::Notch_set_position (byte position)
                {
                 _Notch_position = position ; // 0000 - 0255
                } // 0000 - 0255



   // new tweakables end
 */

void Radio::set_current_s_meter(byte s_meter_current)
{
  _s_meter_current = s_meter_current;
}

byte Radio::get_current_s_meter()
{
  return _s_meter_current;
}


void Radio::set_current_band(byte current_band)
{
  if (current_band < 11 )
  {
    _current_band = current_band;
  }


}

byte Radio::get_current_band()
{
  return _current_band;
}


void Radio::set_current_rx_mode(byte current_receive_mode)
{
  _current_receive_mode = current_receive_mode;

}

byte Radio::get_current_rx_mode()
{
  return _current_receive_mode;
  // convert to ascii
}

unsigned long Radio::get_current_frequency()
{
  return _current_frequency;
}

bool Radio::set_current_frequency(unsigned long current_frequency)
{
  _freq_request_is_out_of_bounds_flag = true;
  // i.e. set flag to indicate a default of out of bounds !

  if (current_frequency < _max_frequency
      && current_frequency > _min_frequency)
  {
    _current_frequency = current_frequency;
    _freq_request_is_out_of_bounds_flag = false;
    // ie. reset flag
  }
  return _freq_request_is_out_of_bounds_flag;
}

int Radio::get_current_TS()
{
  return _current_TS;
}


bool Radio::set_current_TS(int current_TS)
{
  _freq_request_is_out_of_bounds_flag = true;           // i.e. set flag to indicate a default of out of bounds !

  if (current_TS < _max_TS
      && current_TS > _min_TS)
  {
    _current_TS = current_TS;
    _freq_request_is_out_of_bounds_flag = false;               // ie. reset flag
  }
  return _freq_request_is_out_of_bounds_flag;

}

// CI_V message status
void Radio::set_FE1_flag (bool flag)
{
  _FE1_flag = flag;
}
bool Radio::get_FE1_flag ()
{
  return _FE1_flag;
}

void Radio::set_FE2_flag (bool flag)
{
  _FE2_flag = flag;
}
bool Radio::get_FE2_flag ()
{
  return _FE2_flag;
}
void Radio::set_to_flag (bool flag)
{
  _to_flag = flag;
}
bool Radio::get_to_flag ()
{
  return _to_flag;
}
void Radio::set_from_flag (bool flag)
{
  _from_flag = flag;
}
bool Radio::get_from_flag ()
{
  return _from_flag;
}
void Radio::set_RX_Message_validity (byte flag)
{
  _RX_Message_validity = flag;
}

byte Radio::get_RX_Message_validity ()
{
  return _RX_Message_validity;
}
void Radio::set_FA_flag (bool flag)
{
  _FA_flag = flag;
}
bool Radio::get_FA_flag ()
{
  return _FA_flag;
}
void Radio::set_FB_flag (bool flag)
{
  _FB_flag = flag;
}

bool Radio::get_FB_flag ()
{
  return _FB_flag;
}
void Radio::set_FD_flag (bool flag)
{
  _FD_flag = flag;
}

bool Radio::get_FD_flag ()
{
  return _FD_flag;
}

bool Radio::get_RADIO_connected()
{
  return _RADIO_connected;
}

void Radio::set_RADIO_connected(bool radio)
{
  _RADIO_connected = radio;
}
// from KBD object

// from KBD object

// end of constructor
// *******************************************************************************
// *******************      end of radio object     ******************************
// *******************************************************************************

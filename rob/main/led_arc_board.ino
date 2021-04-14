#include "led_arc_board.h"

LEDArcBoard::LEDArcBoard(byte id)
{
  _id = id;
}

void LEDArcBoard::begin()  // initialises the device
{
  _s_meter_mov_ave = 0;
  _s_meter_low = 0;
  _s_meter_high = 0;
  _s_meter_peak = 0;
  // _last_s_meter_peak_index =0 ;

  _low_index = 0;
  _high_index = 0;
  _ave_index  = 0;
  _peak_index = 0;

  // _last_s_meter_peak_index =0 ;
  _peak_Index_LO = 0;
  _peak_Index_HI = 0;
  _peak_TTL_count = 0;

};


byte LEDArcBoard::set_id(byte id)
{
  if(id <= 240 && id >0)              // acceptable value  is from  1 to 240
  {
    _id = id;
  }
  return _id;
}

byte LEDArcBoard::get_id()
{
  return _id;
}
// redundant ??  START
void LEDArcBoard::set_current_s_meter(byte s_meter_current)
{
  _s_meter_current = s_meter_current;
}

byte LEDArcBoard::get_current_s_meter()
{
  return _s_meter_current;
}



void LEDArcBoard::set_s_meter_mov_ave(byte s_meter_average)
{
  _s_meter_mov_ave = s_meter_average;
}

byte LEDArcBoard::get_s_meter_mov_ave()
{
  return _s_meter_mov_ave;
}

void LEDArcBoard::set_s_meter_low(byte s_meter_low)
{
  _s_meter_low = s_meter_low;
}

byte LEDArcBoard::get_s_meter_low()
{
  return _s_meter_low;
}

void LEDArcBoard::set_s_meter_high(byte s_meter_high)
{
  _s_meter_high = s_meter_high;
}

byte LEDArcBoard::get_s_meter_high()
{
  return _s_meter_high;
}
// redundant  --- end ??

byte LEDArcBoard::get_peak_TTL_count ()
{
  return _peak_TTL_count;;
}


byte LEDArcBoard::calculate_current_MAPPED_value(byte S_METER_from_RADIO)    // pass  s meter value ( from radio ) in here
{

  _s_meter_current =  S_METER_from_RADIO;    // get S meter data

  if ( _s_meter_current >= _s_meter_peak )
  {
    _s_meter_peak = _s_meter_current;            // PEAK  but must be zeroed after 600ms  PEAK PEAK PEAK
  }

  // TEMP

  //_s_meter_peak = 170 ;


  _s_meter_mov_ave =   (_s_meter_mov_ave + _s_meter_current+0 ) / 2;           // AVERAGE
  // add old average to new value and divide by 2


  if ( _s_meter_current  <  _s_meter_low  )
  // new is lower than old low
  {
    _s_meter_low = _s_meter_current;              // LOW
    // make new the lowest
  }

  if ( _s_meter_current  >  _s_meter_high )
  // new is higher than old high
  {
    _s_meter_high = _s_meter_current;            // HIGH
    // make new the highest
  }
// do I need a new max which I hold the turn off after 600ms ?

  if ( _s_meter_current  <  _s_meter_high )
  // new is lower than highest
  {
    _s_meter_high = ( _s_meter_high  -8 );
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

  if ( _s_meter_current  >  _s_meter_low  )
  // if new greater than lowest
  {
    _s_meter_low = ( _s_meter_low  +10 );
    //   incresse lowest by + 10 ... keeps on lower LEDs ... stops flikering

    if (_s_meter_low > _s_meter_current)
    {
      _s_meter_high = _s_meter_current;                      // WHY ?
    }
  }

  if ( _s_meter_low  >  _s_meter_high  )
  {
    _s_meter_low  = _s_meter_high;
  }

  if ( _s_meter_high  <  _s_meter_low  )
  {
    _s_meter_high = _s_meter_low;
  }

  if ( _s_meter_current  <  _s_meter_mov_ave )
  {
    _s_meter_mov_ave = ( _s_meter_mov_ave * 0.75 );
  }

// we now have a value of S-meter LOW... MOVING AVERAGE ... HIGH and PEAK


  // *******************************************
  //return  _s_meter_mov_ave;  // THIS FUNCTION RETURNS the moving average TO WHERE ?
  // ******************************************
  // AND DOESNT DO THE REST !!!!  MOVE THIS RETURN TO THE END of this function

// ++++++++++++  CREATE INDEX FOR LED METER BELOW |
//            |
//            V

// put all this index creation inside the radio object  and rem it out here - start  of rem

// now indexes are created and stored for access by other calls to the object



 #ifdef LED_8_MONO
  if (_s_meter_low  <= 120)
  {
    _low_index = map ( _s_meter_low, 0, 120, 0, 5 );        // map
  }
  if (_s_meter_low  >= 121)
  {
    _low_index = map ( _s_meter_low, 121, 240, 0, 3 );        // map
    _low_index = _low_index + 9;
  }
  if (_s_meter_high  <= 120)
  {
    _high_index = map ( _s_meter_high, 0, 120, 0, 5 );        // map
  }
  if (_s_meter_high  >= 121)
  {
    _high_index = map ( _s_meter_high, 121, 240, 0, 3 );        // map
    _high_index = _high_index + 9;
  }
  if (_s_meter_mov_ave  <= 120)
  {
    _ave_index = map ( _s_meter_mov_ave, 0, 120, 0, 5 );             // map
  }
  if (_s_meter_mov_ave  >= 121)
  {
    _ave_index = map ( _s_meter_mov_ave, 121, 240, 0, 3 );             // map
    _ave_index =_ave_index + 9;
  }
 #endif

 #ifdef LED_8_RGB
  if (_s_meter_low  <= 120)
  {
    _low_index = map ( _s_meter_low, 0, 120, 0, 5 );        // map
  }

  if (_s_meter_low  >= 121)
  {
    _low_index = map ( _s_meter_low, 121, 240, 0, 3 );        // map
    _low_index = _low_index + 9;
  }
  if (_s_meter_high  <= 120)
  {
    _high_index = map ( _s_meter_high, 0, 120, 0, 5 );        // map
  }
  if (_s_meter_high  >= 121)
  {
    _high_index = map ( _s_meter_high, 121, 240, 0, 3 );        // map
    _high_index = _high_index + 9;
  }
  if (_s_meter_mov_ave  <= 120)
  {
    _ave_index = map ( _s_meter_mov_ave, 0, 120, 0, 5 );             // map
  }
  if (_s_meter_mov_ave  >= 121)
  {
    _ave_index = map ( _s_meter_mov_ave, 121, 240, 0, 3 );             // map
    _ave_index =_ave_index + 9;
  }
 #endif

 #ifdef LED_16_MONO
  if (_s_meter_low  <= 120)
  {
    _low_index = map (  _s_meter_low, 0, 120, 0, 9 );        // map
  }
  if ( _s_meter_low  >= 121)
  {
    _low_index = map (  _s_meter_low, 121, 240, 0, 6 );        // map
    _low_index = _low_index + 9;
  }
  if ( _s_meter_high  <= 120)
  {
    _high_index = map (  _s_meter_high, 0, 120, 0, 9 );        // map
  }
  if ( _s_meter_high  >= 121)
  {
    _high_index = map (  _s_meter_high, 121, 240, 0, 6 );        // map
    _high_index = _high_index + 9;
  }
  if ( _s_meter_mov_ave  <= 120)
  {
    _ave_index = map (  _s_meter_mov_ave, 0, 120, 0, 9 );             // map
  }
  if ( _s_meter_mov_ave  >= 121)
  {
    _ave_index = map (  _s_meter_mov_ave, 121, 240, 0, 6 );             // map
    _ave_index =_ave_index + 9;
  }
 #endif

 #ifdef LED_16_RGB // INDEX FOR LEDS

  if ( _s_meter_low  <= 120)
  {
    _low_index = map (  _s_meter_low, 0, 120, 0, 9 );        // map
  }
  if ( _s_meter_low  >= 121)
  {
    _low_index = map (  _s_meter_low, 121, 241, 1, 6 );        // map
    _low_index = _low_index + 9;
  }
  if ( _s_meter_high  <= 120)
  {
    _high_index = map (  _s_meter_high, 0, 120, 0, 9 );        // map
  }
  if ( _s_meter_high  >= 121)
  {
    _high_index = map (  _s_meter_high, 121, 241, 1, 6 );        // map
    _high_index = _high_index + 9;
  }
  if ( _s_meter_mov_ave  <= 120)
  {
    _ave_index = map (  _s_meter_mov_ave, 0, 120, 0, 9 );             // map
  }
  if ( _s_meter_mov_ave  >= 121)
  {
    _ave_index = map (  _s_meter_mov_ave, 121, 241, 1, 6 );             // map
    _ave_index =_ave_index + 9;
  }
#endif // RSF


// move this to the radio object or create an S-meter object ?





  if ( _s_meter_peak  <= 120)
  {
    _peak_index = map (  _s_meter_peak, 0, 120, 0, 9 );             // map
  }
  if ( _s_meter_peak  >= 121)
  {
    _peak_index = map ( _s_meter_peak, 121, 241, 1, 6 );            // map
    _peak_index = _peak_index + 9;
  }

  _peak_TTL_count++;


  if ( _peak_TTL_count >= LED_PEAK_TTL)
  {
    _peak_index = 0;
    _s_meter_peak = 0;            // clear peak
    _peak_TTL_count =0;            // clear count

  }



/*
      if ( _s_meter_high  <= 120)
              {
              _max_index = map (  _s_meter_high , 0, 120, 0, 9 ) ; // map
              }
      if ( _s_meter_high  >= 121)
              {
              _max_index = map ( _s_meter_high, 121, 241, 1, 6 ) ; // map
              _max_index = _max_index + 9 ;
              }
 */

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

   // default :
   // _peak_Index_LO =0xFF;// low byte
   // _peak_Index_HI =0xFF;// high byte
   // break  ;

 }

 return _s_meter_mov_ave;   // THIS FUNCTION RETURNS the moving average TO calling statement

 // i.e. returns the moving average S meter value ?



}
//matrix.print(MaxIndex1, DEC);
//matrix.writeDisplay(); // adds 1.5ms of delay

// put all this index creation inside the radio object  and rem it out here - end of rem

/*
       byte get_s_meter_peak_Index_LO()  ;  // for LED
       byte get_s_meter_peak_Index_HI()  ;  // for LED
       byte get_s_meter_high_index() ;  // "    "
       byte get_s_meter_ave_index()  ;  // "    "
       byte get_s_meter_low_index()  ;  // "    "

*/




byte LEDArcBoard::get_s_meter_peak_Index_LO()
{
 return _peak_Index_LO;
}

byte LEDArcBoard::get_s_meter_peak_Index_HI()
{
 return _peak_Index_HI;
}



// end of objet funcion -mapped values


byte LEDArcBoard::get_s_meter_low_index()
{
 return _low_index;
}

byte LEDArcBoard::get_s_meter_high_index()
{
 return _high_index;
}

byte LEDArcBoard::get_s_meter_ave_index()
{
 return _ave_index;
}




// *******************************************************************************
// *******************      end of LED Meter Object   ****************************
// *******************************************************************************

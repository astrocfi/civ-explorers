#include "led_arc_board.h"
#include "s_meter.h"

//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  ++++++++++++++++++++++       get s-meter function     ++++++++++++++++++++++++++++++++++++++++++
//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// orphan from tab 5
void request_s_meter()
{
#ifdef XXX
  tx_ci_v_via_serial_ttl(10);     // ci-v tx !

  bit_bash(10);       // sort out the nibbles

// new calc
  Icom.set_current_s_meter( ( (ci_v_rx_DATA[1])  *100 ) + ( (ci_v_rx_DATA[2] ) *10 ) + (  ci_v_rx_DATA[3] ) );

  // old calc
  // Icom.set_current_s_meter( ( Icom.get_s_meter_hundreds() *100) + (Icom.get_s_meter_tens() * 10 ) +  (Icom.get_s_meter_units()) ) ;

  //XXX LEDArc.calculate_current_MAPPED_value(Icom.get_current_s_meter() );    // ( Icom.get_current_s_meter() )
  // this returns an S-meter value, but also initiates the calcs inside the object to
  //generate the low, high and max1, max 2 indices

#endif
}
// end of orphan from tab 5


void S_meter2LED_index() {
#ifdef XXX
  // pass these of 'get' from radion object

  byte ave_index;
  byte low_index = LEDArc.get_s_meter_low_index();
  byte high_index = LEDArc.get_s_meter_high_index();
  byte ARCstart;
  byte ARCstop;

  byte max_index;
  byte MaxIndex1 =  LEDArc.get_s_meter_peak_Index_LO();
  byte MaxIndex2 = LEDArc.get_s_meter_peak_Index_HI();

  Serial.println ( );
  Serial.print ("  LEDArc.get_s_meter_peak_Index_LO = ");
  Serial.print (LEDArc.get_s_meter_peak_Index_LO() );

  Serial.print ("  LEDArc.get_s_meter_peak_Index_LO  =  " );
  Serial.println (LEDArc.get_s_meter_peak_Index_LO() );



// ***************************************************************
// *********** now send index values to draw the LED arc *********
// ***************************************************************

  if (  (low_index != high_index ) && ( low_index > high_index)  )// if lowest and highest are not same
  {

    for (int j = low_index; j <= high_index; j++)         // light LEDs from lowest to highest
    {
      digitalWrite(STORE_PIN, LOW);
      LED_arc (j,MaxIndex1,MaxIndex2);            // 'j' is current index  maxindex 1 and 2 are peak values)
      //LED_arc (j,0xFF,0xFF) ;// was working with this line
      digitalWrite(STORE_PIN, HIGH);
                   #ifdef LED_8_MONO
      delay(LED_8_ATTACK);
                   #endif
                   #ifdef LED_8_RGB
      delay(LED_8_ATTACK);
                   #endif
                   #ifdef LED_16_MONO
      delay(LED_16_ATTACK);
                   #endif
                   #ifdef LED_16_RGB
      delay(LED_16_ATTACK);
                   #endif
    }

  }

/// increasing
  if (high_index > low_index)
  {

    for (int j = low_index; j <= high_index; j++)
    {

      digitalWrite(STORE_PIN, LOW);
      LED_arc (j,MaxIndex1,MaxIndex2);                        // peak hold
      //LED_arc (j,MaxIndex1,0xFF) ; // was working with this line
      //LED_arc (j,0xFF,0xFF) ;
      digitalWrite(STORE_PIN, HIGH);
                 #ifdef LED_8_MONO
      delay(LED_8_ATTACK);
                 #endif

                 #ifdef LED_8_RGB
      delay(LED_8_ATTACK);
                 #endif

                 #ifdef LED_16_MONO
      delay(LED_16_ATTACK);
                 #endif

                 #ifdef LED_16_RGB
      delay(LED_16_ATTACK);
                 #endif
    }
  }
  //  *********SUSTAIN
                 #ifdef LED_8_MONO
  delay(LED_8_SUSTAIN);
                   #endif

                   #ifdef LED_8_RGB
  delay(LED_8_SUSTAIN);
                   #endif

                   #ifdef LED_16_MONO
  delay(LED_16_SUSTAIN);
                   #endif

                   #ifdef LED_16_RGB
  delay(LED_16_SUSTAIN);
                   #endif


// decreasing
  if (high_index <= low_index)
  {

    for (int j = high_index; j >= low_index; j--)
    {
      digitalWrite(STORE_PIN, LOW);
      LED_arc (j,MaxIndex1,MaxIndex2);                          // peak hold
      // LED_arc (j,0xFF,0xFF) ;// was working with this line
      digitalWrite(STORE_PIN, HIGH);
                           #ifdef LED_8_MONO
      delay(LED_8_DECAY);
                           #endif

                           #ifdef LED_8_RGB
      delay(LED_8_DECAY);
                           #endif

                           #ifdef LED_16_MONO
      delay(LED_16_DECAY);
                           #endif

                           #ifdef LED_16_RGB
      delay(LED_16_DECAY);
                           #endif
    }

  }
  #endif //XXX

}



#ifdef LED_8_RGB
void LED_arc(byte led, byte Max1, byte Max2) // LED_8_RGB // RSF
{        //start
  byte index = led;
  // byte led_pwm = shine ;
  switch ( index ) {       //switch
  case 0:          // s0
    // NO LED      OFF OFF OFF OFF OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red   ALL off
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 1
  case 1:          // s1
    // first       BLU OFF OFF OFF OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 2
  case 2:          // s3
    // second       BLU BLU OFF OFF OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00111111);               // 2nd blue
    break;        //  end of case 1
  case 3:          // s5
    // Third       BLU BLU BLU OFF OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00011111);               // 2nd blue
    break;        //  end of case 3
  case 4:          // s7
    // Forth       BLU BLU BLU BLU OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111);               // 2nd blue
    break;        //  end of case 4
  case 5:           // s9
    // Fifth       BLU BLU BLU BLU GRN OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111);               // 2nd blue
    break;        //  end of case 5
  case 6:           // s9+20
    // Sixth       BLU BLU BLU BLU GRN RED OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111011);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111);               // 2nd blue
    break;        //  end of case 6
  case 7:           // s9+40
    // Seventh     BLU BLU BLU BLU GRN RED RED OFF
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111001);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111);               // 2nd blue
    break;        //  end of case 7
  case 8:          // s9+60
    // Eigth       BLU BLU BLU BLU GRN RED RED RED
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111000);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111);               // 2nd blue
    break;        //  end of case 7
  }         // end of switch
}         // end of function

   #endif



#ifdef LED_8_MONO
void LED_arc(byte led)
{        //start
  byte index = led;
  // byte led_pwm = shine ;
  switch ( index ) {       //switch
  case 0:          // s0
    // // NO LED   OFF OFF OFF OFF OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 1
  case 1:          // s1
    // first       BLU OFF OFF OFF OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 2
  case 2:          // s2
    // second      BLU BLU OFF OFF OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00111111);               // 2nd blue
    break;        //  end of case 1
  case 3:          // s3
    // Third       BLU BLU BLU OFF OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00011111);               // 2nd blue
    break;        //  end of case 3
  case 4:          // s4
    // Fifth       BLU BLU BLU BLU OFF OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00001111);               // 2nd blue
    break;        //  end of case 4
  case 5:           // s5...S9
    // Sixth       BLU BLU BLU BLU GRN OFF OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00000111);               // 2nd blue
    break;        //  end of case 5
  case 6:           // s6
    // Seventh     BLU BLU BLU BLU GRN RED OFF OFF
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00000011);               // 2nd blue
    break;        //  end of case 6
  case 7:           // s7
    // Eigth       BLU BLU BLU BLU GRN RED RED OFF
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00000001);               // 2nd blue
    break;        //  end of case 7
  case 8:           // s7
    // Eigth       BLU BLU BLU BLU GRN RED RED RED
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00000000);               // 2nd blue
    break;        //  end of case 7
  }         // end of switch
}         // end of function
 #endif

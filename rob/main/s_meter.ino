#include "led_arc_board.h"
#include "s_meter.h"

//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  ++++++++++++++++++++++       get s-meter function     ++++++++++++++++++++++++++++++++++++++++++
//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// orphan from tab 5
void request_s_meter()
{
  tx_ci_v_via_serial_ttl(10);     // ci-v tx !

  bit_bash(10);       // sort out the nibbles

// new calc
  Icom.set_current_s_meter( ( (ci_v_rx_DATA[1])  *100 ) + ( (ci_v_rx_DATA[2] ) *10 ) + (  ci_v_rx_DATA[3] ) );

  // old calc
  // Icom.set_current_s_meter( ( Icom.get_s_meter_hundreds() *100) + (Icom.get_s_meter_tens() * 10 ) +  (Icom.get_s_meter_units()) ) ;

  LEDArc.calculate_current_MAPPED_value(Icom.get_current_s_meter() );    // ( Icom.get_current_s_meter() )
  // this returns an S-meter value, but also initiates the calcs inside the object to
  //generate the low, high and max1, max 2 indices


}
// end of orphan from tab 5


void S_meter2LED_index() {

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

}
// END OF FUNCION TOKEN 2 LED


//START OF LED ARC FUNCTION VARIATIONS
#ifdef LED_16_RGB
//  =======================================================================================================================================
//        was void LED16_rgb(byte led)   ....  renamaed to the generic void LED_arc(byte led) AND encapsulated
//  =======================================================================================================================================

void LED_arc(byte led, byte Max1, byte Max2) // LED_16_RGB
{        //start
  byte index = led;
  Serial.println();
  Serial.print( "Max1 = " );
  Serial.print( Max1 );
  Serial.print( "  Max2 = " );
  Serial.println( Max2 );

  byte MaxHold1 = Max1;        // make this an object variable instead of an argument to the function
  byte MaxHold2 = Max2;
  byte red1   = 0;
  byte green1 = 0;
  byte blue1  = 0;
  byte red2   = 0;
  byte green2 = 0;
  byte blue2  = 0;

  // byte led_pwm = shine ;





  switch ( index ) {       //switch
  case 0:          // s0
    // Zeroth LED ALL OFF
    red1   = B11111111;               // 0XFF 1st red
    green1 = B11111111;               // 0XFF 1st green
    blue1  = B11111111;               // 0XFF 1st blue.....off ALL OFF

    red2   = B11111111;               // 0XFF 2nd red
    green2 = B11111111;               // 0XFF 2nd green
    blue2  = B11111111;               // 0XFF 2nd blue
    break;        //  end of case 0


  case 1:          // s1
    // 1st LED only 1 BLUE
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B01111111;               // 0X7F 1st blue

    red2   = B11111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 1
  case 2:          // s2
    // 2nd LED only 2 BLUE
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00111111;               // 0X3F 1st - 2ND blue

    red2   = B11111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 2
  case 3:          // s3
    // 3rd LED only 3 BLUE
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00011111;               // 0X1F 1st -  3RD blue.....off

    red2   = B11111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 3
  case 4:          // s4
    // 4th LED only 4 BLUE
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00001111;               // 0X0F 1st - 4TH blue.....blue

    red2   = B11111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 4
  case 5:          // s5
    // Fifth LED only 5 BLUE
    red1   = B11110111;               // 1st red  *
    green1 = B11111111;               // 1st green
    blue1  = B00000111;               // 0X07 1st- 5TH  blue....blue

    red2   = B11111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 5
  case 6:           // s6
    // Sixth LED only 6 BLUE
    red1   = B11110111;               // 1st red  *
    green1 = B11111111;               // 1st green
    blue1  = B00000011;               // 0X03 1st - 6TH blue.....blue

    red2   = B11111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 6
  case 7:           // s7
    // Seventh LED only 7 BLUE
    red1   = B11110111;               // 1st red  *
    green1 = B11111111;               // 1st green
    blue1  = B00000001;               // 0X01 1st - 7TH  blue

    red2   = B11111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 7
  case 8:           // s8
    // Eigth LED only 8 BLUE
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 0X00 1st - 8TH blue

    red2   = B11111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 8

  // drive next PCB now ...

  case 9:           // s9
    // Nineth LED  8 BLUE + 1 GREEN
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 0X00 1st - 8TH blue

    red2   = B11111111;               // 2nd red
    green2 = B01111111;               // 2nd    ONLY  GREEN
    blue2  = B11111111;               // 2nd blue
    break;        //  end of case 9
  case 10:            // s9+10
    // Tenth LED  8 BLUE + 1 GREEN + 1 RED
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 0X00 1st blue
    red2   = B10111111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B01111111;               // 2nd blue
    break;        //  end of case 10
  case 11:             // s9+10
    // Eleventh LED 8 BLUE + 1 GREEN + 2 RED
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 0X00 1st blue
    red2   = B10011111;               // 0XDF 2nd red
    green2 = B11111111;               // 0XBF 2nd green
    blue2  = B01111111;               // 2nd blue
    break;        //  end of case 10
  case 12:              // s9+20
    // Twelth LED  8 BLUE + 1 GREEN + 3 RED
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 0X00 1st blue
    red2   = B10001111;               // 0XCF 2nd red
    green2 = B11111111;               // 0XBF 2nd green
    blue2  = B01111111;               // 2nd blue
    break;        //  end of case 20
  case 13:              // s9+30
    // Thirteenth LED  8 BLUE + 1 GREEN + 4 RED
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 1st blue
    red2   = B10000111;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B01111111;               // 2nd blue
    break;        //  end of case 30
  case 14:              // s+40
    // Fourteenth LED  8 BLUE + 1 GREEN + 5 RED
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 1st blue
    red2   = B10000011;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B01111111;               // 2nd blue
    break;        //  end of case 40
  case 15:              // s+50
    // Fifteenth LED  8 BLUE + 1 GREEN + 6 RED
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 1st blue
    red2   = B10000001;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B01111111;               // 2nd blue
    break;        //  end of case 50
  case 16:               // s+60
    // sixtenth LED  8 BLUE + 1 GREEN + 6 RED
    red1   = B11111111;               // 1st red
    green1 = B11111111;               // 1st green
    blue1  = B00000000;               // 1st blue
    red2   = B10000000;               // 2nd red
    green2 = B11111111;               // 2nd green
    blue2  = B01111111;               // 2nd blue
    break;        //  end of case 60
  }         // end of switch

  red1   = red1   &  MaxHold1;// combine ( AND ) NOW average value (red1) with PEAK to give White LED PEAK Indicator
  green1 = green1 &  MaxHold1;// MaxHold1 and MaxHold2 are derived from S.meter data
  blue1  = blue1  &  MaxHold1;// and converted to a bit mask for indeces 1-16... try and move MaxHold to object
  red2   = red2   &  MaxHold2;
  green2 = green2 &  MaxHold2;
  blue2  = blue2  &  MaxHold2;
//digitalWrite(STORE_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, red1); // 1st red
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, green1); // 1st green
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, blue1); // 1st blue
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, red2); // 2nd red
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, green2); // 2nd green
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, blue2); // 2nd blue
//digitalWrite(STORE_PIN, HIGH);
}         // end of function

   #endif


 #ifdef LED_16_MONO
//  =======================================================================================================================================
//        was void LED16_mono(byte led)   ....  renamaed to the generic void LED_arc(byte led) AND encapsulated
//  =======================================================================================================================================
//  16 MONO START ---->>>>>>>>> NOT TESETED YET  to be edited
void LED_arc (byte led)
{        //start
  byte index = led;
  // byte led_pwm = shine ;
  switch ( index ) {       //switch
  case 0:          // s0
    // Zeroth LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red   off   LHS
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green on..........on
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B01111111);               // 1st blue.....off
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue  mono .................. THIS ONE ????
    break;        //  end of case 1
  case 1:          // s1
    // first LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red....off
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00111111);               // 1st blue .....off
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 2
  case 2:          // s2
    // second LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red....off
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00011111);               // 1st blue.....off
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 1
  case 3:          // s3
    // Third LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00001111);               // 1st blue.....blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 3
  case 4:          // s4
    // Fifth LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000111);               // 1st blue....blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 4
  case 5:           // s5
    // Sixth LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000011);               // 1st blue.....blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 5
  case 6:           // s6
    // Seventh LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000001);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 6
  case 7:           // s7
    // Eigth LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd blue
    break;        //  end of case 7
  case 8:           // s8
    // Nineth LED only BLUEE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 8
  case 9:            // s+9
    // Tenth LED only BLUE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 9
  case 10:             // s+10
    // Eleventh LED only WHITE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11011111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 10
  case 11:              // s+20
    // Twelth LED only WHITE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11001111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 20
  case 12:              // s+30
    // Thirteenth LED only WHITE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000111);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 30
  case 13:              // s+40
    // Fourteenth LED only WHITE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000011);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 40
  case 14:              // s+50
    // Fifteenth LED only WHITE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000001);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 50
  case 15:               // s+60
    // sixtenth LED only WHITE
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st red
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);               // 1st green
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000);               // 1st blue
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000000);               // 2nd red
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111);               // 2nd green
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111);               // 2nd blue
    break;        //  end of case 60
  }         // end of switch
}         // end of function

   #endif


// save a copy

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
// END OF LED ARC FUNCTION VARIATIONS



// end of tab 6 LED switch
// start of tab 5_LED_loop
//wasin tab5.. void get_S_meter ()


// end of tab_5_led loop

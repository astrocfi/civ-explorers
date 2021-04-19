/*******************************************************************************
 * Author: Robert S. French <rfrench@rfrench.org>
 * Derived from work by John Rowling
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative
 * Commons, PO Box 1866, Mountain View, CA 94042, USA.
 ******************************************************************************/

#include "config.h"
#include "util.h"

#ifdef PRO_MINI
SoftwareSerial HostSerial(HOST_SERIAL_PIN_RX, HOST_SERIAL_PIN_TX);
#endif

void util_init()
{
  pinMode(HOST_SERIAL_PIN_RX, INPUT);
  pinMode(HOST_SERIAL_PIN_TX, OUTPUT);
  HostSerial.begin(57600);
  HostSerial.listen();
}

void host_print()
{
}

void host_print(unsigned long x, int format=DEC)
{
  HostSerial.print(x, format);
}

void host_print(const char *s)
{
  HostSerial.print(s);
}

void host_println()
{
  HostSerial.println();
}

void host_println(unsigned long x, int format=DEC)
{
  HostSerial.println(x, format);
}

void host_println(const char *s)
{
  HostSerial.println(s);
}


// *******************************************************************************
// ****************    function declarations    **********************************
// *******************************************************************************


void printDigits(int digits){
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
// send an NTP request to the time server at the given address

// start of tab G token 2 mode
byte token_2_mode (char mode_token ) // not used now as only TOKEN valid in this KEYBOARD object
{             // receives a (byte) mode value from button press then returns a (char) symbol to compare with symbol from controller
  int mode_id  = 99;

  switch (mode_token)
  {
  case 'a':                   // LSB
    mode_id = 0;
    break;

  case 'b':              // USB
    mode_id = 1;
    break;

  case 'c':                   // AM
    mode_id = 2;
    break;

  case 'd':                   // CW
    mode_id = 3;
    break;

  /*
     case 'e' :  // FSK / RTTY
     mode_id = 4 ;
     break ;
   */

  case 'f':                   // FM
    mode_id = 5;
    break;

  case 'g':                   // WFM
    mode_id = 6;
    break;

  default:
    mode_id = 99;


  }              // end of switch}
  return mode_id;
}

/*
 * Mode ID's      my symbol   (source ICOM )
 *    00      a    LSB
 *    01      b    USB
 *    02      c    AM
 *    03      d    CW
 *    04      e    RTTY / FSK
 *    05      f    FM
 *    06      g    WFM
 *    07      h    CW-R
 *    08      i    RTTY-R / FSK-R
 *    09      j
 *    10      k
 *    11      l    S-AM(D)
 *    12      m    PSK
 *    13      n    PSK-R
 *    14      o    S-AM(L)
 *    15      p    S-AM(U)
 *    16      q    P25
 *    17      r    D-STAR
 *    18      s    dPMR
 *    19      t    NXDN-VN
 *    20      u    NXDN-N
 *    21      v    DCR
 */
// end of tab G

// start of tab H mode 2 token
char mode_2_token (byte icom_mode )
{

  // mode_value =  icom_mode ;

  switch (icom_mode)
  {
  case 0:               // LSB
    return 'a';
    break;

  case 1:                // USB
    return 'b';
    break;

  case 2:               // AM
    return 'c';
    break;

  case 3:               // CW
    return 'd';
    break;
  /*
     case 4 :  // RTTY
     return 'e' ;
     break ;
   */

  case 5:               // FM
    return 'f';
    break;

  case 6:               // WFM
    return 'g';
    break;


  default:
    return '`';



  }                // end of switch}

}




byte high_nibble_unpacker (byte high_target) {  // unpacks the high nibble by using a bit mask

  byte two_nibbles =0;

  boolean bits4 =LOW;
  boolean bits5 =LOW;
  boolean bits6 =LOW;
  boolean bits7 =LOW;

  byte eights = 0;
  byte fours = 0;
  byte twos = 0;
  byte ones = 0;

  byte high_nibble_unpacked_bcd = 0;

  two_nibbles = high_target;         // get hold of the packed BCD byte

  //now pass the byte through and read the bits

  //now pass the byte through and read the bits
  // assumes bigendian
  bits7= two_nibbles & 0x80;
  high_nibble_unpacked_bcd = high_nibble_unpacked_bcd + bits7; high_nibble_unpacked_bcd = high_nibble_unpacked_bcd <<1;         //get bit 7 ?

  bits6= two_nibbles & 0x40;
  high_nibble_unpacked_bcd = high_nibble_unpacked_bcd + bits6; high_nibble_unpacked_bcd = high_nibble_unpacked_bcd <<1;

  bits5= two_nibbles & 0x20;
  high_nibble_unpacked_bcd = high_nibble_unpacked_bcd + bits5; high_nibble_unpacked_bcd = high_nibble_unpacked_bcd <<1;

  bits4= two_nibbles & 0x10;
  high_nibble_unpacked_bcd = high_nibble_unpacked_bcd + bits4;

  return high_nibble_unpacked_bcd;
}               // eof


/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   END OF HIGH NIBBLE FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//

byte low_nibble_unpacker (byte low_target) { // unpacks the low nibble by using a bit mask

  byte two_nibbles =0;

  boolean bits0 =LOW;
  boolean bits1 =LOW;
  boolean bits2 =LOW;
  boolean bits3=LOW;

  byte eights = 0;
  byte fours = 0;
  byte twos = 0;
  byte ones = 0;
  byte low_nibble_unpacked_bcd = 0;

  two_nibbles = low_target;

  // assumes bigendian
  two_nibbles = low_target;

  bits3= two_nibbles & 0x08;
  low_nibble_unpacked_bcd =low_nibble_unpacked_bcd + bits3; low_nibble_unpacked_bcd = low_nibble_unpacked_bcd <<1;

  bits2= two_nibbles & 0x04;
  low_nibble_unpacked_bcd =low_nibble_unpacked_bcd + bits2; low_nibble_unpacked_bcd = low_nibble_unpacked_bcd <<1;

  bits1= two_nibbles & 0x02;
  low_nibble_unpacked_bcd =low_nibble_unpacked_bcd + bits1; low_nibble_unpacked_bcd = low_nibble_unpacked_bcd <<1;

  bits0= two_nibbles & 0x01;
  low_nibble_unpacked_bcd =low_nibble_unpacked_bcd + bits0;

  return low_nibble_unpacked_bcd;

}          // eofumctiom


void bit_bash(byte control) {
  // basically this function will unpack BCD bytes... in conjunction with HIGH an LOW nibble unpacker

  byte target =0;
  byte local_command  =  control;    // determins if we are interested in frequ, mode or s-meter
  byte data_starts_at = ci_v_TX1_RX1_MESSAGEs  [local_command]   [11];      // gets the start position of REAL DATA indicated by entry in ci_v_RX1_MESSAGEs
  byte data_ends_at   = ci_v_TX1_RX1_MESSAGEs  [local_command]   [12];      //  gets the end position of REAL DATA...

  byte low_nibble_unpacked_bcd = 0;
  byte high_nibble_unpacked_bcd = 0;

  //unsigned long vco_frequency = 0 ;

  boolean drawDots = false;

  // clear array

  memset(ci_v_rx_DATA, 0, 15);

  for ( byte i = data_starts_at; i <= data_ends_at; i++ )
  {
    /*
       if (local_command == 8)  // frequency
                  {
                  band_store[Icom.get_current_band()][i-data_starts_at]=RX1_MESSAGE_BUFFER[ i ] ; // copy frequ data bytes into band store
                  }
     */


    target = RX1_MESSAGE_BUFFER[ i ];                     //   [8] to [12]  gets a packed BYTE from a global array....freq = [11] to [15]

    high_nibble_unpacked_bcd = high_nibble_unpacker (target);                    // calls unpacker function to extract bcd data from the packed byte and place it in the LOWER nibble of a new unpacked byte called 'high_nibble_unpacked_bcd'

    low_nibble_unpacked_bcd = low_nibble_unpacker (target);                // calls unpacker function to extract bcd data from the packed byte and place it in the LOWER nibble of a new unpacked byte called 'low_nibble_unpacked_bcd'

    ci_v_rx_DATA[((i-data_starts_at)*2)+0] = high_nibble_unpacked_bcd;                    // write to  [   ]  [0] ... 2...4...6...8...
    ci_v_rx_DATA[((i-data_starts_at)*2)+1] = low_nibble_unpacked_bcd;                    // write to  [   ]  [1] ... 3...5...7...9...

  }                      // end of for loop
}

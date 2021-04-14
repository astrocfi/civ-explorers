
// start of tab : RGB16_-_BRIDGE...
// ***********************************************************************
//                    Source File info***
// ***********************************************************************
// FILE Name : RGB16_-_FBB_rev_02_or_03pcb_net-0_MEGAorPRO_MAR_06_2021              <------ 
// Author    : JOHN ROWING                                                     
// Date      : 06 MAR 2021                                                   <------
// Version   :                                 
// Notes     : most serial.print statements deleted
//


// **********************************************************************
// file id shows on boot
// ********************************************************************** 
#define rev "RGB16_-_FBB_rev_02_or_03pcb_net-0_MEGorPRO_MAR_06_2021" //           <------
// **********************************************************************
     //   NOTES
// **********************************************************************
/* tweaking the mode & band selection  - seems to work 
// now tweak this MEGA version for PRO
// there might be a 7300 anomoly, and it isn't spotting NO RADIO CONNECTED
// added PEAK hold indicator

*/


// **********************************************************************
// Hardware Dependance  - YOU MUST CHECK THESE !                          <------

// **********************************************************************
//            MICROCONTROLLER TYPE
// **********************************************************************
// select arduino type
#define MEGA2560    1                             //                       <------

//#define PRO_MINI 1
// **********************************************************************

// **********************************************************************
//            select pcb version 
// **********************************************************************
 //#define rev2pcb    1        // un rem for earlier pcb                   <------
              
 #define rev3pcb    1        // un rem for later pcb

// **********************************************************************
// rem out unwanted LED definition prior to compilation
// **********************************************************************
                        // #define LED_NONE   1
                        // #define LED_8_MONO   1
                        // #define LED_16_MONO  1
                        // #define LED_8_RGB    1     // un rem for LED type on LED pcb          <------
 #define LED_16_RGB   1

#include <Arduino.h>
#include <SPI.h>

#include <Adafruit_MCP4725.h>

//#include <TM1638plus_Model2.h>
//#include <TM1638plus.h>

#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Wire.h> 
#include <RingBuf.h>
#include <SoftwareSerial.h> 

#ifdef MEGA2560
#include <Ethernet2.h>
#endif
// function prototypes here in this format 
// type name ( arg type ) ;

// ***************************************
// ********  function prototypes  ********
// ***************************************

void moving_coil (byte) ;
byte token_2_mode (char ) ;
char mode_2_token (byte) ;
char Scan_buttons() ;
void request_mode() ;

void LED_arc(byte,byte,byte) ;
void LED_test_white(byte) ;
void S_meter2LED_index() ;
byte listen_TTL_Serial_port () ;
byte g0ld_read_Unsolicited_Message() ;
byte read_Unsolicited_Message() ;
byte tx_ci_v_via_serial_ttl(byte) ;
byte high_nibble_unpacker (byte) ;
byte low_nibble_unpacker (byte) ;
void bit_bash(byte) ;
unsigned long get_frequency() ;
void request_s_meter () ;
void Controller_mode_Logic () ;
byte initialise_time() ;
bool test_time() ;
void split_number (int) ;
void split_year (int) ;
void split_month (int) ;
void split_day (int) ;
void split_hour (int) ;
void split_minute (int) ;
uint8_t HexToBCD(uint8_t) ;
byte remote_control ( byte) ;
void clear_buffers() ;




#ifdef MEGA2560
 #define IP_toggle_pin  24     // set IP and MAC address based on the resistor during SETUP 
#endif

#ifdef PRO_MINI
#define IP_toggle_pin  12     // set IP and MAC address based on the resistor during SETUP 
#endif 


// **********************************************************************
//         NETWORK SHEILD  TYPE
// **********************************************************************
//  NETWORK CARD TYPE = REV n0ne  ... check the includes !!!                  <------
// ********************************************************************** 


// select radio  model  - use this to modify the command list for mode change
 #define RADIO_8600  0x96                                  //                <------
 #define RADIO_7300  0x94
 #define RADIO       0x00

 #define CI_V_TX_pin     10  // ?  soft serial ?/?
 #define CI_V_RX_pin     11  // ?  ""

// *******************************************************************************
//            LED METER CHARACTERISTICS
// *******************************************************************************

                #define LED_8_ATTACK      0   // ms  was 10 
                #define LED_8_SUSTAIN     2   // ms  was 10                 
                #define LED_8_DECAY       0   // ms  was 30

                #define LED_16_ATTACK     0   // ms
                #define LED_16_SUSTAIN    1   // ms  was 10  
                #define LED_16_DECAY      2
                #define LED_PEAK_TTL     20




// **********************************************************************
//                                                Diagnostic Switches
// **********************************************************************
 //#define PRINTSHOW   1
 //#define PRINTSHOWCIVRX 1
 //#define PRINTSHOWCIVTX 1


   // #define SHOW_HI_LO    1 // S-meter data 
   // #define SHOW_CI_V_TX     1 // TX data   
   // #define SHOW_CI_V_RX    1  


  // #define debug1   1 // display bit dissassembly
  // #define debug2    1 // display TX and RX processes in CI-V via serial ttl

// the impact is seen below.
// if I comment out either or both of the above #define compiler macro directives 
// the code behaves differently
// **********************************************************************



// **********************************************************************
//                                                MICROCONTROLLER type
// **********************************************************************
 #ifdef MEGA2560
 // example... if it's a 2560 compile this code
 #endif

  #ifdef rev3pcb
 // example... if it's a rev 3 pcb compile this code
  #endif
 
// **********************************************************************


// *******************************************************************************
//                                                  object        
// *******************************************************************************




Adafruit_7segment matrix = Adafruit_7segment();





// *******************************************************************************
// ****************                   globals           **************************
// *******************************************************************************

#define RED       49
#define GREEN     41
#define BLUE      45
#define YELLOW1   33
#define YELLOW2   37
#define WHITE     29



// from Button board======================================
// Define string with response to Master
String answer = "{current_modeq_req_modec_r1234}";

byte NTP_PACKET_SIZE = 48 ;
unsigned long current_frequency = 0 ;
char VCO[11]= "9876543210" ;
unsigned long startMillis ;
unsigned long stopMillis ;
unsigned long diffMillis ;



unsigned long currentMillis ;
unsigned long previousMillis ;
const long interval = 100 ;
byte flash = false ;

uint8_t minutes_NTP ;


char RX_MILLIS[11] ; // store incoming millisecond count received from controller

byte rx_byte ; 
byte tx_byte ;

#define CONTROLLER_ID   240  //E0h
#define COMMUTATOR  5 //  will count 1-4 ...1..2..3..4..4..1..2..3..4..4


#define KBD_REPEAT_TX_DELAY  1  // not used ?

#define BUTTONS   A0  // analogue input
//#define Band_BUTTONS   A1  // analogue input 

#define BAND_JUMP   20  //  was 6 number of counts round loop before band jump iniiated when mode button held down 
 
#define HOLDOFF_RATIO 4    //  was 45 number of loops between Sending CI-V messages poling the radio   .... about 22s per loop

#define UNSOLICITED_HOLD_OFF 25

#define FLASHINTERVAL 100

#define RX_DELAY   5 //  was 50  allow time to fill RX buffer
#define TX_DELAY   5// was 75

#define LOOP_DELAY  1  // 



#define CIV_RX_SOLICITED_BUFFER_FILL_DELAY  30  // was 26 if delay too short the reply might be cropped and appear to be a collision !                              
#define CIV_RX_UNSOLICITED_BUFFER_FILL_DELAY  10  // was 50 

// *******************************************************************************
// mode button tx row definitions - see array byte ci_v_TX1_RX1_MESSAGEs[21][9] below
// ******************************************************************************* 
#define MODE_A  'a'   // TX row 1 for LSB.....ascii 97
#define MODE_B  'b'   // TX row 2 for USB...........98
#define MODE_C  'c'   // TX row 3 for AM ...........99
#define MODE_D  'd'   // TX row 4 for CW  ..........100
// #define MODE_D  'f'   // TX row 5 for FM

// ***********************************************************************************
// band definitions tx row definitions - see array byte ci_v_TX1_RX1_MESSAGEs[21][9] below
// *********************************************************************************** 
#define BAND_A  'k'   // hold down Mode Button 1 for 3 seconds to TX row 11 for 160m .... ascii 107
#define BAND_B  'l'   // hold down Mode Button 2 for 3 seconds to TX row 12 for  80m
#define BAND_C  'm'   // hold down Mode Button 3 for 3 seconds to TX row 13 for  40m
#define BAND_D  'o'   // hold down Mode Button 4 for 3 seconds to TX row 15 for  20m  .....15 + 96 = 111 = 'o'
// #define BAND_D  'n'   // for row 14

// ***********************************************************************************
// HAM BAND Boundary definitions
// ***********************************************************************************                             
#define LO_160M  1810000       //    //   BAND=1; TX row 11        
#define HI_160M  2000000                              

#define LO_80M  3500000       //     //    BAND=2  TX row 12       
#define HI_80M  3800000

#define LO_40M  7000000        //    //   BAND=3  TX row 13      
#define HI_40M  7200000

#define LO_30M  10100000        //   //  BAND=4  TX row 14         
#define HI_30M  10150000

#define LO_20M  14000000       //    //   BAND=5  TX row 15      
#define HI_20M  14350000                 

#define LO_17M  18068000       //    //   BAND=6  TX row 16      
#define HI_17M  18168000 

#define LO_15M  21000000       //     //   BAND=7  TX row 17      
#define HI_15M  21450000 

#define LO_12M  24890000       //     //   BAND=8  TX row 18      
#define HI_12M  24990000 

#define LO_10M  28000000       //     //   BAND=9  TX row 19     
#define HI_10M  29700000 

#define LO_6M   50000000       //     //  BAND=10  TX row 20      
#define HI_6M   52000000 
// *******************************************************************************             


// *******************************************************************************
// Data processing aid
// *******************************************************************************
#define VCOF0  35 // position in array
#define VCOF1  34
#define VCOF2  33
#define VCOF3  32
#define VCOF4  31
#define VCOF5  30
#define VCOF6  29
#define VCOF7  28
#define VCOF8  27
#define VCOF9  26 
// *******************************************************************************


// *******************************************************************************
// I/O pins on the Arduino connected to strobe, clock, data, for TM1638 display
// *******************************************************************************
 #ifdef MEGA2560
                #define  STROBE_TM 16 //  orange strobe     was 12 ... clash with ethernet shield which uses 10,11,12,13
                #define  CLOCK_TM 15  // yellow clk         was 11
                #define  DIO_TM 14    // green dio          was 10..... 
 #endif               
// *******************************************************************************


// *******************************************************************************
// I/O pins on the Arduino for CI-V interface PCB & LED PCB  (version dependant)
// *******************************************************************************
 #ifdef rev2pcb
                // shift register drive
                #define  PWM_PIN    7 //was 9 now 7  ( 06 march 2021 )
                #define  STORE_PIN  8 
                #define  CLOCK_PIN  9 //was 7 now 9  ( 06 march 2021 )
                #define  DATA_PIN   6
                
                #define LSB_LED_PIN 2 
                #define AM_LED_PIN  4 // was 3
                #define USB_LED_PIN 3 // was 4
                #define CW_LED_PIN  5 
 #endif

 #ifdef rev3pcb
                // shift register drive via alternative pins from the arduino
                #define  PWM_PIN    9  //was 7 now 9  ( 06 march 2021 )
                #define  STORE_PIN  7  //was 9 now 7  ( 06 march 2021 )
                #define  CLOCK_PIN  8
                #define  DATA_PIN   6
                
                #define LSB_LED_PIN 5 
                #define AM_LED_PIN  4 
                #define USB_LED_PIN 3 
                #define CW_LED_PIN  2 
 #endif


byte commutator = 0 ;

byte READ1_COUNTER = 0 ;
 
bool swap_nibbles = false; //Optional , Default is false if left out, see note in readme at URL                              

// swap_nibbles = boolean default false, if true swaps nibbles on display byte
  // TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles);
  
// TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM);



byte RX1_MESSAGE_BUFFER[48] ;
byte TX1_MESSAGE_BUFFER[20] ;  // used to record WHAT CI-V message was sent (up to 20 characters)... is that enough ?  

byte RX1_COUNTER = 0 ;
byte TX1_COUNTER = 0 ;

byte RX3_MESSAGE_BUFFER[48] ;  // comms from UNO ... will hold settings / changes for objects attributes
byte TX3_MESSAGE_BUFFER[48] ;  // comms to UNO 

byte RX3_COUNTER = 0 ;
byte TX3_COUNTER = 0 ;






// move these flags to radio object

bool FIRST_FE_flag = false ;  // could make all theses flags part of the object
bool SECOND_FE_flag = false ;
bool FD_flag = false ;
bool ID_flag = false ;



//char test = "0001456" ;

bool round_one = false ;



/*
 * The band_store[ ] [ ] array has the "current" VCO frequency written to the relevent rows i.e. when the VCO changes, 
 * the LAST VCO value ( 5 bytes ) are written into row zero band_store [ 0 ] [ x ] 
 * However, if the VCO fequency value is within the defined band boundaries, the 5 bytes are also written into the relevent row
 * *********To find the code search for "copy frequency  ( 5 bytes ) from band store to array" *********
 */

// band store  // put these into TX message array !
//  so                      10Hz .. 1Hz    1kHz ..  100Hz   100kHz .. 10kHz     10MHz .. 1MHz    1GHz .. 100MHz
//                               00             00                 00                 00              00
byte band_store[11][5] = {
                          {    0x00,           0x00,             0x80,              0x00,          0x00   }, // band 0   by frequency ... got to store 10 nibbles in 5 bytes !
                          {    0x00,           0x50,             0x60,              0x01,          0x00   }, // band 1  1.8   ... these will store the last frequency used 
                          {    0x00,           0x00,             0x68,              0x03,          0x00   }, // band 2    3.6300 MHz  ... entred as 0  80  104  03  
                          {    0x00,           0x50,             0x01,              0x07,          0x00   }, // band 3   7.00         ... or as HEX 
                          {    0x00,           0x50,             0x01,              0x02,          0x00   }, // band 4  10.680   ... which realy is  5x two 4 bit BCD nibbles
                          {    0x00,           0x50,             0x12,              0x14,          0x00   }, // band 5  14.1
                          {    0x00,           0x80,             0x06,              0x18,          0x00   }, // band 6     18MHz .. 24dec = 18hex = 1bcd & 8bcd
                          {    0x00,           0x00,             0x00,              0x21,          0x00   }, // band 7
                          {    0x00,           0x00,             0x00,              0x24,          0x00   }, // band 8
                          {    0x00,           0x00,             0x00,              0x28,          0x00   }, // band 9
                          {    0x00,           0x00,             0x00,              0x50,          0x00   }  // band 10
                          } ;  
                                                           
 //  frequency data reply is 5 bytes, starting at the 6th byte (70)  to the 10th byte (00)  but each nibble is ONE BCD digit
                  //nibble A       B      A        B          A        B         A       B         A         B
                  //  so  10Hz .. 1Hz    1kHz .. 100Hz      100kHz   10kHz      10MHz   1MHz       1GHz    100MHz
                  //       byte 1            byte 2             byte 3             byte 4             byte 5 

// 10 to the power         1       0      3        2          5        4         7       6          9       8    


 /*                          
  *  mode store isn't currently used                          
  */

byte mode_store[11] = { 0,0,0,0,0,1,0,0,0,0,0 } ;



                                              

 /* This array "ci_v_TX1_MESSAGEs[25][11]" is used to assemble the apprpriate CI-V message ( which is then sent to the radio ).  
 The Header, the TO and FROM address is added in "tx_ci_v_via_serial_ttl" function.
 * 
 * The bytes values in the first column are (s)end Length of the TX'd message e.g. to request the VCO current frequency, the assembled message
 *  will be "FE FE 96 E0 03 FD"  which is 6 bytes.  When sent they are looped back into the RX buffer
 * so the RX Buffer will contain a copy ... and about 20ms later the reply  ( of Length R .. the 2nd column ) will be appended to the copy of the
 *  TX message so that the "RX1_MESSAGE_BUFFER[rx_count]" would for example contain the following bytes
 * 
 * RX1_MESSAGE_BUFFER[n] contents :      |----SENT------|------------reply--------------|
 *                                     "FE FE 96 E0 03 FD FE FE 96 E0 03 aa bb cc dd ee FD
 *                                       1  2  3  4  5  6                                         send length = 6 bytes                                   
 *                                                        1  2  3  4  5  6  7  8  9  10 11        reply length = 11 bytes 
 * 
 * 
 * The Send and Reply length values are used to direct checking and ( I think ) data extraction in "bit_bash( )" and associated functions.
 * 
 */

 /*  add start byte, to from bytes then TX each byte up to and including 253 byte counting from byte 1 (not zero !) 
 * ...positions in RX buffer : add on S= send length (including sent FD)... R = receive length (including final replied FD)
 *    
 *   St    En   Rp // St is stard position (counting from 0) of data in the RX BUFFER ( which also has the TX message which requested the data) 
 *  En is end of data before the final FD character, Rt is reply type  0 = none  1 = FB   2  = data
 */

 byte ci_v_TX1_RX1_MESSAGEs[45][14]={        // was 25 x 14
                                             // add data payload start (St) and end (En) position in RX Buffer
  //      S   R       M0     M1     M2      M3     M4     M5      M6     M7    M8   St    En    Rt      
      {   0,  0,      0,      0,     0,     0,     0,      0,     0,     0, 0xFD,    0,    0,   0}, // row 0   // note ... do not send this ... block '0' in tx function
      {   7,  6,   0x06,   0x00,  0xFD,     0,     0,      0,     0,     0,    0,   11,   11,   1}, //  row 1 set to LSB (change to mode)  reply is only 6 bytes FE FE E0 96 FB FD .                            
      {   7,  6,   0x06,   0x01,  0xFD,     0,     0,      0,     0,     0,    0,   11,   11,   1}, //      2   "  USB  (change to mode)     ditto
      {   7,  6,   0x06,   0x02,  0xFD,     0,     0,      0,     0,     0,    0,   11,   11,   1}, //      3   "  AM   (change to mode)     ditto
      {   7,  6,   0x06,   0x03,  0xFD,     0,     0,      0,     0,     0,    0,   11,   11,   1}, //      4   "  CW   (change to mode)     ditto
      {   7,  6,   0x06,   0x04,  0xFD,     0,     0,      0,     0,     0,    0,   11,   11,   1}, //      5   "  FSK  (change to mode)     ditto +++++++++ different 7300 (RTTY ?)
      {   7,  6,   0x06,   0x05,  0xFD,     0,     0,      0,     0,     0,    0,   11,   11,   1}, //      6   "  FM   (change to mode)     ditto
      {   7,  6,   0x06,   0x06,  0xFD,     0,     0,      0,     0,     0,    0,   11,   11,   1}, //      7   "  WFM  (change to mode)     ditto ---------------- not 7300
      {   6,  11,  0x03,   0xFD,     0,     0,     0,      0,     0,     0,    0,   11,   15,   2}, //  8 what current frequency 6 bytes sent reply 11 bytes FE FE E0 96 03 a b c d e FD
      {   6,  8,   0x04,   0xFD,     0,     0,     0,      0,     0,     0,    0,   11,   12,   2}, //  9 what current mode ... 6 bytes sent 
      {   7,  9,   0x15,   0x02,  0xFD,     0,     0,      0,     0,     0,    0,   13,   14,   2}, //  10 what S-meter value ? ..... 7 bytes sent 

      {  11,  6,   0x05,   0x00,  0x50,  0x60,  0x01,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 11 set band 1 (160m) frequency  11 bytes sent  6 bytes reply  FE FE E0 96 FB FD 
      {  11,  6,   0x05,   0x00,  0x00,  0x68,  0x03,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 12 set band 2 (80m) frequency  11 bytes sent ..  6 bytes reply  FE FE E0 96 FB FD  
      {  11,  6,   0x05,   0x00,  0x50,  0x02,  0x07,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 13 set band 3 (40m) frequency  11 bytes sent ..  6 bytes reply  FE FE E0 96 FB FD
      {  11,  6,   0x05,   0x00,  0x50,  0x01,  0x12,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 14 set band 4 (30m) frequency  11 bytes sent ... 6 bytes reply  FE FE E0 96 FB FD
      {  11,  6,   0x05,   0x00,  0x50,  0x01,  0x14,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 15 set band 5 (20m) frequency  11 bytes sent ..  6 bytes reply  FE FE E0 96 FB FD
      {  11,  6,   0x05,   0x00,  0x51,  0x12,  0x15,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 16 set band 5 (20m) frequency  11 bytes sent ..  6 bytes reply  FE FE E0 96 FB FD
      {  11,  6,   0x05,   0x00,  0x52,  0x12,  0x16,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 17 set band 5 (20m) frequency  11 bytes sent ..  6 bytes reply  FE FE E0 96 FB FD
      {  11,  6,   0x05,   0x00,  0x53,  0x12,  0x17,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 18 set band 5 (20m) frequency  11 bytes sent ..  6 bytes reply  FE FE E0 96 FB FD
      {  11,  6,   0x05,   0x00,  0x54,  0x12,  0x18,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 19 set band 5 (20m) frequency  11 bytes sent ..  6 bytes reply  FE FE E0 96 FB FD
      {  11,  6,   0x05,   0x20,  0x55,  0x12,  0x19,   0x00,  0xFD,     0,    0,   15,    15,  1}, // 20 set band 5 (20m) frequency  11 bytes sent ... 6 bytes reply  FE FE E0 96 FB FD
      {  9,  13,   0x1A,   0x05,  0x01,  0x31,   253,      0,     0,     0,    0,   17,    20,  2}, // 21 enquire  date   (8600) different for (7300)    xx bytes sent ... xx bytes reply     
      {  9,  13,   0x1A,   0x05,  0x01,  0x32,   253,      0,     0,     0,    0,   17,    18,  2}, // 22 enquire time    (8600) different for (7300) bytes sent ... xx bytes reply      
      {  13,  6,   0x1A,   0x05,  0x01,  0x31,  0x20,   0x20,  0x10,  0x16, 0xFD,   17,    17,  1}, // 23 set  date  13 bytes sent ... 6 bytes reply  FE FE E0 96 FB FD 
      {  11,  6,   0x1A,   0x05,  0x01,  0x32,  0x13,   0x45,  0xFD,     0,    0,   15,    15,  1}, // 24 set time 11 bytes sent   ... 6 bytes reply  FE FE E0 96 FB FD 

      {   7,  8,   0x16,   0x40,  0xFD,     0,     0,      0,     0,     0,    0,   13,   13,   2}, // 25 ... enquire  NR OFF / ON  reply is 7+2 
      {   7,  9,   0x14,   0x06,  0xFD,     0,     0,      0,     0,     0,    0,    0,    0,   2}, // 26 ... enquire NR LEVEL  0 - 255 reply is 7 + 1
      {   7,  8,   0x16,   0x22,  0xFD,     0,     0,      0,     0,     0,    0,    0,    0,   2}, // 27 ... enquire NB OFF / ON
      {   9, 10,   0x1A,   0x05,  0x01,  0x76,  0xFD,      0,     0,     0,    0,    0,    0,   2}, // 28 ... enquire NB DEPTH  00- 09  (8600) different for (7300) 
      {   9, 11,   0x1A,   0x05,  0x01,  0x77,  0xFD,      0,     0,     0,    0,    0,    0,   2}, // 29 ... enquire NB WIDTH  0 - 255 (8600) different for (7300)
      {   7,  9,   0x14,   0x12,  0xFD,     0,     0,      0,     0,     0,    0,    0,    0,   2}, // 30 ... enquire NB LEVEL  0 - 255
      {   7,  8,   0x16,   0x41,  0xFD,     0,     0,      0,     0,     0,    0,    0,    0,   2}, // 31 ... enquire NOTCH AUTO OFF / ON
      {   7,  8,   0x16,   0x48,  0xFD,     0,     0,      0,     0,     0,    0,    0,    0,   2}, // 32 ... enquire NOTCH MANUAL
      {   7,  8,   0x16,   0x57,  0xFD,     0,     0,      0,     0,     0,    0,    0,    0,   2}, // 33 ... enquire NOTCH WIDTH 00 - 02
      {   7,  9,   0x14,   0x0D,  0xFD,     0,     0,      0,     0,     0,    0,    0,    0,   2}, // 34 ... enquire NOTCH POSITION 0 - 255,,,,,,,,,

      {   8,  6,   0x16,   0x40,  0x00,  0xFD,     0,      0,     0,     0,    0,   12,   12,   1}, // 35 ... set  NR OFF / ON ... reply is only 6 bytes FE FE E0 96 FB FD .
      {   9,  6,   0x14,   0x06,  0x00,  0x00,  0xFD,      0,     0,     0,    0,    0,    0,   1}, // 36 ... set NR LEVEL  0 - 255          ditto
      {   8,  6,   0x16,   0x22,  0x00,  0xFD,     0,      0,     0,     0,    0,    0,    0,   1}, // 37 ... set NB OFF / ON                ditto
      {   9,  6,   0x1A,   0x05,  0x01,  0x76,  0xFD,      0,     0,     0,    0,    0,    0,   1}, // 38 ... set NB DEPTH  00- 09  (8600 M4=76) different for (7300 M4=89) 
      {   9,  6,   0x1A,   0x05,  0x01,  0x77,  0xFD,      0,     0,     0,    0,    0,    0,   1}, // 39 ... set NB WIDTH  0 - 255 (8600 M4=77) different for (7300 M4=90)
      {   9,  6,   0x14,   0x12,  0x00,  0x00,  0xFD,      0,     0,     0,    0,    0,    0,   1}, // 40 ... set NB LEVEL  0 - 255          ditto
      {   8,  6,   0x16,   0x41,  0x00,  0xFD,     0,      0,     0,     0,    0,    0,    0,   1}, // 41 ... set NOTCH AUTO OFF / ON        ditto
      {   8,  6,   0x16,   0x48,  0x00,  0xFD,     0,      0,     0,     0,    0,    0,    0,   1}, // 42 ... set NOTCH MANUAL               ditto
      {   8,  6,   0x16,   0x57,  0x00,  0xFD,     0,      0,     0,     0,    0,    0,    0,   1}, // 43 ... set NOTCH WIDTH 00 - 02        ditto
      {   9,  6,   0x14,   0x0D,  0x00,  0x00,  0xFD,      0,     0,     0,    0,    0,    0,   1}  // 44 ... set NOTCH POSITION 0 - 255     ditto
                               };
                               // need 10 more rows
                               
                               // command default is for R8600, 7300 is different ...change in set up   // command default is for R8600, 7300 is different 

/*
 *               CMD     SUB    SUB   SUB     YY    YY     MM      DD    253
 * {  11, 15,   0x1A,   0x05,  0x01,  0x31,   20,   20,    12      01    253},   // 23 set  date         xx bytes sent ... xx bytes reply       
 * 
 *                                            HH        MM    253
   {  11, 15,   0x1A,   0x05,  0x01,  0x32,   23,       59,    253,        0}    // 24 set time          xx bytes sent ... xx bytes reply
 */


/* 
 *  This array "ci_v_rx_DATA[11]" is used to store the DATA and is used with Bit_Bash to extract the BCD values when appropriate
 */
byte ci_v_rx_DATA[15] =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


// frequency message example    {FE, FE,000, 94,000, 70, 86,  6,  6,000, FD }
// adding missing leading zeros {FE, FE, 00, 94, 00, 70, 86, 06, 06, 00, FD } 
// this equates as follws:      FEFE(mesage start)  00(to any listener)  94(from R8600)  now bcd frequency Hz to GHz  006,068.640 MHz

// 2020JAN05 .. what if the FREQ data was solicited....
// will it be like this :       {FE, FE, E0, 94, 03, 70, 86, 06, 06, 00, FD } i.e. 3rd byte is E0 instead of the broadcast 00 'to' address

//  frequency data reply is 5 bytes, starting at the 6th byte (70)  to the 10th byte (00)  but each nibble is ONE BCD digit
                  //nibble A       B      A        B          A        B         A       B         A         B
                  //  so  10Hz .. 1Hz    1kHz ..  100Hz     100kHz   10kHz     10MHz   1MHz       1GHz    100MHz
                  //       byte 1            byte 2             byte 3             byte 4             byte 5 

// 10 to the power         1       0      3        2          5        4         7       6          9       8    



//    there this example is  00 06 06 86 70 00   =  6.0686.70 Mhz

                               
// jan 2019  array size increased from 22x13 to 23x13 and ADDed  an new row to call for S-meter value  at index 11, jammer moved to index 22

// FE,FE is preamble, ra is radio address, EO is controller address, cn is command number, sc is sub command number, dt is data area, FD is end of message code                    
// check that collision flag is false

// *******************************************************************************
// ****************         end of globals    ************************************
// *******************************************************************************


// *******************************************************************************
// ****************      define controller object       **************************
// *******************************************************************************

class Controller
                     
                      {  // CLASS MEMBER VARIABLES - these are initialised at start up
      
                        public : 
                                Controller( byte hex_id) ; //  
                                void begin();
                      
                                byte get_hex_id();
                                void set_hex_id(byte hex_id) ;

                                
                                void set_CONTROLLER_Operating_Mode(int8_t) ;  // not used as intended
                                int8_t get_CONTROLLER_Operating_Mode() ;

                                void set_CONTROLLER_LOOP_interval(long) ; // not used 
                                long get_CONTROLLER_LOOP_interval() ;
                                
                                void set_local_IP_port_and_MAC_swap(bool) ; 
                                bool get_local_IP_port_and_MAC_swap() ; 
                              
                                void set_listen_for_CI_V_message_flag(bool);
                                bool get_listen_for_CI_V_message_flag();
      
                                void set_just_sent_CI_V_message_flag(bool);
                                bool get_just_sent_CI_V_message_flag();                          
          
                                void set_CI_V_TX_inhibit_flag(bool);
                                bool get_CI_V_TX_inhibit_flag();
      
                                void set_tx_verified_flag(bool);
                                bool get_tx_verified_flag();
      
                                void set_TX_MESSAGE_NOT_CORRUPT_flag(bool);  
                                bool get_TX_MESSAGE_NOT_CORRUPT_flag();
      
                                void set_collision_flag(bool) ; 
                                bool get_collision_flag() ;
                                                                
                                void set_tx_jam_code_flag(bool) ;
                                bool get_tx_jam_code_flag();
                                                                
                                void set_rx_jam_code_flag(bool) ; 
                                bool get_rx_jam_code_flag() ;

                                //holdoff
                                bool get_CI_V_TX_HOLDOFF_flag();                // get the flag state AND decrimentinternal counter
                                byte set_CI_V_TX_HOLDOFF_counter(byte count) ;  // time2live byte
                                byte read_CI_V_TX_HOLDOFF_counter() ;           // read the current value of the internal counter



                                void set_dayz(byte dayz)  ;
                                byte get_dayz()  ;
      
                                void set_monthz(byte monthz)  ;
                                byte get_monthz()  ;
      
                                void set_yearLowz(byte yearLowz)  ;
                                byte get_yearLowz()  ;   
      
                                void set_yearHighz(byte yearHighz)  ;
                                byte get_yearHighz()  ; 
      
                                
                                                      
      
                                void set_hourz(byte hourz)  ;
                                byte get_hourz()  ;
      
                                void set_minz(byte minz)  ;
                                byte get_minz()  ;

                                void set_ntp_valid_flag(bool) ; 
                                bool get_ntp_valid_flag() ;



                        private :             
                               byte _hex_id ;

                               long _CONTROLLER_LOOP_interval ; // new

                               int8_t _CONTROLLER_operating_mode ;
                               byte _CONTROLLER_hardware_local_remote_toggle ; 
                               // define pin to test for high / low which sets ip & mac via :  bool _CONTROLLER_local_IP
                               bool _CONTROLLER_local_IP_port_mac_flag ; 
                               // TRUE = local IP, port & MAC    FALSE = remote IP, port & MAC

                               boolean _listen_for_CI_V_message_flag ; 
                               // = false   // listening is monitoring the CI-V one wire bus for traffic 
                               boolean _receive_flag ; // = false  
                               // this flag indicates receiving rather than listening
                               boolean _just_sent_CI_V_message_flag ; // = false 
                                // a message has just been sent ... and will be checked by listening / receiving the looped back data stream 
                               boolean _tx1_inhibit_flag ;  // = true  
                               // default is true i.e. do not transmit ... other checks will set this to false to allow transmission to occur ONCE
                               boolean _tx_verified_flag ; // 
                               boolean _TX_MESSAGE_NOT_CORRUPT_flag ;  
                               // = false  // this flag indicates received datagram is valid  i.e. FE FE ... and FD  received ..IS THERE A CHECKSUM too ?
                               boolean _collision_flag ; // = false  
                               //  when true = a  was collision detected  .....  false = no collision occured when WE sent data 
                               boolean _tx_jam_code_flag ; //  = false 
                               // jamming code sent( or to be sent ? ) because a collision was detected.
                               boolean _rx_jam_code_flag ; // = false 
                               // jamming code received
                              //holdoff
                               boolean _CI_V_TX_HOLDOFF_flag ; 
                               byte    _CI_V_time2live ;

                              // ntp
                              boolean _ntp_valid_flag ; //
                                
                              byte _day ;
                              byte _month ;
                              byte _yearHigh ;
                              byte _yearLow ;
    
                              byte _mins ;
                              byte _hours ;

                               

                      };       // end of object variable definition
                               // now define the methods

Controller::Controller( byte hex_id)  //, int8_t Button_switch_pin, int8_t Button_led_pin ) // this is a constructor.. local sounder is the o/p pin 
                                                                                            {
                                                                                             // 
                                                                                              _hex_id = hex_id;
                                                                                            }
                                                                                            
       void Controller::begin() // initialises the device
            {
              _CONTROLLER_operating_mode = 1 ;    // now start in mode 1 = initialising
              _CONTROLLER_LOOP_interval = 10 ; 
              _CONTROLLER_hardware_local_remote_toggle = 0 ;//IP_toggle_pin ;  // use this to read pin31 high or low to set IP and MAC
            _listen_for_CI_V_message_flag = false ;
            _receive_flag = false ;
            _just_sent_CI_V_message_flag = false ;
            _tx1_inhibit_flag = true ; 
            _tx_verified_flag = false ;
            _TX_MESSAGE_NOT_CORRUPT_flag = false ;
            _collision_flag = false ;
            _tx_jam_code_flag = false ; 
            _rx_jam_code_flag = false ;
            //holdoff
            _CI_V_TX_HOLDOFF_flag = false ;
            _CI_V_time2live  = 0;
            _day = 0 ;
            _month= 0  ;
            _yearHigh = 0 ;
            _yearLow = 0 ;
              _mins = 0 ;
            _hours = 0  ;

            }

              
                void Controller::set_hex_id(byte hex_id)
                                                      {
                                                      if(hex_id <= 240 && hex_id >0)  
                                                      // acceptable value  is from  1 to 240
                                                        {
                                                        _hex_id = hex_id ; 
                                                        }
                                                        
                                                        }
              byte Controller::get_hex_id()
                                          {
                                            return _hex_id;
                                          }


              void Controller::set_CONTROLLER_Operating_Mode(int8_t mode)      
                                                                        {
                                                                        _CONTROLLER_operating_mode = mode;
                                                                         //   - sets operationg MODE value  for THIS device
                                                                        }

              int8_t Controller::get_CONTROLLER_Operating_Mode()     
                                                              {
                                                                return _CONTROLLER_operating_mode ;         
                                                                //   - gets operationg MODE value  for this device... and returns it
                                                              } 

              void Controller::set_CONTROLLER_LOOP_interval(long duration)   // 
                                                                            {             
                                                                              _CONTROLLER_LOOP_interval=duration;         
                                                                              // allows adjustment of the LED flash rate
                                                                            }

              long Controller::get_CONTROLLER_LOOP_interval()                //   
                                                                {
                                                                  return _CONTROLLER_LOOP_interval ;           
                                                                }

              void Controller::set_local_IP_port_and_MAC_swap(bool mode)
                                                                  {
                                                                  _CONTROLLER_local_IP_port_mac_flag = mode ; 
                                                                  // sets local or remote, depending on physical state of pin 31 ( IP_toggle_pin  )
                                                                  }

              bool Controller::get_local_IP_port_and_MAC_swap() 
                                                        {
                                                        return _CONTROLLER_local_IP_port_mac_flag ;
                                                        }

            void Controller::set_listen_for_CI_V_message_flag(bool state)  // 
                                                  {
                                                    _listen_for_CI_V_message_flag = state ;
                                                  }

                      bool Controller::get_listen_for_CI_V_message_flag()
                                                  {
                                                    return _listen_for_CI_V_message_flag ;
                                                  }

            void Controller::set_just_sent_CI_V_message_flag(bool state)  // 
                                                          {
                                                            _just_sent_CI_V_message_flag = state ;
                                                          }

              bool Controller::get_just_sent_CI_V_message_flag()
                                                  {
                                                    return _just_sent_CI_V_message_flag ;
                                                  }
                                                        
            void Controller::set_CI_V_TX_inhibit_flag(bool state)  
                                                            {
                                                              _tx1_inhibit_flag = state ;
                                                            }

              bool Controller::get_CI_V_TX_inhibit_flag()
                                                    {
                                                      return _tx1_inhibit_flag ;
                                                    }  

            void Controller::set_tx_verified_flag(bool state)  
                                                              {
                                                                _tx_verified_flag = state ;
                                                              }
    
              bool Controller::get_tx_verified_flag()
                                                    {
                                                      return _tx_verified_flag ;
                                                    } 

              void Controller::set_TX_MESSAGE_NOT_CORRUPT_flag(bool state)  
                                                                      {
                                                                        _TX_MESSAGE_NOT_CORRUPT_flag = state ;
                                                                      }
                                      
              bool Controller::get_TX_MESSAGE_NOT_CORRUPT_flag()
                                                              {
                                                                return _TX_MESSAGE_NOT_CORRUPT_flag ;
                                                              }  

            void Controller::set_collision_flag(bool state)  
                                                            {
                                                              _collision_flag = state ;
                                                            }

              bool Controller::get_collision_flag()
                                                  {
                                                    return _collision_flag ;
                                                  }  

            void Controller::set_tx_jam_code_flag(bool state)  
                                                              {
                                                                _tx_jam_code_flag = state ;
                                                              }

              bool Controller::get_tx_jam_code_flag()
                                                      {
                                                        return _tx_jam_code_flag ;
                                                      }

            void Controller::set_rx_jam_code_flag(bool state)  
                                                              {
                                                                _rx_jam_code_flag = state ;
                                                              }
        
              bool Controller::get_rx_jam_code_flag()
                                                    {
                                                      return _rx_jam_code_flag ;
                                                    }

              void Controller::set_ntp_valid_flag(bool state)  
                                                              {
                                                                _ntp_valid_flag = state ;
                                                              }
        
              bool Controller::get_ntp_valid_flag()
                                                    {
                                                      return _ntp_valid_flag ;
                                                    }


          bool Controller::get_CI_V_TX_HOLDOFF_flag()  // .. decrements the time 2 live counter and returns state
                          {
                            if (_CI_V_time2live > 0 )
                                                {
                                                  _CI_V_time2live-- ;          // each time 'read' decrement until zero
                                                  _CI_V_TX_HOLDOFF_flag = true ;
                                                }
                                                  //  if (_time2live <= 0 ) 
                            else
                                                  {
                                                  _CI_V_time2live = 0 ;
                                                  _CI_V_TX_HOLDOFF_flag = false ;
                                                  }
                            return _CI_V_TX_HOLDOFF_flag ;
                          }

            byte Controller::set_CI_V_TX_HOLDOFF_counter(byte count)  // sets the time 2 live counter
                            {
                              _CI_V_time2live = count ;   
                                // load time to live with ZERO to clear flag i.e. to block the inhib and allow Network listening
                                                      // or
                              // load time to live with the number of loops to inhibit
                                                  
                                if (_CI_V_time2live > 0)    // allow seting this to zero to s
                                                    {
                                                      _CI_V_TX_HOLDOFF_flag = true ; 
                                                      // i.e. inhibit ( dont allow Network Listen )
                                                    }
                                                          // if (_time2live <= 0 ) 
                                else
                                                      {
                                                      _CI_V_time2live =0 ;
                                                      _CI_V_TX_HOLDOFF_flag = false ;
                                                      }

                                                  

                                return _CI_V_time2live ;
                            }

                      byte Controller::read_CI_V_TX_HOLDOFF_counter()  // read the time 2 live value
                                                                          {
                                                                            return _CI_V_time2live ;

                                                                          }

   //  ****************************************** date & time **************************                                           

            void Controller::set_dayz(byte dayz)
                                            {
                                              _day  = dayz ;
                                            }
                               
            byte Controller::get_dayz()
                                    {
                                      return  _day ;
                                    }

            void Controller::set_monthz(byte monthz)
                                                {
                                                  _month = monthz;
                                                }
                                                
            byte Controller::get_monthz() 
                                    {
                                     return _month ;
                                    }

            void Controller::set_yearLowz(byte yearLowz) 
                                                    {
                                                     _yearLow = yearLowz ;
                                                    }
                                      
            byte Controller::get_yearLowz()
                                      {
                                        return _yearLow ;  
                                      }
                                      
            void Controller::set_yearHighz(byte yearHighz) 
                                                      {
                                                       _yearHigh  = yearHighz ;
                                                      }
            
            byte Controller::get_yearHighz() 
                                        {
                                         return _yearHigh   ; 
                                        }

            void Controller::set_hourz(byte hourz) 
                                              {
                                                _hours =hourz ;
                                              }
                                                                               
            byte Controller::get_hourz() 
                                    {
                                    return _hours ;
                                    }
                                    
            void Controller::set_minz(byte minz) 
                                            {
                                             _mins = minz ;
                                            }
                               
            byte Controller::get_minz()
                                  {
                                  return _mins ; 
                                  }

                                                                                              
// *******************************************************************************
// *******************      end of controller object    **************************
// *******************************************************************************
//
//
//    RRRRRRRR             A        DDDDDDD      IIIII       OOOOOO
//    RR     RR         AA   AA     DD     D       I        O      O
//    RR      RR       AA     AA    DD      D      I        O      O
//    RRRRRRRR         AA     AA    DD      D      I        O      O
//    RR   RR          AAAAAAAAA    DD      D      I        O      O
//    RR    RR         AA     AA    DD     D       I        O      O
//    RR     RR        AA     AA    DDDDDD       IIIII       OOOOOO
//
//
// *******************************************************************************
//                                radio object  
// *******************************************************************************
class Radio
             {
              public : // this is the constructor
                         
                          Radio(byte hex_id);
                          // these are the public class members..initiates member variables ( aka properties ) and allocates storage.. code is below
                          // object methods  == object internal functions ??
                         
                          void begin();
                          byte get_hex_id();
                          byte set_hex_id(byte hex_id) ;
                          void set_RADIO_connected (bool radio) ;                          
                          bool get_RADIO_connected () ;
/*
// new tweakables start
                          byte NR_get_off_on () ;// 00 or 01
                          void NR_set_off_on (byte on_off) ; 
                          byte NR_get_level () ; // 0000 - 0255
                          void NR_set_level (byte level) ; // 0000 - 0255

                          byte NB_get_off_on () ;// 00 or 01
                          void NB_set_off_on (byte on_off) ; 
                          byte NB_get_depth () ; // 00 - 09
                          void NB_set_depth (byte depth) ; // 00 - 09
                          byte NB_get_width () ; // 0000 - 0255
                          void NB_set_width (byte width) ; // 0000 - 0255
                          byte NB_get_level () ; // 0000 - 0255
                          void NB_set_level (byte level) ; // 0000 - 0255


                          byte Notch_get_auto_off_on () ;// 00 or 01
                          void Notch_set_auto_off_on (byte on_off) ; 
                          byte Notch_get_manual_off_on () ;// 00 or 01
                          void Notch_set_manual_off_on (byte on_off) ;                          
                          byte Notch_get_width () ; // 0000 - 0255
                          void Notch_set_width (byte width) ; // 0000 - 0255
                          byte Notch_get_position () ; // 0000 - 0255
                          void Notch_set_position (byte position) ; // 0000 - 0255
// new tweakables end
*/
                          byte get_current_band() ; // 
                          void set_current_band(byte current_band) ; 
                          byte get_current_rx_mode() ;   // maybe change this to a TOKEN i.e. a char rather than byte ... and remove lookup tables                       
                          void set_current_rx_mode(byte current_receive_mode) ; 
                          unsigned long get_current_frequency() ;
                          boolean set_current_frequency(unsigned long current_frequency) ;
                          int get_current_TS() ;
                          bool  set_current_TS(int tuning_step) ; 
           // s-meter
                          byte get_current_s_meter() ;
                          void set_current_s_meter(byte s_meter_current) ; // current (NOW)

                                    // date / time
                          void set_dayz(byte dayz)  ;
                          byte get_dayz()  ;
                          void set_monthz(byte monthz)  ;
                          byte get_monthz()  ;
                          void set_yearLowz(byte yearLowz)  ;
                          byte get_yearLowz()  ;   
                          void set_yearHighz(byte yearHighz)  ;
                          byte get_yearHighz()  ; 
                          void set_hourz(byte hourz)  ;
                          byte get_hourz()  ;
                          void set_minz(byte minz)  ;
                          byte get_minz()  ;
// CI-V message status
                          void set_FE1_flag (bool flag) ;                          
                          bool get_FE1_flag () ;
                          void set_FE2_flag (bool flag) ;                          
                          bool get_FE2_flag () ;
                          void set_to_flag (bool flag) ;                          
                          bool get_to_flag () ;
                          void set_from_flag (bool flag) ;                          
                          bool get_from_flag () ;    
                          void set_RX_Message_validity (byte flag) ;                          
                          byte get_RX_Message_validity () ;
                          void set_FA_flag (bool flag) ;                          
                          bool get_FA_flag () ;                      
                          void set_FB_flag (bool flag) ;                          
                          bool get_FB_flag () ; 
                          void set_FD_flag (bool flag) ;                          
                          bool get_FD_flag () ;            
                    
              private : //  these are the private members,... variables ( aka properties ) .. there could also be private methods
                        // object properties  ==  variables ??  

                           byte _hex_id ;

                          unsigned long _current_frequency ; // might use a union for the frequency ?
                          unsigned long _min_frequency ; // this is radio specific ... should I look this up ?
                          unsigned long _max_frequency ; // ditto
                          bool _freq_request_is_out_of_bounds_flag ; 
                          // i.e. shall set this if last frequency change request went beyound high or low limit ... roll around ?
                          int _current_TS ; // tuning step i.e. size (KHz) of incr of decr step
                          int _min_TS ; // tuning step i.e. size (KHz) of incr of decr step
                          int _max_TS ; // tuning step i.e. size (KHz) of incr of decr step
                          

                          byte _day ;
                          byte _month ;
                          byte _yearHigh ;
                          byte _yearLow ;

                          byte _mins ;
                          byte _hours ;
                          
                 volatile byte _s_meter_current ;
/*
// start 
                          byte _NR_off_on ;// 00 or 01
                          
                          byte _NR__level  ; // 0000 - 0255

                          byte _NB_off_on  ;// 00 or 01

                          byte _NB_depth  ; // 00 - 09

                          byte _NB_width  ; // 0000 - 0255

                          byte _NB_level  ; // 0000 - 0255

                          byte _Notch_auto_off_on  ;// 00 or 01

                          byte _Notch_manual_off_on  ;// 00 or 01
                         
                          byte _Notch_width  ; // 0000 - 0255

                          byte _Notch_position  ; // 0000 - 0255
// end
*/

// no mapped value ?
                          byte _current_band ; // 
                          byte _current_receive_mode ; // 

                          bool _FE1_flag ;
                          bool _FE2_flag ;
                          bool _FD_flag ;
                          bool _FB_flag ;
                          bool _FA_flag ;
                          bool _to_flag ;
                          bool _from_flag ;
                          bool _RX_Message_validity ;
                          bool _RADIO_connected ;

                };

             
Radio::Radio(byte hex_id) 
                        {
                         _hex_id = hex_id ;
                        }

                       void Radio::begin() // initialises Radio
                                          {

                                            _current_frequency = 1; //    1 MHz
                                            _current_band = 0 ; 
                                            // i.e. the out of bound 'bucket'  max value is 10
                                            _current_receive_mode = 99 ; 
                                            //  modes are 0:LSB, 1:USB, 2:AM, 3:CW, 4:(different 7300 / 8600), 5:FM                                            
                                            _min_frequency =      100000; //  100 KHz
                                            _max_frequency =    3000000000; //   70 MHz for 7300
                                            _current_TS = 1000;           //   tuning step 1 KHz
                                            _min_TS = 100 ;
                                            _max_TS = 50000 ;
                                       // _s_meter_hundreds_value = 0 ;
                                       //_s_meter_tens_value=0 ;
                                       //_s_meter_units_value=0 ;
                                            
                                            _s_meter_current = 0 ;


                                             _day= 0 ;
                                             _month= 0 ;
                                             _yearHigh= 0 ;
                                             _yearLow= 0 ;
                                             _mins= 0 ;
                                             _hours= 0 ;

                                            _FE1_flag = false ;
                                            _FE2_flag = false ;
                                            _FD_flag = false ;
                                            _FB_flag = false ;
                                            _FA_flag = false ;
                                            _to_flag = false ;
                                            _from_flag = false ;
                                            _RX_Message_validity = 0 ;
                                            _RADIO_connected = false;

                                          }

                                         
               byte Radio::set_hex_id(byte hex_id)
                                                  {
                                                   if(hex_id <= 240 && hex_id >0)  // acceptable value  is from  1 to 240
                                                                                {
                                                                                _hex_id = hex_id ; 
                                                                                }
                                                    return _hex_id ;
                                                  }
               byte Radio::get_hex_id()
                                        {
                                          return _hex_id;
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
                                                                       _s_meter_current = s_meter_current ; 
                                                                    }
       
               byte Radio::get_current_s_meter()  
                    {
                      return _s_meter_current;
                    }
             

               void Radio::set_current_band(byte current_band)
                                                              {
                                                              if (current_band < 11 )
                                                                                    {
                                                                                     _current_band = current_band ;   
                                                                                    }
                                                              
                                                                   
                                                              }

               byte Radio::get_current_band()
                                              {
                                                return _current_band;
                                              }


               void Radio::set_current_rx_mode(byte current_receive_mode)
                                              {
                                                _current_receive_mode = current_receive_mode ; 
                                                                                                                                  
                                              }
       
               byte Radio::get_current_rx_mode()
                                                        {
                                                          return _current_receive_mode ;  
                                                          // convert to ascii
                                                        }

              unsigned long Radio::get_current_frequency()
                                                  {
                                                   return _current_frequency;
                                                  }
                               
              bool Radio::set_current_frequency(unsigned long current_frequency)
                                                {
                                                _freq_request_is_out_of_bounds_flag = true  ; 
                                                // i.e. set flag to indicate a default of out of bounds !
                                                
                                                if (current_frequency < _max_frequency
                                                && current_frequency > _min_frequency)
                                                                                    {
                                                                                    _current_frequency = current_frequency ;
                                                                                    _freq_request_is_out_of_bounds_flag = false  ; 
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
                                        _freq_request_is_out_of_bounds_flag = true  ; // i.e. set flag to indicate a default of out of bounds !
                                        
                                        if (current_TS < _max_TS
                                        && current_TS > _min_TS)
                                                                {
                                                                _current_TS = current_TS ;
                                                                _freq_request_is_out_of_bounds_flag = false  ; // ie. reset flag
                                                                }
                                        return _freq_request_is_out_of_bounds_flag;
                                        
                                      }

            void Radio::set_dayz(byte dayz)
                                            {
                                              _day  = dayz ;
                                            }
                              
            byte Radio::get_dayz()
                                    {
                                      return  _day ;
                                    }

            void Radio::set_monthz(byte monthz)
                                                {
                                                  _month = monthz;
                                                }
                                                
            byte Radio::get_monthz() 
                                    {
                                     return _month ;
                                    }

            void Radio::set_yearLowz(byte yearLowz) 
                                                    {
                                                     _yearLow = yearLowz ;
                                                    }
                                     
            byte Radio::get_yearLowz()
                                      {
                                        return _yearLow ;  
                                      }

            void Radio::set_yearHighz(byte yearHighz) 
                                                      {
                                                       _yearHigh  = yearHighz ;
                                                      }
            
            byte Radio::get_yearHighz() 
                                        {
                                         return _yearHigh   ; 
                                        }

            void Radio::set_hourz(byte hourz) 
                                              {
                                                _hours =hourz ;
                                              }
                                 
            byte Radio::get_hourz() 
                                    {
                                    return _hours ;
                                    }

            void Radio::set_minz(byte minz) 
                                            {
                                             _mins = minz ;
                                            }
                              
            byte Radio::get_minz()
                                  {
                                  return _mins ; ;
                                  }

  // CI_V message status
                          void Radio::set_FE1_flag (bool flag) 
                                                              {
                                                              _FE1_flag = flag ; 
                                                              }                          
                          bool Radio::get_FE1_flag () 
                                                      {
                                                      return _FE1_flag  ; 
                                                      } 

                          void Radio::set_FE2_flag (bool flag) 
                                                              {
                                                              _FE2_flag = flag ; 
                                                              }                          
                          bool Radio::get_FE2_flag () 
                                                      {
                                                        return _FE2_flag ;
                                                      }
                          void Radio::set_to_flag (bool flag) 
                                                            {
                                                              _to_flag = flag ;
                                                            }                       
                          bool Radio::get_to_flag ()
                                                    {
                                                      return _to_flag ;
                                                    } 
                          void Radio::set_from_flag (bool flag) 
                                                              {
                                                               _from_flag = flag ;
                                                              }                         
                          bool Radio::get_from_flag () 
                                                      {
                                                        return _from_flag ;
                                                      }
                          void Radio::set_RX_Message_validity (byte flag)
                                                                     {
                                                                     _RX_Message_validity = flag ;  
                                                                     } 
                                                   
                          byte Radio::get_RX_Message_validity ()
                                                            {
                                                            return _RX_Message_validity ; 
                                                            } 
                          void Radio::set_FA_flag (bool flag)
                                                            {
                                                            _FA_flag = flag ;  
                                                            }                          
                          bool Radio::get_FA_flag ()
                                                    {
                                                      return _FA_flag ;
                                                    } 
                          void Radio::set_FB_flag (bool flag)
                                                            {
                                                            _FB_flag = flag ;  
                                                            } 
                                                    
                          bool Radio::get_FB_flag () 
                                                    {
                                                      return _FB_flag ;
                                                    }
                          void Radio::set_FD_flag (bool flag) 
                                                            {
                                                            _FD_flag = flag ;  
                                                            }
                                                    
                          bool Radio::get_FD_flag () 
                                                    {
                                                      return _FD_flag ;
                                                    }

                          bool Radio::get_RADIO_connected() 
                                                          {
                                                            return _RADIO_connected ;
                                                          }

                          void Radio::set_RADIO_connected(bool radio) 
                                                                    {
                                                                      _RADIO_connected = radio ;
                                                                    } 
// from KBD object

// from KBD object                                                              
                               
// end of constructor
// *******************************************************************************
// *******************      end of radio object     ******************************
// *******************************************************************************


// ******************************************************************************
//
//      MM         MM    EEEEEEEE   TTTTTTTTT    EEEEEEEE     RRRRRRRRR
//      MM  M    M MM    EE            TT        EE           RR       R
//      MM   M  M  MM    EE            TT        EE           RR       R
//      MM    M    MM    EEEEEE        TT        EEEEEE       RRRRRRRR
//      MM         MM    EE            TT        EE           RR  RR
//      MM         MM    EE            TT        EE           RR   RR
//      MM         MM    EE            TT        EE           RR    RR
//      MM         MM    EEEEEEEE      TT        EEEEEEE      RR     RR 
//
// *******************************************************************************
//                         LED Meter object  
// *******************************************************************************


 class LEDMeter   
             
              {// CLASS MEMBER VARIABLES - these are initialised at start up
                 
                public : 
                          LEDMeter(byte hex_id); 
                                
                                                    void begin();
                                                    byte get_hex_id();
                                                    byte set_hex_id(byte hex_id) ;
                          void set_current_s_meter(byte s_meter_current) ;
                          byte get_current_s_meter() ;
                                                    byte get_s_meter_mov_ave() ; 
                                                    void set_s_meter_mov_ave(byte s_meter_average) ; // average
                                                    void set_s_meter_low(byte s_meter_low)  ; // low
                                                    byte get_s_meter_low()  ; 
                                                    void set_s_meter_high(byte s_meter_high)  ; // high
                                                    byte get_s_meter_high()  ;
                           byte get_s_meter_peak_Index_LO()  ;  // for LED
                           byte get_s_meter_peak_Index_HI()  ;  // for LED

byte get_peak_TTL_count ();
                                                    byte get_s_meter_high_index() ;  // "    "
                           byte get_s_meter_ave_index()  ;  // "    "
                                                    byte get_s_meter_low_index()  ;  // "    "

                          byte calculate_current_MAPPED_value(byte S_METER_from_RADIO) ;
                private :  
                          byte _hex_id ; 
                          char _button_pressed_NOW_TOKEN ;
                         byte _s_meter_current ;
                         byte _s_meter_mov_ave ;//was volatile
                         byte _s_meter_low  ;//was volatile
                         byte _s_meter_high  ;//was volatile
                         byte _s_meter_peak  ; //was volatile
                                  
                          byte _low_index  ;
                          byte _high_index  ;
                          byte _ave_index ;
                          byte _peak_index = 0 ;
                          //byte _last_s_meter_peak_index ;
                          byte _peak_Index_LO  ;
                          byte _peak_Index_HI  ;
                          byte _peak_TTL_count ; // is this a Time to Live  counter ?                                
              };


LEDMeter::LEDMeter(byte hex_id) // this is a constructor.. 
                              {
                              _hex_id = hex_id;
                              }
                                                                                            
       void LEDMeter::begin() // initialises the device
                                {
                                _s_meter_mov_ave = 0 ;
                                _s_meter_low = 0 ;
                                _s_meter_high = 0 ;
                                _s_meter_peak = 0 ;
                               // _last_s_meter_peak_index =0 ;

                                _low_index = 0 ;
                                _high_index = 0 ;
                                _ave_index  = 0 ;
                                _peak_index = 0 ;

                               // _last_s_meter_peak_index =0 ;
                                _peak_Index_LO = 0  ;
                                _peak_Index_HI = 0  ;
                                _peak_TTL_count = 0 ; 

                                };  


               byte LEDMeter::set_hex_id(byte hex_id)
                                                        {
                                                        if(hex_id <= 240 && hex_id >0)  // acceptable value  is from  1 to 240
                                                        {
                                                        _hex_id = hex_id ; 
                                                        }
                                                          return _hex_id ;
                                                        }

               byte LEDMeter::get_hex_id()
                                            {
                                              return _hex_id;
                                            }
// redundant ??  START
               void LEDMeter::set_current_s_meter(byte s_meter_current)  
                                                                    {
                                                                       _s_meter_current = s_meter_current ; 
                                                                    }
       
               byte LEDMeter::get_current_s_meter()  
                    {
                      return _s_meter_current;
                    }
 


               void LEDMeter::set_s_meter_mov_ave(byte s_meter_average)  
                                                                    {
                                                                       _s_meter_mov_ave = s_meter_average ; 
                                                                    }
       
               byte LEDMeter::get_s_meter_mov_ave()  
                                                {
                                                  return  _s_meter_mov_ave;
                                                }
                                        
              void LEDMeter::set_s_meter_low(byte s_meter_low)  
                                                           {
                                                             _s_meter_low = s_meter_low ; 
                                                           }
       
             byte LEDMeter::get_s_meter_low()  
                                          {
                                            return _s_meter_low ;
                                          }
 
             void LEDMeter::set_s_meter_high(byte s_meter_high)  
                                                            {
                                                               _s_meter_high = s_meter_high ; 
                                                            }
       
             byte LEDMeter::get_s_meter_high()  
                                            {
                                              return _s_meter_high ;
                                            } 
// redundant  --- end ??

            byte LEDMeter::get_peak_TTL_count ()
                                            {
                                             return _peak_TTL_count ; ;
                                            }


   byte LEDMeter::calculate_current_MAPPED_value(byte S_METER_from_RADIO) // pass  s meter value ( from radio ) in here 
                      {
                       
                      _s_meter_current =  S_METER_from_RADIO ; // get S meter data

                      if ( _s_meter_current >= _s_meter_peak ) 
                                                            {
                                                              _s_meter_peak = _s_meter_current ; // PEAK  but must be zeroed after 600ms  PEAK PEAK PEAK
                                                            }

                                                            // TEMP

                                                            //_s_meter_peak = 170 ;


                        _s_meter_mov_ave =   (_s_meter_mov_ave + _s_meter_current+0 ) / 2 ;      // AVERAGE
                      // add old average to new value and divide by 2 


                      if ( _s_meter_current  <  _s_meter_low  ) 
                      // new is lower than old low 
                                                              { 
                                                              _s_meter_low = _s_meter_current ;   // LOW             
                                                              // make new the lowest
                                                              }

                      if ( _s_meter_current  >  _s_meter_high )                                                 
                      // new is higher than old high
                                                  { 
                                                  _s_meter_high = _s_meter_current;              // HIGH       
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
                                                                            _s_meter_high = 0 ;  
                                                                            // dont allow highest below zero
                                                                            }
                                                    if (_s_meter_high > 242) 
                                                    //  stop is rolling round to 255 or there abouts
                                                                              { 
                                                                              _s_meter_high =0 ; 
                                                                                // dont allow highest below zero
                                                                              }                         
                                                    }
                      
                      if ( _s_meter_current  >  _s_meter_low  )                                               
                       // if new greater than lowest
                                                      { 
                                                      _s_meter_low = ( _s_meter_low  +10 ) ;   
                                                      //   incresse lowest by + 10 ... keeps on lower LEDs ... stops flikering
                                                      
                                                      if (_s_meter_low > _s_meter_current) 
                                                                                          { 
                                                                                            _s_meter_high = _s_meter_current ; // WHY ?
                                                                                          }
                                                      }
                          
                        if ( _s_meter_low  >  _s_meter_high  )
                                                              { 
                                                                _s_meter_low  = _s_meter_high  ;
                                                              } 

                       if ( _s_meter_high  <  _s_meter_low  )
                                                              { 
                                                              _s_meter_high = _s_meter_low ; 
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
//                                                |
//                                                V

// put all this index creation inside the radio object  and rem it out here - start  of rem

// now indexes are created and stored for access by other calls to the object



 #ifdef LED_8_MONO
              if (_s_meter_low  <= 120)
                                            {
                                             _low_index = map ( _s_meter_low , 0, 120, 0, 5 ) ; // map  
                                            }
                  if (_s_meter_low  >= 121)
                                            {
                                             _low_index = map ( _s_meter_low , 121, 240, 0, 3 ) ; // map 
                                             _low_index = _low_index + 9 ;
                                            }
                  if (_s_meter_high  <= 120)
                                            {
                                             _high_index = map ( _s_meter_high , 0, 120, 0, 5 ) ; // map  
                                            }
                  if (_s_meter_high  >= 121)
                                            {
                                             _high_index = map ( _s_meter_high , 121, 240, 0, 3 ) ; // map 
                                             _high_index = _high_index + 9 ;
                                            }   
                  if (_s_meter_mov_ave  <= 120)
                                            {
                                            _ave_index = map ( _s_meter_mov_ave , 0, 120, 0, 5 ) ; // map  
                                            }
                  if (_s_meter_mov_ave  >= 121)
                                            {
                                            _ave_index = map ( _s_meter_mov_ave , 121, 240, 0, 3 ) ; // map 
                                            _ave_index =_ave_index + 9 ;
                                            }
 #endif 

 #ifdef LED_8_RGB
              if (_s_meter_low  <= 120)
                                            {
                                             _low_index = map ( _s_meter_low , 0, 120, 0, 5 ) ; // map  
                                            }

                  if (_s_meter_low  >= 121)
                                            {
                                             _low_index = map ( _s_meter_low , 121, 240, 0, 3 ) ; // map 
                                             _low_index = _low_index + 9 ;
                                            }
                  if (_s_meter_high  <= 120)
                                            {
                                             _high_index = map ( _s_meter_high , 0, 120, 0, 5 ) ; // map  
                                            }
                  if (_s_meter_high  >= 121)
                                            {
                                             _high_index = map ( _s_meter_high , 121, 240, 0, 3 ) ; // map 
                                             _high_index = _high_index + 9 ;
                                            }   
                  if (_s_meter_mov_ave  <= 120)
                                            {
                                            _ave_index = map ( _s_meter_mov_ave , 0, 120, 0, 5 ) ; // map  
                                            }
                  if (_meter_mov_ave  >= 121)
                                            {
                                            _ave_index = map ( _s_meter_mov_ave , 121, 240, 0, 3 ) ; // map 
                                            _ave_index =_ave_index + 9 ;
                                            }
 #endif 

 #ifdef LED_16_MONO
                  if (_s_meter_low  <= 120)
                                            {
                                             _low_index = map (  _s_meter_low , 0, 120, 0, 9 ) ; // map  
                                            }
                  if ( _s_meter_low  >= 121)
                                            {
                                             _low_index = map (  _s_meter_low , 121, 240, 0, 6 ) ; // map 
                                             _low_index = _low_index + 9 ;
                                            }
                  if ( _s_meter_high  <= 120)
                                            {
                                             _high_index = map (  _s_meter_high , 0, 120, 0, 9 ) ; // map  
                                            }
                  if ( _s_meter_high  >= 121)
                                            {
                                             _high_index = map (  _s_meter_high , 121, 240, 0, 6 ) ; // map 
                                             _high_index = _high_index + 9 ;
                                            }   
                  if ( _s_meter_mov_ave  <= 120)
                                            {
                                            _ave_index = map (  _s_meter_mov_ave , 0, 120, 0, 9 ) ; // map  
                                            }
                  if ( _s_meter_mov_ave  >= 121)
                                            {
                                            _ave_index = map (  _s_meter_mov_ave , 121, 240, 0, 6 ) ; // map 
                                            _ave_index =_ave_index + 9 ;
                                            }
 #endif 

 #ifdef LED_16_RGB // INDEX FOR LEDS

                  if ( _s_meter_low  <= 120)
                                            {
                                             _low_index = map (  _s_meter_low , 0, 120, 0, 9 ) ; // map  
                                            }
                  if ( _s_meter_low  >= 121)
                                            {
                                             _low_index = map (  _s_meter_low , 121, 241, 1, 6 ) ; // map 
                                             _low_index = _low_index + 9 ;
                                            }
                  if ( _s_meter_high  <= 120)
                                            {
                                             _high_index = map (  _s_meter_high , 0, 120, 0, 9 ) ; // map  
                                            }
                  if ( _s_meter_high  >= 121)
                                            {
                                             _high_index = map (  _s_meter_high , 121, 241, 1, 6 ) ; // map 
                                             _high_index = _high_index + 9 ;
                                            }   
                  if ( _s_meter_mov_ave  <= 120)
                                            {
                                            _ave_index = map (  _s_meter_mov_ave , 0, 120, 0, 9 ) ; // map  
                                            }
                  if ( _s_meter_mov_ave  >= 121)
                                            {
                                            _ave_index = map (  _s_meter_mov_ave , 121, 241, 1, 6 ) ; // map 
                                            _ave_index =_ave_index + 9 ;
                                            }

 
// move this to the radio object or create an S-meter object ?





                  if ( _s_meter_peak  <= 120)
                                            {
                                            _peak_index = map (  _s_meter_peak , 0, 120, 0, 9 ) ; // map  
                                            }
                  if ( _s_meter_peak  >= 121)
                                            {
                                            _peak_index = map ( _s_meter_peak, 121, 241, 1, 6 ) ; // map 
                                            _peak_index = _peak_index + 9 ;
                                            }

                       _peak_TTL_count ++ ;

                     
                      if ( _peak_TTL_count >= LED_PEAK_TTL) 
                                          {
                                            _peak_index = 0 ;
                                            _s_meter_peak = 0 ; // clear peak
                                            _peak_TTL_count =0 ; // clear count
                                            
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
 
//  *********************************************************************************
//
//     IIIIIIII   NN           NN     DDDDDDDD       EEEEEEEE     XXX          XX       
//        II      NN NN        NN     DD      DD     EE            XX         XX
//        II      NN  NN       NN     DD       DD    EE             XX       XX   
//        II      NN   NN      NN     DD       DD    EE              XX     XX
//        II      NN    NN     NN     DD       DD    EEEEEEE          XX   XX
//        II      NN     NN    NN     DD       DD    EE                X X X
//        II      NN      NN   NN     DD       DD    EE               XX  XX
//        II      NN       NN  NN     DD       DD    EE              XX    XX
//        II      NN        NN NN     DD      DD     EE             XX      XX
//     IIIIIIII   NN           NN     DDDDDDDD       EEEEEEEE     XX         XX
//
// *********************************************************************************


      switch (_peak_index) 
                       {
                      case 0 :
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0xFF;// high byte  was FF TEMP
                      break  ;

                      case 1 :
                      _peak_Index_LO =0x7F;// low byte
                      _peak_Index_HI =0xFF;// high byte
                      break ;

                      case 2 :       
                      _peak_Index_LO =0xBF;// low byte
                      _peak_Index_HI =0xFF;// high byte 
                      break  ; 

                      case 3 :       
                      _peak_Index_LO =0xDF;// low byte
                      _peak_Index_HI =0xFF;// high byte
                      break  ; 

                      case 4 :       
                      _peak_Index_LO =0xEF;// low byte
                      _peak_Index_HI =0xFF;// high byte
                      break  ; 

                      case 5 :       
                      _peak_Index_LO =0xF7;// low byte
                      _peak_Index_HI =0xFF;// high byte
                      break  ;

                      case 6 :       
                      _peak_Index_LO =0xFB;// low byte
                      _peak_Index_HI =0xFF;// high byte
                      break  ;

                      case 7 :        
                      _peak_Index_LO =0xFD;// low byte
                      _peak_Index_HI =0xFF;// high byte
                      break  ;

                      case 8 :
                      _peak_Index_LO =0xFE;// low byte
                      _peak_Index_HI =0xFF;// high byte
                      break  ;

                      case 9 :       
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0x7F;// high byte 
                      break  ; 

                      case 10 :       
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0xBF;// high byte
                      break  ; 

                      case 11 :       
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0xDF;// high byte
                      break  ; 

                      case 12 :       
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0xEF;// high byte
                      break  ;

                      case 13 :       
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0xF7;// high byte
                      break  ;

                      case 14 :       
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0xFB;// high byte
                      break  ; 

                      case 15 :       
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0xFD;

                      case 16 :  
                      _peak_Index_LO =0xFF;// low byte
                      _peak_Index_HI =0xFE;// high byte     
                      break  ;

                     // default : 
                     // _peak_Index_LO =0xFF;// low byte
                     // _peak_Index_HI =0xFF;// high byte     
                     // break  ;

                      }

        return  _s_meter_mov_ave;  // THIS FUNCTION RETURNS the moving average TO calling statement 
        
        // i.e. returns the moving average S meter value ?

                        

       }
                     //matrix.print(MaxIndex1, DEC);
                     //matrix.writeDisplay(); // adds 1.5ms of delay 
 #endif 
// put all this index creation inside the radio object  and rem it out here - end of rem 

/*
                          byte get_s_meter_peak_Index_LO()  ;  // for LED
                          byte get_s_meter_peak_Index_HI()  ;  // for LED
                          byte get_s_meter_high_index() ;  // "    "
                          byte get_s_meter_ave_index()  ;  // "    "
                          byte get_s_meter_low_index()  ;  // "    "

*/




              byte LEDMeter::get_s_meter_peak_Index_LO()  
                                  {
                                    return _peak_Index_LO;
                                  }    

              byte LEDMeter::get_s_meter_peak_Index_HI()  
                                  {
                                    return _peak_Index_HI;
                                  }



                                   // end of objet funcion -mapped values


              byte LEDMeter::get_s_meter_low_index()  
                                  {
                                    return _low_index;
                                  }

              byte LEDMeter::get_s_meter_high_index()  
                                  {
                                    return _high_index;
                                  }

              byte LEDMeter::get_s_meter_ave_index()  
                                  {
                                    return _ave_index;
                                  }




// *******************************************************************************
// *******************      end of LED Meter Object   ****************************
// *******************************************************************************

//     BBBBBBBBBBB    UU         UU       TTTTTTTTTT    TTTTTTTTTTT     OOOOOO      NNN           NN       BBBBBBBBBBB    RRRRRRRR    DDDDDDDDDD
//     BB        BB   UU         UU           TT             TT       O        O     NN N         NN       BB        BB   RR      RR  DD       DD
//     BB        BB   UU         UU           TT             TT      O          O    NN   N       NN       BB        BB   RR      RR  DD        DD
//     BBBBBBBBBB     UU         UU           TT             TT      O          O    NN     N     NN       BBBBBBBBBB     RR     RR   DD        DD
//     BB        BB   UU         UU           TT             TT      O          O    NN       N   NN       BB        BB   RR  RR      DD        DD
//     BB       BB     UU       UU            TT             TT       O        O     NN         N NN       BB       BB    RR   RR     DD       DD
//     BBBBBBBBB         UUUUUUU              TT             TT         OOOOOO       NN           NN       BBBBBBBBB      RR    RR    DDDDDDDDD


// *******************************************************************************
//                           LOCAL keyboard object - not necessary ? migrate soon
// *******************************************************************************




class ButtonBoard
                     
                      {  // CLASS MEMBER VARIABLES - these are initialised at start up
      
                        public : 
                                ButtonBoard(byte hex_id); 
                                
                                void begin();

                                byte get_hex_id();
                                byte set_hex_id(byte hex_id) ;
                                
                                char get_button_pressed_NOW_TOKEN();                   
                                void set_button_pressed_NOW_TOKEN(char button_TOKEN) ; 

                                bool get_any_button_pressed_toggle () ;
                                void set_any_button_pressed_toggle (bool toggle) ;

                                char get_last_button_pressed_TOKEN();                   
                                void set_last_button_pressed_TOKEN(char button_TOKEN) ;                                 

                                byte get_last_button_pressed_repeat_counter();                   // added
                                void set_last_button_pressed_repeat_counter(byte duration) ;    // added    

      
                                char get_requested_rx_mode_token() ;   
                                void set_requested_rx_mode_token(char requested_rx_mode_token) ; 
                              
                                char get_current_rx_mode_token() ;                         
                                void set_current_rx_mode_token(char current_rx_mode_token) ; 


                                char get_requested_rx_band_token() ;   
                                void set_requested_rx_band_token(char requested_rx_band_token) ; 
                              
                                char get_current_rx_band_token() ;                         
                                void set_current_rx_band_token(char current_rx_band_token) ; 
/*
// new tweakables start
                          byte NR_get_off_on () ;// 00 or 01
                          void NR_set_off_on (byte on_off) ; 
                          byte NR_get_level () ; // 0000 - 0255
                          void NR_set_level (byte level) ; // 0000 - 0255

                          byte NB_get_off_on () ;// 00 or 01
                          void NB_set_off_on (byte on_off) ; 
                          byte NB_get_depth () ; // 00 - 09
                          void NB_set_depth (byte depth) ; // 00 - 09
                          byte NB_get_width () ; // 0000 - 0255
                          void NB_set_width (byte width) ; // 0000 - 0255
                          byte NB_get_level () ; // 0000 - 0255
                          void NB_set_level (byte level) ; // 0000 - 0255


                          byte Notch_get_auto_off_on () ;// 00 or 01
                          void Notch_set_auto_off_on (byte on_off) ; 
                          byte Notch_get_manual_off_on () ;// 00 or 01
                          void Notch_set_manual_off_on (byte on_off) ;                          
                          byte Notch_get_width () ; // 0000 - 0255
                          void Notch_set_width (byte width) ; // 0000 - 0255
                          byte Notch_get_position () ; // 0000 - 0255
                          void Notch_set_position (byte position) ; // 0000 - 0255
// new tweakables end
*/


                        private :  
                                 byte _hex_id ; 
                                 char _button_pressed_NOW_TOKEN ; 
                                 bool _any_button_pressed_toggle ;
                                 char _last_button_pressed_TOKEN ; 
                                 byte _last_button_pressed_repeat_counter ;// new for button board 
                                 char _current_rx_mode_token ; 
                                 char _requested_rx_mode_token;
                                 char _current_rx_band_token ; 
                                 char _requested_rx_band_token;  
/*
// start 
                          byte _NR_off_on ;// 00 or 01
                          
                          byte _NR__level  ; // 0000 - 0255

                          byte _NB_off_on  ;// 00 or 01

                          byte _NB_depth  ; // 00 - 09

                          byte _NB_width  ; // 0000 - 0255

                          byte _NB_level  ; // 0000 - 0255

                          byte _Notch_auto_off_on  ;// 00 or 01

                          byte _Notch_manual_off_on  ;// 00 or 01
                         
                          byte _Notch_width  ; // 0000 - 0255

                          byte _Notch_position  ; // 0000 - 0255
// end
*/    
                      };       // end of object variable definition
                               // now define the methods

ButtonBoard::ButtonBoard(byte hex_id) // this is a constructor.. 
                                    {
                                      _hex_id = hex_id;// tick
                                    }
                                                                                            
       void ButtonBoard::begin() // initialises the device
                                {
                                  _button_pressed_NOW_TOKEN = '`' ;
                                 // _any_button_pressed_toggle = 0 ;
                                  _last_button_pressed_TOKEN = '`' ;                        
                                  _current_rx_mode_token = '`';    // -------------'`' don't care
                                  _requested_rx_mode_token = '`' ; //------------'b' or (as in button sketch) 'x' (where 'x' = don't care )
                                  _current_rx_band_token = '`';    // --------------'`' don't care
                                  _requested_rx_band_token = '`' ; //------------'b' or (as in button sketch) 'x' (where 'x' = don't care )
                                }



               byte ButtonBoard::set_hex_id(byte hex_id)
                                                        {
                                                        if(hex_id <= 240 && hex_id >0)  // acceptable value  is from  1 to 240
                                                        {
                                                        _hex_id = hex_id ; 
                                                        }
                                                          return _hex_id ;
                                                        }

               byte ButtonBoard::get_hex_id()
                                            {
                                              return _hex_id;
                                            }

                                        
               char ButtonBoard::get_button_pressed_NOW_TOKEN()
                                                            {
                                                            return _button_pressed_NOW_TOKEN ;  
                                                            }


                                                            
               void ButtonBoard::set_button_pressed_NOW_TOKEN(char button_TOKEN) // added from button board sketch
                                                                            {
                                                                            _button_pressed_NOW_TOKEN =  button_TOKEN ;  
                                                                            }


               bool ButtonBoard::get_any_button_pressed_toggle () 
                                                                {
                                                                  return _any_button_pressed_toggle ;
                                                                }


              void ButtonBoard::set_any_button_pressed_toggle (bool toggle) 
                                                                          {
                                                                            _any_button_pressed_toggle = toggle ;
                                                                          }


               char ButtonBoard::get_last_button_pressed_TOKEN()
                                                                {
                                                                return _last_button_pressed_TOKEN ;  
                                                                }

                                                            
               void ButtonBoard::set_last_button_pressed_TOKEN(char last_button_TOKEN) // added from button board sketch
                                                                                      {
                                                                                      _last_button_pressed_TOKEN =  last_button_TOKEN ;  
                                                                                      }

                        
               byte ButtonBoard::get_last_button_pressed_repeat_counter()
                                                                          {
                                                                          return _last_button_pressed_repeat_counter ;  
                                                                          }

                                            
               void ButtonBoard::set_last_button_pressed_repeat_counter(byte button_duration) // added from button board sketch
                                                                                              {
                                                                                              _last_button_pressed_repeat_counter =  button_duration ;  
                                                                                              }
                                                         

               void ButtonBoard::set_current_rx_mode_token(char current_rx_mode_token)  
                                                                                        {
                                                                                          _current_rx_mode_token = current_rx_mode_token ; 
                                                                                        }

               char ButtonBoard::get_current_rx_mode_token()  
                                                                {
                                                                  return _current_rx_mode_token;
                                                                }


               void ButtonBoard::set_requested_rx_mode_token(char requested_rx_mode_token) 
                                                                                          {
                                                                                            _requested_rx_mode_token = requested_rx_mode_token ; 
                                                                                          }
       
               char ButtonBoard::get_requested_rx_mode_token()  
                                                              {
                                                                return _requested_rx_mode_token;
                                                              }

               void ButtonBoard::set_current_rx_band_token(char current_rx_band_token)  
                                                                                      {
                                                                                        _current_rx_band_token = current_rx_band_token ; 
                                                                                      }

       
                 char ButtonBoard::get_current_rx_band_token()  
                                                              {
                                                                return _current_rx_band_token;
                                                              }
      
               void ButtonBoard::set_requested_rx_band_token(char requested_rx_band_token) 
                                                                                          {
                                                                                            _requested_rx_band_token = requested_rx_band_token ; 
                                                                                          }
       
               char ButtonBoard::get_requested_rx_band_token()  
                                                              {
                                                                return _requested_rx_band_token;
                                                              }
/*
 // new tweakables start
                          byte ButtonBoard::NR_get_off_on () // 00 or 01
                                                      {
                                                      return _NR_off_on ;// 00 or 01
                                                      }
                           
                          void ButtonBoard::NR_set_off_on (byte on_off)  
                                                                {
                                                                 _NR_off_on = on_off ;
                                                                }

                          byte ButtonBoard::NR_get_level ()  // 0000 - 0255
                                                    {
                                                      return _NR__level  ; // 0000 - 0255
                                                    }

                          void ButtonBoard::NR_set_level (byte on_off)  // 0000 - 0255
                                                                {
                                                                  _NR__level = on_off; // 0000 - 0255
                                                                }

                          byte ButtonBoard::NB_get_off_on () // 00 or 01
                                                      {
                                                        return _NB_off_on  ;// 00 or 01
                                                      }


                          void ButtonBoard::NB_set_off_on (byte on_off) 
                                                                {
                                                                  _NB_off_on = on_off ;// 00 or 01
                                                                } 


                          byte ButtonBoard::NB_get_depth ()  // 00 - 09
                                                    {
                                                    return _NB_depth  ; // 00 - 09
                                                    }
                          void ButtonBoard::NB_set_depth (byte depth)  // 00 - 09
                                                    {
                                                    _NB_depth  = depth ; // 00 - 09
                                                    }
                          byte ButtonBoard::NB_get_width ()  // 0000 - 0255
                                                    {
                                                    return _NB_width  ; // 0000 - 0255
                                                    }
                          void ButtonBoard::NB_set_width (byte width)  // 0000 - 0255
                                                    {
                                                      _NB_width = width ; // 0000 - 0255
                                                    }
                          byte ButtonBoard::NB_get_level ()  // 0000 - 0255
                                                    {
                                                    return _NB_level  ; // 0000 - 0255
                                                    }
                          void ButtonBoard::NB_set_level (byte level)  // 0000 - 0255
                                                    {
                                                      _NB_level = level ; // 0000 - 0255
                                                    }


                          byte ButtonBoard::Notch_get_auto_off_on () // 00 or 01
                                                              {
                                                              _Notch_auto_off_on  ;// 00 or 01
                                                              }
                          void ButtonBoard::Notch_set_auto_off_on (byte on_off)
                                                                        {
                                                                         _Notch_auto_off_on = on_off ;// 00 or 01
                                                                        }  
                          byte ButtonBoard::Notch_get_manual_off_on () 
                                                                {
                                                               return _Notch_manual_off_on  ;// 00 or 01 // 00 or 01
                                                                }
                          void ButtonBoard::Notch_set_manual_off_on (byte on_off) 
                                                                          {
                                                                            _Notch_manual_off_on = on_off ;// 00 or 01
                                                                          }                          
                          byte ButtonBoard::Notch_get_width () 
                                                        {
                                                        return _Notch_width  ; // 0000 - 0255
                                                        } // 0000 - 0255

                          void ButtonBoard::Notch_set_width (byte width) 
                                                                  {
                                                                    _Notch_width = width ; // 0000 - 0255
                                                                  } // 0000 - 0255

                          byte ButtonBoard::Notch_get_position () 
                                                          {
                                                          return _Notch_position  ; // 0000 - 0255
                                                          } // 0000 - 0255

                          void ButtonBoard::Notch_set_position (byte position) 
                                                          {
                                                           _Notch_position = position ; // 0000 - 0255
                                                          } // 0000 - 0255

 
                          
// new tweakables end                   
 */                                                                                                      

// *******************************************************************************
// *******************      end of ButtonBoard object   **************************
// *******************************************************************************





// led object - digigal i/o pins for leds below

// *******************************************************************************
// ***************************     LED pin  object     ***************************
// *******************************************************************************
class DigitalPin
                {
                  public : // 
                          DigitalPin(int pin);
                          void begin();
                          void on();
                          void off();
                          bool status();
                          
                
                  private : //
                  
                          int _pin;
                          bool _status;
                };

DigitalPin::DigitalPin(int pin) // this is a constructor
                              {
                                _pin = pin;// the pin number is stored
                              }
                              
       void DigitalPin::begin() // initialises the pin
                              {
                                pinMode(_pin,OUTPUT);
                                digitalWrite(_pin,LOW);
                              }
                              
                              
        void DigitalPin::on() // turns pin on
                              {
                                digitalWrite(_pin,HIGH);
                                _status=1;
                              }
                              
          void DigitalPin::off() // turns pin off
                              {
                                digitalWrite(_pin,LOW);
                                _status=0;
                              }
                              
       bool DigitalPin::status()
                              {
                                return _status;
                              }


// end of constructor

// *******************************************************************************
// ****************             end of pin object       **************************
// *******************************************************************************

// new object



 
// end of new object



// *******************************************************************************
// *******************   Illuminated Buton  object     ***************************
// *******************************************************************************


// from button sketch  eventually make this part of buttonboard definition
class ButtonWithLED
                {
                  public : // 
                          ButtonWithLED(int pin);
                          void begin();
                          void on();              //same as 'on' ?
                          void off();
                          bool LED_status();   //   ******************************************         // was bool ... now 0 = off  1= on   2 = flashing
                          void set_flash(bool) ;  // flash if true
                          bool get_flash() ;
                         
                          void twinkle(); // calls flashing routine 
                  private : //
                  
                          int _pin;
                          bool _LED_status; //     ******************************************         // was bool ... now 0 = off  1= on   2 = flashing
                          bool _flash ;
                          volatile unsigned long _currentMillis ;
                          volatile unsigned long _previousMillis ;
                          
                };

ButtonWithLED::ButtonWithLED(int pin) // this is a constructor
                              {
                                _pin = pin;// the pin number is stored
                              }
                              
       void ButtonWithLED::begin() // initialises the pin
                                  {
                                    pinMode(_pin,OUTPUT);
                                    _flash = false ; // flash if true
                                    _LED_status= false;
                                    digitalWrite(_pin,LOW); 
                                  }
                              
                              
        void ButtonWithLED::on() // turns pin on
                                {
                                  digitalWrite(_pin,HIGH);
                                  _LED_status=1;
                                }
                              
          void ButtonWithLED::off() // turns pin off
                                  {
                                    digitalWrite(_pin,LOW);
                                    _LED_status=0;
                                  }
                              
       bool ButtonWithLED::LED_status()
                                      {
                                        return _LED_status;  // high means on , low means off
                                      }

          void ButtonWithLED::twinkle() // twinkle if flash true, dont twinkle if flash false
                                                  {
                                                   if( _flash)  // if flash is set true...do flash
                                                             {
                                                              _currentMillis = millis() ;
                                                             
           
                                                              if (_currentMillis - _previousMillis  >= FLASHINTERVAL )     //     ..........timer has expired so now flip on / off state
                                                                                                                     
                                                               {  
                                                                _previousMillis = _currentMillis ;
                                                                
                                                                if (_LED_status==false && _flash==true) // status : false = off, true = on
                                                                                                      {  
                                                                                                       _LED_status=1; // if it was off AND Flashing ...turn it on
                                                                                                       digitalWrite(_pin,HIGH);
                                                                                                       }
                                                                                                        else  
                                                                                                            {
                                                                                                             _LED_status=0; //if it was on .AND Flashing..turn it off
                                                                                                             digitalWrite(_pin,LOW);
                                                                                                            }                                                     
                                                               }
          
                                                              
                                                             } 
                                                  }

                                                  
          void ButtonWithLED::set_flash(bool flash) // flash if true, dont flash if false
                                                  {  
                                                  
                                                   if (flash==true)
                                                                   {
                                                                    _flash = HIGH ; 
                                                                   } 

                                                    if (flash==false)
                                                                   {
                                                                    _flash = LOW ; 
                                                                    _LED_status=0; //if it was on .AND Flashing..turn it off
                                                                     digitalWrite(_pin,LOW);
                                                                    
                                                                   } 
                  
                                                  }

                                                  
          bool ButtonWithLED::get_flash() // is pin set to flash ?
                              {
                                return _flash ;
                              }

// *******************************************************************************
// ****************              end of constructor     **************************
// *******************************************************************************


// 
// *******************************************************************************
// ****************         instantiate objects        ***************************
// *******************************************************************************

 #ifdef PRO_MINI  
 // DigitalPin YellowLED(13);  // YellowLED is an Object - driving pin 36 
 #endif
 
//LED_S_meter My_meter(1) ;


ButtonWithLED LSB_Button_LED(LSB_LED_PIN); //  2 
ButtonWithLED AM_Button_LED(AM_LED_PIN);  //   3 
ButtonWithLED USB_Button_LED(USB_LED_PIN); //  4 
ButtonWithLED CW_Button_LED(CW_LED_PIN);  //   5 

 
 #ifdef MEGA2560
DigitalPin WhiteLED(WHITE);  // WhiteED is an Object - driving pin 29  
DigitalPin Yellow1LED(YELLOW1);  // YellowLED is an Object - driving pin 33 
DigitalPin Yellow2LED(YELLOW2);  // YellowLED is an Object - driving pin 37 
DigitalPin GreenLED(GREEN);  // GreenLED is an Object - driving pin 41 
DigitalPin BlueLED(BLUE);   // BlueLED is an Object - driving pin 45 
DigitalPin RedLED(RED);    // RedLED is an Object - driving pin 49 
 #endif


Controller Arduino(CONTROLLER_ID) ;  
Radio Icom(RADIO) ;       // generic radio  0x00

 //#define RADIO_8600  0x96   // these get assigned by the config 'toggle' pin (12 or 24) being HI (7300) or LO (8600)                              //               
 //#define RADIO_7300  0x94   // ditto

LEDMeter FrontLEDmeter(1) ; 


ButtonBoard LOCAL_KBD(1) ;

 
RingBuf<uint8_t, 79> ci_v_ring_Buffer;   

#ifdef PRO_MINI
SoftwareSerial Soft_Serial(CI_V_RX_pin,CI_V_TX_pin);
#endif

 Adafruit_MCP4725 meter_dac;

// *******************************************************************************
// ****************    end of declarations    ************************************
// *******************************************************************************
//      SSSSSS      EEEEEEEEEE    TTTTTTTTTT      UU       UU       PPPPPPPP
//    SS      SS    EE                TT          UU       UU       PP      PP
//     SS           EE                TT          UU       UU       PP       PP
//       SS         EE                TT          UU       UU       PP      PP
//         SS       EEEEEEEE          TT          UU       UU       PPPPPPPP
//           SS     EE                TT          UU       UU       PP
//    SS      SS    EE                TT           UU     UU        PP
//      SSSSSS      EEEEEEEEEE        TT             UUUUU          PP
// *******************************************************************************
// ****************    SETUP   ***************************************************
// *******************************************************************************


void moving_coil (byte index) 
                    {

                    uint32_t counter = index * 255 ;
                    // Run through the full 12-bit scale for a triangle wave
                                   
                     meter_dac.setVoltage(counter, false);              
                                   
                      /*             
                                    for (counter = 0; counter < 4095; counter++)
                                                                                {
                                                                                  meter_dac.setVoltage(counter, false);
                                                                                }
                                    delay(1) ; 

                                    //delay(100) ;                                          
                                    for (counter = 4095; counter > 0; counter--)
                                                                                {
                                                                                  meter_dac.setVoltage(counter, false);
                                                                                }
                                    delay(1) ; 
                                    */
                    }
          



void setup()
              {
 pinMode( IP_toggle_pin , INPUT_PULLUP) ; //  check for High or low 
              // ...after initialisation this sets device IP and MAC adress to be local or remote  pinMode( IP_toggle_pin , INPUT_PULLUP) ; //  check for High or low 
              // ...after initialisation this sets device IP and MAC adress to be local or remote 
 #ifdef MEGA2560
               GreenLED.begin();
               BlueLED.begin();
               RedLED.begin();
               Yellow1LED.begin();
               Yellow2LED.begin();
               WhiteLED.begin();
               
              pinMode( IP_toggle_pin , INPUT_PULLUP) ; //  check for High or low 
              // ...after initialisation this sets device IP and MAC adress to be local or remote 
               
          #endif  




// **********************
// I2C devices
// *********************
    matrix.begin(0x70);
 meter_dac.begin(0x62); 


// ********************
//  other objects  
// ********************

 Icom.begin();
// Icom_7300.begin();

 FrontLEDmeter.begin() ;

 Arduino.begin() ;
 LOCAL_KBD.begin() ; 
 
 LSB_Button_LED.begin();
 AM_Button_LED.begin();
 USB_Button_LED.begin();
 CW_Button_LED.begin();

  moving_coil (4) ;

  delay(2000);

  moving_coil (8) ;

  delay(2000);




 Arduino.set_local_IP_port_and_MAC_swap ( digitalRead(IP_toggle_pin) );   // read the input pin

 if ( Arduino.get_local_IP_port_and_MAC_swap () == false) 
                                                          { 
                                                          Icom.set_hex_id (0x96); 
                                                          #ifdef MEGA2560
                                                                BlueLED.on() ; 
                                                                delay (500) ;                       
                                                                for (int Q = 0 ; Q <= 10 ; Q++)
                                                                    {
                                                                      GreenLED.on() ;
                                                                      delay (100) ;
                                                                      GreenLED.off() ;
                                                                      delay (100) ;
                                                                    }
                                                                delay (500) ;
                                                                BlueLED.off() ;
                                                          #endif
                                                          
                                                          }
                                                          else 
                                                          { 
                                                          Icom.set_hex_id (0x94); 
                                                          #ifdef MEGA2560
                                                                    BlueLED.on() ; 
                                                                    delay (500) ;                       
                                                                    for (int Q = 0 ; Q <= 10 ; Q++)
                                                                        {
                                                                        RedLED.on() ;
                                                                        delay (100) ;
                                                                        RedLED.off() ;
                                                                        delay (100) ;
                                                                        }
                                                                  delay (500) ;        
                                                                  BlueLED.off() ;
                                                          #endif
                                                          }

 if ( Icom.get_hex_id() == 0x96)  // modify CI-V messages for date & time dependant of radio model
                                    {
                                                    Icom.set_hex_id(RADIO_8600) ; // pin 24 to GND
                                                    ci_v_TX1_RX1_MESSAGEs[21][4] = 0x01 ; // get date
                                                    ci_v_TX1_RX1_MESSAGEs[21][5] = 0x31 ; // get date
                                                    ci_v_TX1_RX1_MESSAGEs[22][4] = 0x01 ; // get time
                                                    ci_v_TX1_RX1_MESSAGEs[22][5] = 0x32 ; // get time

                                                    ci_v_TX1_RX1_MESSAGEs[23][4] = 0x01 ; // set date
                                                    ci_v_TX1_RX1_MESSAGEs[23][5] = 0x31 ; // set date
                                                    ci_v_TX1_RX1_MESSAGEs[24][4] = 0x01 ; // set time
                                                    ci_v_TX1_RX1_MESSAGEs[24][5] = 0x32 ; // set time
                                    }

if ( Icom.get_hex_id() == 0x94)
                                    {                                    
                                                    Icom.set_hex_id(RADIO_7300) ; // pin 24 open ( pulled to 5v)
                                                    ci_v_TX1_RX1_MESSAGEs[21][4] = 0x00 ; // get date
                                                    ci_v_TX1_RX1_MESSAGEs[21][5] = 0x94 ; // get date
                                                    ci_v_TX1_RX1_MESSAGEs[22][4] = 0x00 ; // get time
                                                    ci_v_TX1_RX1_MESSAGEs[22][5] = 0x95 ; // get time

                                                    ci_v_TX1_RX1_MESSAGEs[23][4] = 0x00 ; // set date
                                                    ci_v_TX1_RX1_MESSAGEs[23][5] = 0x94 ; // set date
                                                    ci_v_TX1_RX1_MESSAGEs[24][4] = 0x00 ; // set time
                                                    ci_v_TX1_RX1_MESSAGEs[24][5] = 0x95 ; // set time
                                    }





#ifdef MEGA2560
               Serial.begin(57600);    //Start the serial connection with the laptop
               Serial1.begin(19200);    //Start the serial1 connection with the radio to tx and rx data via CI-V interface            
               Serial3.begin(9600);    //Start the serial3 connection with the controller / REMOTE_KBD
 #endif



#ifdef PRO_MINI
              pinMode(CI_V_RX_pin, INPUT);
              pinMode(CI_V_TX_pin, OUTPUT);
              Serial.begin(19200) ;
              Soft_Serial.begin(57600);    //print out to other board
              Soft_Serial.listen() ;
#endif


     
//set pins to output because they are addressed in the main loop
pinMode(PWM_PIN, OUTPUT);
pinMode(STORE_PIN, OUTPUT);
pinMode(CLOCK_PIN, OUTPUT);
pinMode(DATA_PIN, OUTPUT);

analogWrite (PWM_PIN, 220);
digitalWrite(STORE_PIN, HIGH);



              
//  *******************************************************************************
//                                  show rev     
//  ******************************************************************************* 

 #ifdef PRO_MINI 
                Serial.println(rev);   // this code revision info 
                Soft_Serial.println(rev) ; 
                Soft_Serial.println(Icom.get_hex_id(),HEX); 
 #endif 

 
#ifdef MEGA2560
               Serial.println(rev);   // this code revision info 
               Serial.println(Icom.get_hex_id(),HEX);                            
#endif
                           
                            
LSB_Button_LED.on();delay (140) ;LSB_Button_LED.off();
AM_Button_LED.on();delay (140) ;AM_Button_LED.off();
USB_Button_LED.on();delay (140) ; USB_Button_LED.off(); 
CW_Button_LED.on(); delay (140) ;CW_Button_LED.off();
 
 #ifdef MEGA2560
                WhiteLED.on();delay(140) ; WhiteLED.off();delay(10) ; 
                Yellow1LED.on();delay(140) ;Yellow1LED.off(); delay(10) ;
                Yellow2LED.on(); delay(140) ; Yellow2LED.off(); delay(10) ; 
                GreenLED.on();  delay(140) ;  GreenLED.off();delay(10) ;
                BlueLED.on();delay(140) ;  BlueLED.off();delay(10) ; 
                RedLED.on(); delay(140) ; RedLED.off();delay(10) ; 
 #endif


// ***********************************************
//    popoulating the TX array from Band_store
// ***********************************************

for (int r = 1; r < 11; r++) 
                            {

                            for (int c = 0; c < 5; c++) 
                                                        {
                                                         ci_v_TX1_RX1_MESSAGEs[r+10][c+3]  = band_store[r][c] ;
                                                        }
                                                        
                            }

Arduino.set_listen_for_CI_V_message_flag(false) ;            // init sets to false ... 
Arduino.set_just_sent_CI_V_message_flag(false) ;           // init sets to false ...
Arduino.set_CI_V_TX_inhibit_flag(true) ;                 // init sets to true ...
Arduino.set_TX_MESSAGE_NOT_CORRUPT_flag(false) ;  // init sets to false ...
Arduino.set_collision_flag(false) ;                  // init sets to false ...
Arduino.set_tx_jam_code_flag(false) ;              // init sets to false ...
Arduino.set_rx_jam_code_flag(false) ;            // init sets to false ...



// *****************************************************************************************************
// **************  get date and time from the radio ****************************************************
// *****************************************************************************************************

int flush_count ;

//..........................................

 #ifdef PRO_MINI                  
                while (Serial.available() > 0) // while bytes from CI-V intrefaceare waiting to be read do it  
                      { 
                      Serial.read();  // get first or next byte 
                      flush_count++ ; // count bytes
                      if ( flush_count >= 1046 ) { break ; } // bug out  1046
                      }

                Soft_Serial.print ( "flush count : ") ;
                Soft_Serial.println (flush_count);

//..........................................
                
                Soft_Serial.print ( "flush 1 : ") ;
                Soft_Serial.print (RX1_COUNTER = Serial.available());
                
                Serial.read(); // empty RX hardware buffer now      
                delay(100) ;
                Soft_Serial.print ( "flush 2 : ") ;
                Soft_Serial.print (RX1_COUNTER = Serial.available());
                 
                /*
                RX1_COUNTER = Serial.available();
                Serial.read(); // empty RX hardware buffer now
                delay(100) ; 
                */  
#endif 

#ifdef MEGA2560
                RX1_COUNTER = Serial1.available(); 
                Serial1.read();         
                delay(100) ;
                /*
                RX1_COUNTER = Serial1.available();
                Serial1.read(); 
                delay(100) ; 
                */  
#endif 
                         
// get date & time

                memset(ci_v_rx_DATA, 0, 15);    // clear the array  
                memset(RX1_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);// clear the array  
                memset(TX1_MESSAGE_BUFFER, 0, 20);// clear the array 

    #ifdef MEGA2560              
                  GreenLED.on() ;
                  RedLED.on() ;
                  WhiteLED.on() ;
                  delay (200) ;
   #endif

                  Arduino.set_CI_V_TX_inhibit_flag(false) ; // ..... TRANSMIT INHIBIT DISABLED 
                  delay (100) ;

                  tx_ci_v_via_serial_ttl(21) ; // send CI-V message to request date
                  delay (100) ;     
                  Arduino.set_CI_V_TX_inhibit_flag(true) ; // ..... TRANSMIT INHIBIT ENABLED
                  delay (100) ;
                  bit_bash(21) ;  // sort out the nibbles 
                  delay (100) ;

                  Icom.set_yearLowz (  ci_v_rx_DATA[2]<<4 | ci_v_rx_DATA[3])  ;  // [2] = tens   [3] = units... use <<4 to shift byte to high nibble
                  Icom.set_yearHighz(  ci_v_rx_DATA[0]<<4 | ci_v_rx_DATA[1] ) ;  // [0] = thou   [1] = hunds... use <<4 to shift byte to high nibble

                  Icom.set_monthz( ci_v_rx_DATA[4]<<4 | ci_v_rx_DATA[5])  ;

                  Icom.set_dayz(ci_v_rx_DATA[6]<<4 | ci_v_rx_DATA[7])  ;
                  #ifdef PRO_MINI
                  Soft_Serial.println();
                  Soft_Serial.print(" THESE ARE DAYZ UNITS ") ;
                  Soft_Serial.println(ci_v_rx_DATA[7],HEX);
                  Soft_Serial.println();
                  Soft_Serial.print(" THESE ARE DAYZ TENS ") ;
                  Soft_Serial.println(ci_v_rx_DATA[6],HEX);
                  Soft_Serial.println();
                  #endif

        matrix.print(Icom.get_dayz(), DEC);
        matrix.writeDisplay(); // adds 1.5ms of delay 

        delay(200) ;

        matrix.print(Icom.get_monthz(), DEC);
        matrix.writeDisplay(); // adds 1.5ms of delay 

        delay(200) ;

        matrix.print(Icom.get_yearLowz(), DEC);
        matrix.writeDisplay(); // adds 1.5ms of delay 

        delay(200) ;




#ifdef MEGA2560
       Serial.println();
       Serial.print(" THESE ARE DAYZ UNITS ") ;
       Serial.println (  ci_v_rx_DATA[7] , HEX );           
#endif

//   **********************************************************************

//    check is radio connected ?   year or month or day shouuld no be zero

//   **********************************************************************

if (   Icom.get_dayz() == 0 )  // also use .get_dayz to toggle pollling

                                {
                                 #ifdef MEGA2560
                                 Serial.print ("dayz = " );
                                 Serial.print (Icom.get_dayz(),DEC );
                                 Serial.println ("    no radio connected      ");
                                 #endif

                                  #ifdef PRO_MINI
                                  Soft_Serial.print ("dayz = " );
                                  Soft_Serial.print (Icom.get_dayz(),DEC);
                                  Soft_Serial.print ( "   no radio connected    ") ;
                                  #endif

                                  Icom.set_RADIO_connected(false)  ;
                                }
                                
                                else
                                {
                                  #ifdef MEGA2560
                                  Serial.print ("dayz = " );
                                  Serial.print (Icom.get_dayz(), HEX );
                                  Serial.println ("    radio connected      ");
                                  #endif

                                  #ifdef PRO_MINI
                                  Soft_Serial.print ("dayz = " );
                                  Soft_Serial.print (Icom.get_dayz(),HEX);
                                  Soft_Serial.print ( "    radio connected    ") ;
                                  #endif
                                  Icom.set_RADIO_connected(true)  ;

                                }


memset(ci_v_rx_DATA, 0, 15);    // clear the array  
memset(RX1_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);// clear the array  
memset(TX1_MESSAGE_BUFFER, 0, 20);// clear the array 

delay (200) ;
                                                                                      
                //  ***********************************************************
                //  *****************   get     time   from radio   ***********
                //  ***********************************************************

         


                memset(ci_v_rx_DATA, 0, 15);    // clear the array  
                memset(RX1_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);// clear the array  
                memset(TX1_MESSAGE_BUFFER, 0, 20);// clear the array 

                  
                   Arduino.set_CI_V_TX_inhibit_flag(false) ; // ..... TRANSMIT INHIBIT DISABLED 
          delay (100) ;

                   Arduino.set_CI_V_TX_inhibit_flag(true) ; // ..... TRANSMIT INHIBIT ENABLED
                   bit_bash(22) ;  // sort out the nibbles 
          delay (100) ;

                                                    
                          Icom.set_hourz(ci_v_rx_DATA[0]<<4 | ci_v_rx_DATA[1])  ;


                          Icom.set_minz(ci_v_rx_DATA[2]<<4 | ci_v_rx_DATA[3])  ;

                  memset(ci_v_rx_DATA, 0, 15);  // clear the array

// save time and date somewhere now
#ifdef MEGA2560
                  GreenLED.off() ;
                  RedLED.off() ;
                  WhiteLED.off() ;
 #endif   // mege2560
                                       
delay (100) ;

memset(RX1_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);// clear the array
memset(TX1_MESSAGE_BUFFER, 0, 20);// clear the array


} // end of set up


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++          end of setup function            ++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

///   memset(packetBuffer, 0, NTP_PACKET_SIZE);  /// keep safe here

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++         turn off led function         ++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// end of 0_setup tab

// start of 1_loop tab



            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
            //                      LL                        MAIN                  MAIN            PP P PP P PP P P   
            //                      LL                     OO       00           00      00         PP              PP
            //                      LL                    OO         00         OO        00        PP               PP
            //                      LL                   OO           OO       OO          OO       PP               PP
            //                      LL                   OO           OO       00          OO       PP              PP
            //                      LL                   OO           OO       OO          OO       PP             PP
            //                      LL                   OO           OO       OO          OO       PP P PP P PP P 
            //                      LL                   OO           00       OO          OO       PP
            //                      LL                   OO           OO       OO          OO       PP     
            //                      LL                   OO           OO       OO          OO       PP
            //                      LL                    OO         OO         OO       OO         PP
            //                      LLLLLLLLLLLLLLLL          LOOP                  LOOP            PP

            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



void loop() {// LOOP OPEN BRACKET 
            
unsigned char tm_buttons ;
byte button_scan_index = 0 ;
boolean valid_data_flag = false ;
unsigned long vco_frequency = 0 ;
int mean_average ;
#ifdef MEGA2560
                  WhiteLED.on() ;
 #endif
                  delay(1) ;
#ifdef MEGA2560
                  WhiteLED.off() ;
#endif      
           // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
           // ++++++++++++++++++++++++++++++++++++++++++     LOOP DELAY    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
           // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 

startMillis=millis() ;            

                      delay(LOOP_DELAY)  ; 
                      
                      
                       // 1ms put this back in when matrix.print is remmed or deleted

                                //matrix.print(button_scan_index, DEC);
                                             // LOCAL_KBD.get_button_pressed_TOKEN() 
                                           // matrix.print(Icom.get_current_s_meter(), DEC) ;  
                                           //matrix.print(Icom.get_s_meter_mov_ave(), DEC) ;      
                                           // matrix.print(Arduino.read_CI_V_TX_HOLDOFF_counter(), DEC);
                                           // matrix.print(Arduino.get_CONTROLLER_Operating_Mode(), DEC); 
                                           // matrix.print(commutator, DEC);  
                                           // matrix.writeDisplay(); // adds 1.5ms of delay
                                           //matrix.print(Icom.get_RX_Message_validity (), DEC );
                                           //matrix.print(diffMillis, DEC);
               // matrix.print(LOCAL_KBD.get_last_button_pressed_repeat_counter()) ;
                                            // FrontLEDmeter.get_peak_TTL_count()
                matrix.print(FrontLEDmeter.get_peak_TTL_count(), DEC);
                matrix.writeDisplay(); // adds 1.5ms of delay 

                                     
                                                
           // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
           // ++++++++++++++++++++++++++++++++++++++++++     MACHINE MODE switch    ++++++++++++++++++++++++++++++++++++++++++++++++
           // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
        
            switch ( Arduino.get_CONTROLLER_Operating_Mode()) // checks what mode on each loop 
                                         
                                          { //SWITCH OPERATING MODE 

                                           // ***************************************
                                           //   MACHINE FAULT mode
                                           // ***************************************                                            
                                           
                                            case 0:  //   MACHINE FAULT mode
                                           

                                            break ; // end of case 0
                                           
                                        
                                         // ***************************************
                                         //   INITIALISE MACHINE mode  get NTP ( work in Progress )
                                         // ***************************************

                                           case 1:   //    MACHINE mode : get NTP
                                          
                                           delay(400) ;
                   
                                           Arduino.set_CONTROLLER_Operating_Mode(4)  ; // exit this state on next loop
                                           break ; // end of case 1
                                           
                                           
                                         // ***************************************
                                         //   CI-V / Ehternet Bridge ( work in Progress )  - no !
                                         // ***************************************

                                           case 4:   //    MACHINE mode :  CI-V / Ehternet Bridge
  
                                           delay(400) ;
                                           Arduino.set_CONTROLLER_Operating_Mode(6)  ; // exit this state on next loop                      
                                           
                                           break ; // end of case 4                                       
                                                                 
                                       // ****************************************************************
                                       //  CI-V controller /mode switch / Band Switch and S-meter display
                                       // ****************************************************************                                                                    
                                                                                            
                                           case 6 : // MACHINE DATA mode  ...  stays in '6' after initialisation unless fault detected
                                       
 
                                                    // *******************************************************************************
                                                    // ***************************    SCAN BUTTONS   *********************************
                                                    // *******************************************************************************
                      
                                                    // ...scan buttons... return an index ... if greater than 0 send a command to the CI-V interface
          
                 // use different flag               if ( Arduino.get_CI_V_TX_HOLDOFF_flag() == false )     // dont scan buttons if hold off is triggered i.e. true
                                                    // {
                // add scan ihibit toggle here --------> use KBD.object  
                                                    button_scan_index = Scan_buttons()  - 96 ;   // if a button pressed ...change ASCII to integer 0 - ?
                // no button pressed  = 122 (ascii)

                // any button pressed : range 97 to 111  [ LSB = 97, 160m = 107 ] , [USB = 98, 80m = 108 ] , [ AM = 99, 40m = 109 ] , [ CW = 100, 20m = 111 ]


                                                    // NOW TRIGGER SEQUENCE ?  0 = NO KEY  = USUAL SEQUENCE == VCO - Smeter - MODE - Smeter -  repeat...
                                                    //                         97  = LSB
                                                    //                         98  = USB
                                                    //                         99  = AM
                                                    //                         100 = CW

                                                    //                         107 = Band 160m - LSB  - repeats while button pressed
                                                    //                         108 = Band  80m - USB
                                                    //                         109 = Band  40m - AM
                                                    //                         111 = Band  20m - CW

                                                                      
                                                    #ifdef PRO_MINI
                                                    if ( button_scan_index < 25 ) 
                                                                                {
                                                                                  Arduino.set_CI_V_TX_inhibit_flag(false) ; // ..... TRANSMIT INHIBIT DISABLED                                         
                                                                                  tx_ci_v_via_serial_ttl(button_scan_index) ; 
                                                                                  Arduino.set_CI_V_TX_inhibit_flag(true) ; // ..... TRANSMIT INHIBIT ENABLED
                                                                                }
                                                                                // tx_ci_v_via_serial_ttl(Scan_buttons());  // ...scan buttons... return an index ... if greater than 0 send a command to the CI-V interface
                                                    #endif
                                        
                                                    #ifdef MEGA2560 
                                                   // output info to USB serial monitor
                                                   // move / change this to a SWITCH statement CASE  
                                                    if ( (button_scan_index < 25) && (button_scan_index > 0) ) 
                                                                                {
                                                                              
                                                                                  Arduino.set_CI_V_TX_inhibit_flag(false) ; // ..... TRANSMIT INHIBIT DISABLED  

                                                                                  // **************************************************************
                                                                                  // button value ( 0 - 15 ?) initiates message transmission
                                                                                  // FUTURE CHANGE will initiate a sequence of message transmissions
                                                                                  // ***************************************************************
                                                                                  tx_ci_v_via_serial_ttl(button_scan_index) ;   


                                                                                  Arduino.set_CI_V_TX_inhibit_flag(true) ; // ..... TRANSMIT INHIBIT DISABLED 
                                                                                }
                                                              #endif 
                                                                                                        // }
                                                                                   
                                                                                                                                                                                            
                                                                                    // *******************************************************************************
                                                                                    // *********************           flash buttons           ***********************
                                                                                    // *******************************************************************************  
                                                                                        
                                                                                     //CW_Button_LED.twinkle();// if "flash" attribute SET TRUE will flashing button LED now (done inside the object)
                                                                                     //AM_Button_LED.twinkle();
                                                                                     //LSB_Button_LED.twinkle();
                                                                                     //USB_Button_LED.twinkle();
                                                                                                                                                                                                                         
                                                                                    // *******************************************************************************************************************************************************
                                                                                    // Listen.... to CI-V one wire bus via hardware interface to TTL RX (pin 19) - do this on every loopto indicate the state of the wire
                                                                                    // ******************************************************************************************************************************************************
                                                                                                                                                                                   
                                                                                    //Arduino.set_listen_for_CI_V_message_flag(true) ;  // we may listen
                                                                                    
                                                                                    RX1_COUNTER = listen_TTL_Serial_port () ;  // 2020JAN12: 0k : listening to CI-V circuit for input (or loop back) on serial1 TTL port  (2560 pin 19)
                                                                                                                               // returns the number of bytes in the from the default arduino 64 byte buffer
                                                                                                                               // DOESN'T remove any bytes from the default arduino 64 byte buffer
                                                                                                                               // i.e. if line was quiet, RX1_COUNTER = 0
                                                                                    if (RX1_COUNTER >= 1 )  
                                                                                                          { // busy line .... take FAIL path to the left
                                                                                                          // Yellow1LED.on() ; // something in CI-V (RX1 ) buffer
                                                                                                           delay(5) ;
                                                                                                           //Yellow1LED.off() ;
                                                                                                          }
                                        

                                                                                    // *******************************************************************************************************************************
                                                                                    //  PRE CURSOR to conditional TRANSMIT ... if permitted   .. will tx on CI-V one wire bus via hardware interface to TTL TX (pin 18)
                                                                                    // *******************************************************************************************************************************
                                                                                     //Icom.set_RADIO_connected(true)  ;  // temp force polling on                                      
                                                                                    // ....check THREE CONDITIONS ... FIRST nothing recently sent ...AND (logical AND) ...    
                                                                                    //     SECOND nothing recently received ..i.e. if no bytes in buffer ... 3rd only do it when CI-V holdoff is expired
                                                                                                                                            
                                                                                    if ( ( Arduino.get_just_sent_CI_V_message_flag() == false )
                                                                                    && (RX1_COUNTER == 0) && ( Arduino.get_CI_V_TX_HOLDOFF_flag() == false ) // polling radio and S-meter update is stopped by the holdoff flag too
                                                                                    && ( Icom.get_RADIO_connected() == true   ) )  // if radio not connected dayz will be zero therefore inhibit polling                      
                                                                                    {  // OPEN IF BRACKET 01 ..........can now send data request as the one wire bus is quiet  
 //                                                                                                   |
 //                                                                                                   |                        
 //                                                                                                   |                                                                
 //                                                                       <-----------false           |       true---------->           
 //                                                                                                   V                                                                         
 //                                    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< UNSOLICITED  <<<<<<<<<<<<<      >>>>>> SOLICITED >>>>>     ( nothing in the RX buffer, and I haven't recently sent a data request to the radio
 //                                   |                                                                                        |    ( most often down this rhs route ... i.e line is mostly quiet with few unsolicited messages )
 //                                   |                                                                                        |    ( unless you turn the VCO knob ! .. in which case a flood of unso;icited data arrives )
 //                                   V                                                                                        V
 //                                   V                                                                                        V
                                        //                                                             DOWN through here if ALL tests passed : this controller CAN send s CI-V request for data
                    // i.e. RX1_COUNTER is NOT zero etc                                                                      
                    //    down HERE if all 3 tests failed !                                            DOWN this route mostly !!    
                                                                                                                                                   
                                                           
                   //  down here if 3 tests failed                                                 //********************************************************************************************************************************
                                                                                                   // new logic here ... while VCO is not rotated i.e. while no unsloicited messages are received  
                                                                                                   // allow the polling of the radio ( get VCO, get MODE, get S-METER )  and allow reception of NETWORK packets
                                                                                                   //
                                                                                                   // But when VCO rotated and many unsolicited CI-V messages are received STOP polling the radio and STOP listening for NETWORK packets  
                                        
                                                                                                                                  
                                                                                                   Arduino.set_CI_V_TX_inhibit_flag(false) ; // ..... TRANSMIT INHIBIT DISABLED .... 9 dec temporary rem out and copy to else below to turn of polling while vco rotated
                                                                                                   //                                                      CAN NOW TRANMIT CI-V MESSAGE ...  inhibit is DISABLED .... i.e. false !
                                                                                                 
                                                                                                   /*
                                                           
                                                                                                  if ( Arduino.get_CI_V_TX_HOLDOFF_flag() == true ) //    . .this "get" also decrements the internal counter 100..99..98..97.. 
                                                                                                                                                               {                   // the flag is set to FALSE when internal counter hits zero
                                                                                                                                                                //vco  NOT rotated
                                                                                                                                                               
                                                                                                                                                                Arduino.set_CI_V_TX_inhibit_flag(true) ; //  stop polling
                                                                                                                                                               
                                                                                                                                                              
                                                                                                                                                               }
                                      
                                                                                                                                                           else // listen and polling inhibited .. .the 'get' also decrements the internal counter
                                                                                                                                                               {
                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                               Arduino.set_CI_V_TX_inhibit_flag(false) ;   // continue polling 
                                                                                                                                                                
                                                                                                                                                                //net_listen() ; 
                                                                                                                                                               }
                                      
//  down here if 3 tests failed    .       .          .                                             */                
                                      
                                                                                                // " 8...9.. or 10 "  determines what message is sent to the CI-V interface to request data from the radio
                                      
                                                                                                 //********************************************************************************************************************************
                                                          

//  down here if 3 tests failed and this will 
//  fail too as inhib should be set after last TX                                                //  DOWN through here if ALL tests passed 

//                     ||                                                                        ||                                                 
//                     ||                                                                        ||
//                   \ || /                                                                    \ || /                                                 
//                     \/                                                                        \/
//  down here if 3 tests plus 1 failed
                                      //                                                          *****************************************************************************
                                      //                                                                       enable CI-V transmission of polling messages
                                      //                                                          *****************************************************************************
                                       
                                      
                                      //                                                          ***********************************************************************************
                                      //                                                            and rotate through Request VCO(2) , Request S-meter(1,3,5), Request MODE(4) , Request S-meter
                                      //                                                          ***********************************************************************************                                                              
                                                                                                    
                                                                                                  switch (commutator ) 
                                          
                                                                                                     { // OPEN CASES
                                          
                                                                                                      case 2 : // request VCO 
                                      
                                                                                                                valid_data_flag  = tx_ci_v_via_serial_ttl(8); // send ci-v tx !
                                                                                                                 
                                                                                                                if(valid_data_flag== true){ 
                                                                                                                                             
                                                                                                                                           bit_bash(8) ;  // sort out the nibbles 
                                                                                                                                           
                                                                                                                                           Icom.set_current_frequency( get_frequency() ) ;  //  THE RADIO's CURRENT FREQUENCY SAVED IT IN OBJECT (RADIO) 
                                                                                                                                                                                     
                                                                                                                                             // *******************************
                                                                                                                                             // now loop to clear RX buffer 
                                                                                                                                             // ******************************* 
                                                                                                                                             
                                                                                                                                            memset(RX1_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);
                                                                                                                                             
                                                                                                                                            memset(ci_v_rx_DATA, 0, 15); 
                                              
                                                                                                                                             
                                                                                                                                           }                                  
                                                                                                      break ; // end case 2
                                              
                                                              
                                                                                                      case 4 : // request mode
                                                                                                             
                                                                                                             valid_data_flag  = tx_ci_v_via_serial_ttl(9); // ci-v tx !
                                                                                                                     
                                                                                                               if(valid_data_flag== true){ 
                                                                                                                
                                                                                                                                          bit_bash(9) ;  // sort out the nibbles 
                                                                                                                                          Icom.set_current_rx_mode(ci_v_rx_DATA[1]) ; //  
                                                                                                                                          LOCAL_KBD.set_current_rx_mode_token( mode_2_token (  ci_v_rx_DATA[1]  ) ) ;                                 
                                                                                                                                          
                                                                                                            // *******************************************************************************
                                                                                                            // ***************************    GET RADIO MODE   *******************************
                                                                                                            // *******************************************************************************
                                                                                                                                 
                                                                                                             
                                                                                                            //  ... use mode data from radio
                                                                                                            
                                                                                                             request_mode() ; // obtains the current mode from the radio ... and then drives the approriate button led
                                                                                                      
                                                                                                                                               // *******************************
                                                                                                                                               // now loop to clear RX buffer 
                                                                                                                                               // ******************************* 
                                                                                                                          
                                                                                                                          
                                                                                                                                               memset(RX1_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);
                                                                                                                                             
                                                                                                                                               memset(ci_v_rx_DATA, 0, 15); 
                                                                                                                                                                             
                                                                                                                                          }                                    
                                                                                                        break ; // end case 4

                                                                                                        // *******************************************************************************
                                                                                                        // *********************    WRITE TO LED S METER DISPLAY @ ***********************
                                                                                                        // *******************************************************************************            
                                                  
                                                                                                        case ( 1 ): // do this for 3 alternate loops request S meter data               
                                                                                                                   
                                                                                                                  request_s_meter () ;
                                                                                                                  S_meter2LED_index() ; // @ updates LED S-meter
                                                                                                                  
                                                                                                        break ; // end case 1 
                                      
                                                                                                        case ( 3 ): // do this for 3 alternate loops request S meter data               
                                                                                                                  
                                                                                                                  request_s_meter () ;
                                                                                                                  S_meter2LED_index() ; // @ updates LED S-meter
                                                                                                                  
                                                                                                         break ; // end case 3 
                                      
                                  
                                                                                                         } // CLOSES CASES    end of switch


                                                                                          commutator++ ; // 
                                                                                          if (commutator >= COMMUTATOR)
                                                                                                                      { 
                                                                                                                      commutator = 0 ; 
                                                                                                                      } 

         
//                 down here 
//                    ||
//                    \/                                                                          

//                 down here 
//                    ||
//                    \/

                                                             
                                                                                                      } // end of 3 condition test :  THIS SHOULD BE CLOSE BRACKET 01

                                                                                               else {


                                                                                                    delay (CIV_RX_SOLICITED_BUFFER_FILL_DELAY) ;  
                                                                                                   // hopefully to preserve timing when polling is disabled by dayz = 0

                                                                                        
                                                                                            }
  
                                                                    // *********************************************************
                                                                    // RETURNS TO HERE AFTER CI-V data Transmitted
                                                                    // *********************************************************
                                                                                            
                                                                    // ****************************************************************************************************************** 
                                                                    // now check LOGIC of mode switching in TAB N
                                                                    // ******************************************************************************************************************
                
                                                                    // Controller_mode_Logic () ;  // needs work in new logic ..see tab N .. new logic
                                                                       
                                                                    // ******************************************************************************************************************
                                                                    // *********************      ANYTHING  BELOW HERE IS NOT THE DESIRED PATH             ******************************
                                                                    // ****************************************************************************************************************** 
                                                                                        
                                                                    // ****************************************************************************************************************** 
                                                                    // we know TX bytes will NOW  be in Rx buffer...and also REPLY BYTES TOO unless radio is off or CI-V link is broken
                                                                    // ******************************************************************************************************************

// down here if 3 tests plus 1 failed------------> 

                                     
                                     //Arduino.set_listen_for_CI_V_message_flag(true)... use his to turn on unsoloiced listening

                  if (( Arduino.get_just_sent_CI_V_message_flag() == false ) &&  // .....NOTHING recently sent ..AND         
                  (RX1_COUNTER!= 0 )) // .....................................               SOMETHING has recently been received ......this is UN-SOLICITED DATA - MAYBE A BROADCAST MESSAGE - check 'from id' ?   
                                     { //  unsolicited message processing
                                      //  ***************************************************************************
                                      //  ***************************************************************************
                                      //  NETWORK LISTEN INHIBIT ENABLED ... cannot now listen for TIMETOLIVE loops counted down in the SOLICITED path with the .get command
                                      
                                      // ************************************************************************************************************
                                     // break ; //switch_off during debug
                                      delay(CIV_RX_UNSOLICITED_BUFFER_FILL_DELAY) ; // was 125ms
                      
                                      Arduino.set_CI_V_TX_HOLDOFF_counter(UNSOLICITED_HOLD_OFF) ; // stop polling
                                                                                                       // stop scanning buttons
                                                                                                       // stop updating the S-meter LEDs

                                       ci_v_ring_Buffer.clear() ;

                                      byte udp_count = 0 ;
                                      READ1_COUNTER =  read_Unsolicited_Message() ; // empty h/w buffer into rx_buffer and return
                                      // number of bytes... this needs a specific routine to handle unsolicited data rather than just flushing it ! ... WRITE NEW FUNCTION !
                                                          
                                                                    // *********************************************************
                                                                    //          send udp packet ! 
                                                                    // *********************************************************                                              
                                                                         
                                                                          READ1_COUNTER =0;
                                                                          RX1_COUNTER = 0 ;
                                                                          
                                                                         // *******************************
                                                                         // now loop to clear RX buffer 
                                                                         // ******************************* 
                                                      // set all bytes in the buffer to 0
                                                     // memset(packetBuffer, 0, NTP_PACKET_SIZE);
                                                    
       
                                                                         
                                                                         }   // 2020jan09 or use 'listen false' flag (above in line 206) 
                                                                         //to allow us to just dump this received unsolicited (perhaps broadcast) message 




                                                    // ************************************************************************************************************************************
                                                    //      at this point we may have 'looped sent data' AND 'reply data' in the buffer ... the length will depend on the message type ... which we know !
                                                    // ************************************************************************************************************************************
                                                    //    this SHOULDNT HAPPEN ?



                                                      if (( Arduino.get_just_sent_CI_V_message_flag() == true ) &&  //...SOMEthing recently sent ..AND                  
                                                      (RX1_COUNTER >= 1 )) // SOMEthing recently received ..i.e. there are bytes in the rx buffer 
                                                                           //... should be both the TX bytes and (by now the Reply) RX bytes this is probably SOLICITED DATA
                                                                         { 
                                                                         // TAB_R() ; 
                                                                         }

                                                      // if (address check ok ) 
                                                                      //   TAB T
                                                      
                                                      // *********************************************************
                                                      //             NOW check TO / FROM addresses
                                                      // *********************************************************
                                                                         
                                                      if (Arduino.get_collision_flag() == false) {

                                                                                                        //TAB_S 
                                                                                                      }

                                                         // *********************************************************
                                                         //       send it to PROCESING via usb serial
                                                         // *********************************************************

                                                          //SEND SERIAL packets
                                                          
                                                          // send_usb_serial() ; // this outputs serial data for the PROCESSING s/w running on other PC / MAC / LAPTOP  
                                                                                                 
                                                          //SEND NETWORK packets


                                                        // *********************************************************
                                                       //             collision occured ?
                                                       //  *********************************************************
                                                            
                                                             if (Arduino.get_collision_flag() == true) {

                                                                                                            //TAB_U
                                                                                                            } 

                                                   break ; // end of case 6 MACHINE MODE

                                            
                                                  // *********************************************************
                                                   //                  end of mode 6
                                                   // *********************************************************


 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 // ++++++++++++++++++++++++++++++++++++++++++  End of MACHINE MODE switch   +++++++++++++++++++++++++++++++++++++++++++++
 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
 
                                                    }// end of switch MACHINE CONTROLLER operating mode

 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 // ++++++++++++++++++++++++++++++++++++++++++  End of MACHINE MODE switch   +++++++++++++++++++++++++++++++++++++++++++++
 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
 

                Arduino.set_collision_flag(false) ;  // reset it .. why here
 
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++     COMMUTATOR SWITCH    ++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
               
 /*               
                switch (commutator) 
                                    {
 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                      case 0:
                                   
                                      break ;
 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
                                      case 1:

                                      
                                      break ;
 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
                                      case 2 :
                                    
               
                                      break ;
 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                      case 3 : 


                                      break ;
 // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 

                                    } // end of switch (commutator)
  */                                  
                                    
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // +++++++++++++++++++++++++++++++++++++++  End of COMMUTATOR SWITCH   ++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++           



                                                                                      
  stopMillis=millis() ;
                    
   diffMillis =stopMillis - startMillis ;

                       
               
               }  // end of MAIN LOOP    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  

 
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++          end of main loop        +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

             
// end of 1_control_loop tab

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
                                    { // receives a (byte) mode value from button press then returns a (char) symbol to compare with symbol from controller
int mode_id  = 99 ; 
                                        
                                    switch (mode_token)
                                                          {
                                                            case 'a' :  // LSB
                                                            mode_id = 0 ;
                                                            break ;
          
                                                            case 'b'  :  // USB
                                                            mode_id = 1 ;
                                                            break ;
          
                                                            case 'c' :  // AM
                                                            mode_id = 2 ;
                                                            break ;
          
                                                            case 'd' :  // CW
                                                            mode_id = 3 ;
                                                            break ;
                                                            
                                                            /*
                                                            case 'e' :  // FSK / RTTY
                                                            mode_id = 4 ;
                                                            break ;
                                                            */
                                                            
                                                            case 'f' :  // FM
                                                            mode_id = 5 ;
                                                            break ;
                                                            
                                                            case 'g' :  // WFM
                                                            mode_id = 6 ;
                                                            break ; 

                                                            default :
                                                            mode_id = 99 ;                                                                                                                           

                                                           
                                                          }// end of switch}
                                  return mode_id ;
                                  }

 /*    
* Mode ID's      my symbol   (source ICOM )   
*    00            a          LSB
*    01            b          USB
*    02            c          AM
*    03            d          CW
*    04            e          RTTY / FSK
*    05            f          FM
*    06            g          WFM
*    07            h          CW-R
*    08            i          RTTY-R / FSK-R
*    09            j
*    10            k 
*    11            l          S-AM(D)
*    12            m          PSK
*    13            n          PSK-R
*    14            o          S-AM(L)
*    15            p          S-AM(U)
*    16            q          P25
*    17            r          D-STAR
*    18            s          dPMR
*    19            t          NXDN-VN
*    20            u          NXDN-N
*    21            v          DCR
*/
// end of tab G

// start of tab H mode 2 token
char mode_2_token (byte icom_mode ) 
                                    {
                                        
                                     // mode_value =  icom_mode ;
                                      
                                      switch (icom_mode)
                                                            {
                                                              case 0 :  // LSB
                                                              return 'a' ;
                                                              break ;

                                                              case 1  :  // USB
                                                              return 'b' ;
                                                              break ;

                                                              case 2 :  // AM
                                                              return 'c' ;
                                                              break ;

                                                              case 3 :  // CW
                                                              return 'd' ;
                                                              break ;
                                                              /*
                                                              case 4 :  // RTTY
                                                              return 'e' ;
                                                              break ;
                                                              */
                                                              
                                                              case 5 :  // FM
                                                              return 'f' ;
                                                              break ;
                                                              
                                                              case 6 :  // WFM
                                                              return 'g' ;
                                                              break ;

                                                              
                                                              default:
                                                              return '`' ;
                                                                                                                                                                                         
                                                        
                                                             
                                                            }// end of switch}
   
                                    }


// put other functions here before setup and loop 

// start 2_button loop


                //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++                                                      
                //  ++++++++++++++++++++++                     scanbuttons               +++++++++++++++++++++++++++  
                //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
            //             SS  SS  SS           CC CC CC CC                AA              NN N           NN
            //          SS           SS       CC                         AA  AA            NN  NN         NN
            //         SS                   CC                          AA    AA           NN   NN        NN
            //          SS                                             AA      AA          NN    NN       NN
            //            SS                CC                        AA        AA         NN     NN      NN
            //               SS             CC                       AA          AA        NN      NN     NN
            //                  SS          CC                      AA AA AA AA AA A       NN       NN    NN
            //                     SS       CC                     AA             AA       NN        NN   NN
            //                      SS      CC                    AA               AA      NN         NN  NN
            //                       SS     CC                    AA               AA      NN         NN  NN
            //                     SS         CC                  AA               AA      NN          NN NN
            //          SS  SS  SS              CC CC CC CC       AA               AA      NN           N NN

            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


char Scan_buttons() 
                {

                    int buttonVoltage = 0 ;
                  
                    buttonVoltage = analogRead(BUTTONS); // READ A0  .... no button press returns a value obout 1023  (5v)

                    //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++                                                      
                    //  ++++++++++++++++++++++  TEST FOR NO BUTTON PRESED   ++++++++++++++++++++++++++++++++++++++++++++  
                    //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


                                                  //  +++++++++++++++++++++++++++++++++++++++++  
                    if ( buttonVoltage > 1000)    //          no buttom pressed
                                                  //  +++++++++++++++++++++++++++++++++++++++++ 
                                              {
                                                //for a 'radio' button functionality ... first clear any buttons previously pressed  ;
                                              //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
                                                if (LOCAL_KBD.get_any_button_pressed_toggle() == false) 
                                                            {
                                                              LOCAL_KBD.set_button_pressed_NOW_TOKEN('`') ; // set 'none' pressed token
                                                            }                                           
                                                
                                                
                                              //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 

                                                LOCAL_KBD.set_last_button_pressed_repeat_counter(0) ; //  clear the (repeat) counter

                                              //  +++++++++++++++++++++++++++++++++++++++++++++++++++++

                                              if (LOCAL_KBD.get_any_button_pressed_toggle() == true )      // try to send only when key released ?         
                                                          {
                                                          LOCAL_KBD.set_any_button_pressed_toggle(false);  // reset the toggle 
                                                          return LOCAL_KBD.get_button_pressed_NOW_TOKEN( ) ;  // NOW or last  ?
                                                          }
                                              else 
                                                  {
                                                    return LOCAL_KBD.get_button_pressed_NOW_TOKEN() ;  // no button pressed - quit function 
                                                  }

                                              

                                              }

                    //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++                                                      
                    //  ++++++++++++++++++++++         A  BUTTON IS PRESED  !        +++++++++++++++++++++++++++++++++++  
                    //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   


                                            //  +++++++++++++++++++++++++++++++++++++++++ 
                  if ( buttonVoltage < 600) //  one of the mode button must be pressed  .. i.e below about 2.7v  
                     {                      //  +++++++++++++++++++++++++++++++++++++++++ 


                                            //  +++++++++++++++++++++++++++++++++++++++++ 
                                            //           DEBOUNCE IT !
                                            //  +++++++++++++++++++++++++++++++++++++++++ 

                          if (LOCAL_KBD.get_last_button_pressed_repeat_counter() <= 2 )
                                                  {
                                                  #ifdef MEGA2560 
                                                  Yellow1LED.on() ;
                                                  #endif
                                                  delay(1) ;   
                                                  // this plus two loops makes de-bounce 
                                                  //about 40 - 50 ms
                                                  #ifdef MEGA2560
                                                  Yellow1LED.off() ;
                                                  #endif
                                                  LOCAL_KBD.set_last_button_pressed_repeat_counter
                                                  (LOCAL_KBD.get_last_button_pressed_repeat_counter() + 1) ; 
                                                  // bump the counter 
                                                  return 0 ; // not debounced yet -  quit the function and loop around ... if key still pressed
                                                  }
                          else 
                              { 
                                // debounce done ... and key still pressed
                                LOCAL_KBD.set_last_button_pressed_repeat_counter(LOCAL_KBD.get_last_button_pressed_repeat_counter() + 1) ; 
                                LOCAL_KBD.set_any_button_pressed_toggle(true); // use this toggle to only send ONE button code ?  
                                // now continue BUT must SKIP OVER setting .any_button_pressed_NOW somewhere
                              }  

                              //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++                                                      
                              //  ++++++++++++++++++++++    TEST FOR WHICH BUTTON  PRESED  !   +++++++++++++++++++++++++++++++++++  
                              //  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++   

                          switch (buttonVoltage) 
                                         {                         
                                            case 0 ... 40 : // 400 ... 600 :
                                            // first button 'LSB'

                                            USB_Button_LED.off() ;
                                            AM_Button_LED.off() ;
                                            CW_Button_LED.off() ;
                          //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
                                            LOCAL_KBD.set_button_pressed_NOW_TOKEN(MODE_A) ;  // 'a' this physical button sets token to
                                                                              //  'a'  this will initiate tx of mode command fo LSB
                          //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++                 
                                            break ; 

                                            case 90 ... 140 : //300 ... 380 :
                                            // second button 'AM'
                                            LSB_Button_LED.off() ;
                                            USB_Button_LED.off() ;
                                            CW_Button_LED.off() ;
                            //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                            LOCAL_KBD.set_button_pressed_NOW_TOKEN(MODE_C) ; // 'c' mode token - AM this physical button 
                                            //   sets token to 'c' this will initiate tx of mode command fo AM
                            //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++              
                                            break ;

                                            case 250 ... 330 : // 120 ... 250 :
                                            // third button 'USB'
                                            LSB_Button_LED.off() ;
                                            AM_Button_LED.off() ;
                                            CW_Button_LED.off() ;
                            //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                            LOCAL_KBD.set_button_pressed_NOW_TOKEN(MODE_B) ; // mode token is 'b'  -USB   this physical 
                                            //     button sets token to 'b' this will initiate tx of mode command fo USB
                            //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++            
                                            break  ;

                                            case 450 ... 500:  //  less than 2.5v ? ... more than 2.0v ?
                                            // forth button 'FM or CW'
                                            LSB_Button_LED.off() ;
                                            USB_Button_LED.off() ;
                                            AM_Button_LED.off() ;
                            //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++                        
                                            LOCAL_KBD.set_button_pressed_NOW_TOKEN(MODE_D) ; // mode token is 'd'  -CW   this physical button
                                            //  sets token to 'd' this will initiate tx of mode command fo CW
                            //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++             
                                            break ;

                                          }   // end of switch           
                                      
                       }  // end of if  Mode_buttonVoltage


                            // *******************************************************************************
                            // save the button pressed token as requested mode  ... should it be band first then mode on a long press ?

                            LOCAL_KBD.set_requested_rx_mode_token( LOCAL_KBD.get_button_pressed_NOW_TOKEN() );  // ascii value for output  MOVE THIS  OUTSIDE ? <------------------------------------------------- 
                    
                            // *******************************************************************************      


                            // *******************************************************************************
                            // *********** key_press_value  is now between 97 and 100   or 122  **************
                            // *******************************************************************************
                          

                          // *******************************************************************************
                          // a button was pressed during this loop
                            if  (  LOCAL_KBD.get_button_pressed_NOW_TOKEN() != '`'  ) // masks out the Z's
                                {   
                                  // is the button previously pressed same as now ?                           
                                  // *******************************************************************************
                                  if ( LOCAL_KBD.get_button_pressed_NOW_TOKEN() == LOCAL_KBD.get_last_button_pressed_TOKEN() )  
                                  // --------------------------------------------------------------------------------------
                                      {
                                       #ifdef MEGA2560
                                        Yellow2LED.on();
                                        #endif MEGA2560
                                        delay(1) ;
                                        #ifdef MEGA2560
                                        Yellow2LED.off();
                                        #endif
                                        LOCAL_KBD.set_last_button_pressed_repeat_counter(LOCAL_KBD.get_last_button_pressed_repeat_counter() + 1) ; 
                                        
                                                                                      // ..YES... start counting how many loops it remains pressed
                                      }
                                  else 
                                      {
                                        LOCAL_KBD.set_last_button_pressed_repeat_counter( 0 ) ;    // ................... 
                                        //        ............... NO  not the same key as last loop ... clear the counter
                                      }
                                                                                            
                                  // .button has been held in LONGER then BAND_JUMP value therefore 
                                  // this is a BAND change request, not a mode change request BAND_JUMP
                                  // *******************************************************************************
                                  if ( LOCAL_KBD.get_last_button_pressed_repeat_counter() >= BAND_JUMP )  
                              
                                  {                                                                  
                                  #ifdef MEGA2560
                                  GreenLED.on();
                                  #endif
                                  delay(1) ;
                                  #ifdef MEGA2560
                                  GreenLED.off();
                                  #endif
                                  LOCAL_KBD.set_last_button_pressed_repeat_counter(0) ;  // clear counter

                                  // first  button held down...YES ....request jump to BAND by sending TRANSCIEVE FREQUENCY CI-V message
                                  if (LOCAL_KBD.get_button_pressed_NOW_TOKEN() == MODE_A)  
                                              {
                                              // tx_ci_v_via_serial_ttl(11) ; 
                                              // request band 1 frequency  160m   *** try return value instead of direct call 
                                              #ifdef MEGA2560
                                              RedLED.on() ;
                                              #endif
                                              delay(1) ;
                                              #ifdef MEGA2560
                                              RedLED.off() ;
                                              #endif
                                              //LOCAL_KBD.set_requested_rx_band_token(BAND_A);  // SET BAND    MOVE THIS OUTSIDE                                                                 
                                              LOCAL_KBD.set_button_pressed_NOW_TOKEN(BAND_A) ;    // SET BUTTON
                                              //requested band = band a
                                              }
                                                                                      
                                      if (LOCAL_KBD.get_button_pressed_NOW_TOKEN() == MODE_C) // AM button held down
                                              {
                                              // tx_ci_v_via_serial_ttl(12) ; // request band 2 frequency  80m
                                              //LOCAL_KBD.set_requested_rx_band_token(BAND_B) ;
                                              LOCAL_KBD.set_button_pressed_NOW_TOKEN(BAND_C) ;
                                              }

                                        if (LOCAL_KBD.get_button_pressed_NOW_TOKEN() == MODE_B) // USB button held down
                                              {
                                              // tx_ci_v_via_serial_ttl(13) ; // request band 3 frequency  40m
                                              // LOCAL_KBD.set_requested_rx_band_token(BAND_C);
                                              LOCAL_KBD.set_button_pressed_NOW_TOKEN(BAND_B) ;
                                              }

                                        if (LOCAL_KBD.get_button_pressed_NOW_TOKEN() == MODE_D) // CW held down
                                              {
                                              // request band 5 frequency  20m
                                              //LOCAL_KBD.set_requested_rx_band_token(BAND_D);
                                              LOCAL_KBD.set_button_pressed_NOW_TOKEN(BAND_D) ; 
                                                // ==================================
                                              }


                                        // button actions                                               
                                        // **********************************************************
                                        //   copy frequency  ( 5 bytes ) from band store to array THIS IS BAND SWITCH !!! 
                                        //... if it had Set VCO command  <----------- move this ?
                                        // **********************************************************
                                        if ( Icom.get_current_band() != 0) 
                                            {           
                                            for ( int i = 0 ; i <= 4 ; i++ )
                                                  {
                                                  ci_v_TX1_RX1_MESSAGEs[Icom.get_current_band() +10][3+i] =
                                                  band_store[Icom.get_current_band()][i]  ; //   11 - 15                                         
                                                  } 
                                            } 
                                  }  // end of band jump if
                                      
                         

                                  // maybe make this function return a "key_press_value" value to select and send the TX message .
                                  //.. could return 0 (do nothing) 1 - 4 send mode change... 11 - 14  band change
                                                                                                  
                                                                                              
                               }        // closing bracket of if ... !- '`' 

                        LOCAL_KBD.set_last_button_pressed_TOKEN( LOCAL_KBD.get_button_pressed_NOW_TOKEN() );    // could be abcd or klmo    

                          
          // now sort out button LED 
                    // button actions                                               
                    // **********************************************************
           if (   mode_2_token  (   Icom.get_current_rx_mode() ) ==  LOCAL_KBD.get_requested_rx_mode_token() )
                                                                                                                    {
                                                                                                                      // what ?
                                                                                                                    }
    return  LOCAL_KBD.get_button_pressed_NOW_TOKEN() ; 

               // LOCAL_KBD.get_button_pressed_NOW_TOKEN() ; 
               // an ascii value .. moved from end  
                                        
               // how about set a valid flag when TOKEN set to non 'z' ( actually '`')
               // .. and return the single key press value 


    
               // LOOK at the call to hare and add processing after it to SEND one of more messages :-)
  }   // end of function  





//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++                                                      
//  ++++++++++++++++++++++       get mode function      ++++++++++++++++++++++++++++++++++++++++++++  
//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void request_mode() 
              {
              //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              //   CI-V data from radio advises MODE .... MODE from radio is the only thing 
              //   which lights the LED except for local flash command
              //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
                //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          switch (    mode_2_token(Icom.get_current_rx_mode() )   )  //  gets radio MODE and convert to token 
                //  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                {
                    case 'a' :  // LSB from radio
                  // stop buttons flashing ...and also turns the button led off
                    USB_Button_LED.set_flash(false) ;
                    AM_Button_LED.set_flash(false) ;
                    CW_Button_LED.set_flash(false) ;
                    LSB_Button_LED.on() ;
                    break ;
                    case 'b' : // USB from radio
                    LSB_Button_LED.set_flash(false) ; 
                    // stop buttons flashing ...and also turns the button led off
                    AM_Button_LED.set_flash(false) ;
                    CW_Button_LED.set_flash(false) ;
                    USB_Button_LED.on() ;
                    break ;
                    case 'c' : // am from radio
                  LSB_Button_LED.set_flash(false) ; 
                  USB_Button_LED.set_flash(false) ;
                  // stop buttons flashing ...and also turns the button led off
                  CW_Button_LED.set_flash(false) ;
                    AM_Button_LED.on() ;
                    break ;
                    case 'd' : // CW from radio
                  LSB_Button_LED.set_flash(false) ; 
                  USB_Button_LED.set_flash(false) ;
                  AM_Button_LED.set_flash(false) ;
                  // stop buttons flashing ...and also turns the button led off
                  CW_Button_LED.on() ;
                    break ;
                    default :  // if token = '`'
                    LSB_Button_LED.set_flash(false) ; //this stop button flash...and turns the button led off
                    USB_Button_LED.set_flash(false) ;
                    AM_Button_LED.set_flash(false) ;
                    CW_Button_LED.set_flash(false) ;
                    break ;  
                }
              }
// end 2_button loop
// end of get mode function 
//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++                                                      
//  ++++++++++++++++++++++       get s-meter function     ++++++++++++++++++++++++++++++++++++++++++  
//  ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// orphan from tab 5
void request_s_meter () 
                       {
                        tx_ci_v_via_serial_ttl(10); // ci-v tx !
                                      
                        bit_bash(10) ;  // sort out the nibbles 
                  
// new calc
                      Icom.set_current_s_meter( ( (ci_v_rx_DATA[1])  *100 ) + ( (ci_v_rx_DATA[2] ) *10 ) + (  ci_v_rx_DATA[3] ) ) ;

                      // old calc
                      // Icom.set_current_s_meter( ( Icom.get_s_meter_hundreds() *100) + (Icom.get_s_meter_tens() * 10 ) +  (Icom.get_s_meter_units()) ) ;  

                      FrontLEDmeter.calculate_current_MAPPED_value(Icom.get_current_s_meter() );  // ( Icom.get_current_s_meter() )
                      // this returns an S-meter value, but also initiates the calcs inside the object to 
                      //generate the low, high and max1, max 2 indices 

 
                      }
// end of orphan from tab 5
 

void S_meter2LED_index() {
      
      // pass these of 'get' from radion object
      
      byte ave_index ;
      byte low_index = FrontLEDmeter.get_s_meter_low_index()     ;
      byte high_index = FrontLEDmeter.get_s_meter_high_index()  ;
      byte ARCstart ;
      byte ARCstop ;

      byte max_index ;
      byte MaxIndex1 =  FrontLEDmeter.get_s_meter_peak_Index_LO()   ;
      byte MaxIndex2 = FrontLEDmeter.get_s_meter_peak_Index_HI()   ;

Serial.println ( ) ;
Serial.print ("  FrontLEDmeter.get_s_meter_peak_Index_LO = ") ;
Serial.print (FrontLEDmeter.get_s_meter_peak_Index_LO() ) ;

Serial.print ("  FrontLEDmeter.get_s_meter_peak_Index_LO  =  " ) ;
Serial.println (FrontLEDmeter.get_s_meter_peak_Index_LO() ) ;


/*
// put all this index creation inside the radio object  and rem it out here - start  of rem
 #ifdef LED_8_MONO
              if (Icom.get_s_meter_low() <= 120)
                                            {
                                             low_index = map ( Icom.get_s_meter_low(), 0, 120, 0, 5 ) ; // map  
                                            }
                  if (Icom.get_s_meter_low() >= 121)
                                            {
                                             low_index = map ( Icom.get_s_meter_low(), 121, 240, 0, 3 ) ; // map 
                                             low_index = low_index + 9 ;
                                            }
                  if (Icom.get_s_meter_high() <= 120)
                                            {
                                             high_index = map ( Icom.get_s_meter_high(), 0, 120, 0, 5 ) ; // map  
                                            }
                  if (Icom.get_s_meter_high() >= 121)
                                            {
                                             high_index = map ( Icom.get_s_meter_high(), 121, 240, 0, 3 ) ; // map 
                                             high_index = high_index + 9 ;
                                            }   
                  if (Icom.get_s_meter_mov_ave() <= 120)
                                            {
                                            ave_index = map ( Icom.get_s_meter_mov_ave(), 0, 120, 0, 5 ) ; // map  
                                            }
                  if (Icom.get_s_meter_mov_ave() >= 121)
                                            {
                                            ave_index = map ( Icom.get_s_meter_mov_ave(), 121, 240, 0, 3 ) ; // map 
                                            ave_index =ave_index + 9 ;
                                            }
 #endif 

 #ifdef LED_8_RGB
              if (Icom.get_s_meter_low() <= 120)
                                            {
                                             low_index = map ( Icom.get_s_meter_low(), 0, 120, 0, 5 ) ; // map  
                                            }

                  if (Icom.get_s_meter_low() >= 121)
                                            {
                                             low_index = map ( Icom.get_s_meter_low(), 121, 240, 0, 3 ) ; // map 
                                             low_index = low_index + 9 ;
                                            }
                  if (Icom.get_s_meter_high() <= 120)
                                            {
                                             high_index = map ( Icom.get_s_meter_high(), 0, 120, 0, 5 ) ; // map  
                                            }
                  if (Icom.get_s_meter_high() >= 121)
                                            {
                                             high_index = map ( Icom.get_s_meter_high(), 121, 240, 0, 3 ) ; // map 
                                             high_index = high_index + 9 ;
                                            }   
                  if (Icom.get_s_meter_mov_ave() <= 120)
                                            {
                                            ave_index = map ( Icom.get_s_meter_mov_ave(), 0, 120, 0, 5 ) ; // map  
                                            }
                  if (Icom.get_s_meter_mov_ave() >= 121)
                                            {
                                            ave_index = map ( Icom.get_s_meter_mov_ave(), 121, 240, 0, 3 ) ; // map 
                                            ave_index =ave_index + 9 ;
                                            }
 #endif 

 #ifdef LED_16_MONO
                  if (Icom.get_s_meter_low() <= 120)
                                            {
                                             low_index = map ( Icom.get_s_meter_low(), 0, 120, 0, 9 ) ; // map  
                                            }
                  if (Icom.get_s_meter_low() >= 121)
                                            {
                                             low_index = map ( Icom.get_s_meter_low(), 121, 240, 0, 6 ) ; // map 
                                             low_index = low_index + 9 ;
                                            }
                  if (Icom.get_s_meter_high() <= 120)
                                            {
                                             high_index = map ( Icom.get_s_meter_high(), 0, 120, 0, 9 ) ; // map  
                                            }
                  if (Icom.get_s_meter_high() >= 121)
                                            {
                                             high_index = map ( Icom.get_s_meter_high(), 121, 240, 0, 6 ) ; // map 
                                             high_index = high_index + 9 ;
                                            }   
                  if (Icom.get_s_meter_mov_ave() <= 120)
                                            {
                                            ave_index = map ( Icom.get_s_meter_mov_ave(), 0, 120, 0, 9 ) ; // map  
                                            }
                  if (Icom.get_s_meter_mov_ave() >= 121)
                                            {
                                            ave_index = map ( Icom.get_s_meter_mov_ave(), 121, 240, 0, 6 ) ; // map 
                                            ave_index =ave_index + 9 ;
                                            }
 #endif 

 #ifdef LED_16_RGB 

                  if (Icom.get_s_meter_low() <= 120)
                                            {
                                             low_index = map ( Icom.get_s_meter_low(), 0, 120, 0, 9 ) ; // map  
                                            }
                  if (Icom.get_s_meter_low() >= 121)
                                            {
                                             low_index = map ( Icom.get_s_meter_low(), 121, 241, 1, 6 ) ; // map 
                                             low_index = low_index + 9 ;
                                            }
                  if (Icom.get_s_meter_high() <= 120)
                                            {
                                             high_index = map ( Icom.get_s_meter_high(), 0, 120, 0, 9 ) ; // map  
                                            }
                  if (Icom.get_s_meter_high() >= 121)
                                            {
                                             high_index = map ( Icom.get_s_meter_high(), 121, 241, 1, 6 ) ; // map 
                                             high_index = high_index + 9 ;
                                            }   
                  if (Icom.get_s_meter_mov_ave() <= 120)
                                            {
                                            ave_index = map ( Icom.get_s_meter_mov_ave(), 0, 120, 0, 9 ) ; // map  
                                            }
                  if (Icom.get_s_meter_mov_ave() >= 121)
                                            {
                                            ave_index = map ( Icom.get_s_meter_mov_ave(), 121, 241, 1, 6 ) ; // map 
                                            ave_index =ave_index + 9 ;
                                            }

 
// move this to the radio object or create an S-meter object ?


                        if (Icom.get_s_meter_high() <= 120)
                                                  {
                                                  max_index = map ( Icom.get_s_meter_high(), 0, 120, 0, 9 ) ; // map  
                                                  }
                        if (Icom.get_s_meter_high() >= 121)
                                                  {
                                                  max_index = map ( Icom.get_s_meter_high(), 121, 241, 1, 6 ) ; // map 
                                                  max_index = max_index + 9 ;
                                                  }
                      


      switch (max_index) 
                       {
                     

                      case 0 :
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0xFF;// high byte
                      break  ;

                      case 1 :
                      MaxIndex1 =0x7F;// low byte
                      MaxIndex2 =0xFF;// high byte
                      break ;

                      case 2 :       
                      MaxIndex1 =0xBF;// low byte
                      MaxIndex2 =0xFF;// high byte 
                      break  ; 

                      case 3 :       
                      MaxIndex1 =0xDF;// low byte
                      MaxIndex2 =0xFF;// high byte
                      break  ; 

                      case 4 :       
                      MaxIndex1 =0xEF;// low byte
                      MaxIndex2 =0xFF;// high byte
                      break  ; 

                      case 5 :       
                      MaxIndex1 =0xF7;// low byte
                      MaxIndex2 =0xFF;// high byte
                      break  ;

                      case 6 :       
                      MaxIndex1 =0xFB;// low byte
                      MaxIndex2 =0xFF;// high byte
                      break  ;

                      case 7 :        
                      MaxIndex1 =0xFD;// low byte
                      MaxIndex2 =0xFF;// high byte
                      break  ;

                      case 8 :
                      MaxIndex1 =0xFE;// low byte
                      MaxIndex2 =0xFF;// high byte
                      break  ;

                      case 9 :       
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0x7F;// high byte 
                      break  ; 

                      case 10 :       
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0xBF;// high byte
                      break  ; 

                      case 11 :       
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0xDF;// high byte
                      break  ; 

                      case 12 :       
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0xEF;// high byte
                      break  ;

                      case 13 :       
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0xF7;// high byte
                      break  ;

                      case 14 :       
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0xFB;// high byte
                      break  ; 

                      case 15 :       
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0xFD;

                      case 16 :  
                      MaxIndex1 =0xFF;// low byte
                      MaxIndex2 =0xFE;// high byte     
                      break  ;

                     // default : 
                     // MaxIndex1 =0xFF;// low byte
                     // MaxIndex2 =0xFF;// high byte     
                     // break  ;

                      }
                     matrix.print(MaxIndex1, DEC);
                     matrix.writeDisplay(); // adds 1.5ms of delay 
 #endif 
// put all this index creation inside the radio object  and rem it out here - end of rem 
*/

// ***************************************************************
// *********** now send index values to draw the LED arc *********
// ***************************************************************

if (  (low_index != high_index ) && ( low_index > high_index)  )// if lowest and highest are not same
                            {

                            for (int j = low_index; j <= high_index; j++)   // light LEDs from lowest to highest
                                                     {                          
                                                      digitalWrite(STORE_PIN, LOW);
                                                                LED_arc (j,MaxIndex1,MaxIndex2) ; // 'j' is current index  maxindex 1 and 2 are peak values) 
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
                                                                                                          LED_arc (j,MaxIndex1,MaxIndex2) ; // peak hold 
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
                                                                                                                  LED_arc (j,MaxIndex1,MaxIndex2) ; // peak hold 
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
//                                was void LED16_rgb(byte led)   ....  renamaed to the generic void LED_arc(byte led) AND encapsulated
//  =======================================================================================================================================
/*
void LED_arc(byte led) // LED_16_RGB 
                    {//start
                    byte index = led ;

                    byte red1 ;
                    byte green1 ;
                    byte blue1 ;
                    byte red12 ;
                    byte green2 ;
                    byte blue2 ;

                    // byte led_pwm = shine ;

                     switch ( index ) {//switch
                                       case 0 :  // s0
                                       // Zeroth LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red   off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green on..........on
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue.....off ALL OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 1


                                       case 1 :  // s0
                                       // Zeroth LED only  1 BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red   off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green on..........on
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B01111111); // 1st blue.....off 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 2 :  // s1
                                       // first LED only 2 BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00111111); // 1st blue .....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 2
                                       case 3 :  // s2
                                       // second LED only 3 BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00011111); // 1st blue.....off 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 4 :  // s3
                                       // Third LED only 4 BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00001111); // 1st blue.....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 3   
                                       case 5 :  // s4
                                       // Fifth LED only 5 BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000111); // 1st blue....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 4       
                                        case 6 :  // s5
                                       // Sixth LED only 6 BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000011); // 1st blue.....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 5       
                                        case 7 :  // s6
                                       // Seventh LED only 7 BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000001); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 6       
                                        case 8 :  // s7
                                       // Eigth LED only 8 BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 7       
                                        case 9 :  // s8
                                       // Nineth LED only 8 BLUE + 1 green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 8       
                                         case 10 :  // s+9
                                       // Tenth LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 9       
                                          case 11 :  // s+10
                                       // Eleventh LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11011111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 10     
                                           case 12 :  // s+20
                                       // Twelth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11001111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green are green and blue transposed ?
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 20                    
                                           case 13 :  // s+30
                                       // Thirteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 30                    
                                           case 14 :  // s+40
                                       // Fourteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000011); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 40                      
                                           case 15 :  // s+50
                                       // Fifteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000001); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 50                        
                                            case 16 :  // s+60
                                       // sixtenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000000); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 60                        
                                        } // end of switch
                    } // end of function

*/
void LED_arc(byte led, byte Max1, byte Max2) // LED_16_RGB 
                    {//start
                    byte index = led ;
                    Serial.println() ;
                    Serial.print( "Max1 = " ) ;
                    Serial.print( Max1 ) ;
                    Serial.print( "  Max2 = " ) ;
                    Serial.println( Max2 ) ;

                    byte MaxHold1 = Max1;  // make this an object variable instead of an argument to the function
                    byte MaxHold2 = Max2;  
                    byte red1   = 0 ;
                    byte green1 = 0 ;
                    byte blue1  = 0 ;
                    byte red2   = 0 ;
                    byte green2 = 0 ;
                    byte blue2  = 0 ;
// try analog out 
                   moving_coil(index);

                    // byte led_pwm = shine ;
                    




                     switch ( index ) {//switch
                                       case 0 :  // s0
                                       // Zeroth LED ALL OFF
                                                      red1   = B11111111; // 0XFF 1st red  
                                                      green1 = B11111111; // 0XFF 1st green 
                                                      blue1  = B11111111; // 0XFF 1st blue.....off ALL OFF

                                                      red2   = B11111111; // 0XFF 2nd red
                                                      green2 = B11111111; // 0XFF 2nd green
                                                      blue2  = B11111111; // 0XFF 2nd blue
                                        break ; //  end of case 0


                                       case 1 :  // s1
                                       // 1st LED only 1 BLUE
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B01111111; // 0X7F 1st blue

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 1
                                       case 2 :  // s2
                                       // 2nd LED only 2 BLUE
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00111111; // 0X3F 1st - 2ND blue 

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 2
                                       case 3 :  // s3
                                       // 3rd LED only 3 BLUE
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green
                                                      blue1  = B00011111; // 0X1F 1st -  3RD blue.....off

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 3
                                       case 4 :  // s4
                                       // 4th LED only 4 BLUE
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00001111; // 0X0F 1st - 4TH blue.....blue

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 4   
                                       case 5 :  // s5
                                       // Fifth LED only 5 BLUE
                                                      red1   = B11110111; // 1st red  *
                                                      green1 = B11111111; // 1st green
                                                      blue1  = B00000111; // 0X07 1st- 5TH  blue....blue

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 5       
                                        case 6 :  // s6
                                       // Sixth LED only 6 BLUE
                                                      red1   = B11110111; // 1st red  *
                                                      green1 = B11111111; // 1st green
                                                      blue1  = B00000011; // 0X03 1st - 6TH blue.....blue

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 6       
                                        case 7 :  // s7
                                       // Seventh LED only 7 BLUE
                                                      red1   = B11110111; // 1st red  *
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000001; // 0X01 1st - 7TH  blue

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 7       
                                        case 8 :  // s8
                                       // Eigth LED only 8 BLUE
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 0X00 1st - 8TH blue

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 8  

                                        // drive next PCB now ...

                                        case 9 :  // s9
                                       // Nineth LED  8 BLUE + 1 GREEN
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 0X00 1st - 8TH blue

                                                      red2   = B11111111; // 2nd red
                                                      green2 = B01111111; // 2nd    ONLY  GREEN 
                                                      blue2  = B11111111; // 2nd blue
                                        break ; //  end of case 9       
                                         case 10 :  // s9+10
                                       // Tenth LED  8 BLUE + 1 GREEN + 1 RED
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 0X00 1st blue
                                                      red2   = B10111111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B01111111; // 2nd blue
                                        break ; //  end of case 10       
                                          case 11 :  // s9+10
                                       // Eleventh LED 8 BLUE + 1 GREEN + 2 RED 
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 0X00 1st blue
                                                      red2   = B10011111; // 0XDF 2nd red
                                                      green2 = B11111111; // 0XBF 2nd green
                                                      blue2  = B01111111; // 2nd blue
                                        break ; //  end of case 10     
                                           case 12 :  // s9+20
                                       // Twelth LED  8 BLUE + 1 GREEN + 3 RED
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 0X00 1st blue
                                                      red2   = B10001111; // 0XCF 2nd red
                                                      green2 = B11111111; // 0XBF 2nd green
                                                      blue2  = B01111111; // 2nd blue
                                        break ; //  end of case 20                    
                                           case 13 :  // s9+30
                                       // Thirteenth LED  8 BLUE + 1 GREEN + 4 RED
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 1st blue
                                                      red2   = B10000111; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B01111111; // 2nd blue
                                        break ; //  end of case 30                    
                                           case 14 :  // s+40
                                       // Fourteenth LED  8 BLUE + 1 GREEN + 5 RED
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 1st blue
                                                      red2   = B10000011; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B01111111; // 2nd blue
                                        break ; //  end of case 40                      
                                           case 15 :  // s+50
                                       // Fifteenth LED  8 BLUE + 1 GREEN + 6 RED
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 1st blue
                                                      red2   = B10000001; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B01111111; // 2nd blue
                                        break ; //  end of case 50                        
                                            case 16 :  // s+60
                                       // sixtenth LED  8 BLUE + 1 GREEN + 6 RED
                                                      red1   = B11111111; // 1st red  
                                                      green1 = B11111111; // 1st green 
                                                      blue1  = B00000000; // 1st blue
                                                      red2   = B10000000; // 2nd red
                                                      green2 = B11111111; // 2nd green
                                                      blue2  = B01111111; // 2nd blue
                                        break ; //  end of case 60                        
                                        } // end of switch

red1   = red1   &  MaxHold1 ; // combine ( AND ) NOW average value (red1) with PEAK to give White LED PEAK Indicator
green1 = green1 &  MaxHold1 ; // MaxHold1 and MaxHold2 are derived from S.meter data
blue1  = blue1  &  MaxHold1 ; // and converted to a bit mask for indeces 1-16... try and move MaxHold to object
red2   = red2   &  MaxHold2 ;
green2 = green2 &  MaxHold2 ;
blue2  = blue2  &  MaxHold2 ;
//digitalWrite(STORE_PIN, LOW);
shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, red1); // 1st red
shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, green1); // 1st green 
shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, blue1); // 1st blue
shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, red2); // 2nd red
shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, green2); // 2nd green
shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, blue2); // 2nd blue
//digitalWrite(STORE_PIN, HIGH);
                    } // end of function

   #endif
                    

 #ifdef LED_16_MONO  
//  =======================================================================================================================================
//                                was void LED16_mono(byte led)   ....  renamaed to the generic void LED_arc(byte led) AND encapsulated
//  =======================================================================================================================================
//  16 MONO START ---->>>>>>>>> NOT TESETED YET  to be edited
void LED_arc (byte led)
                    {//start
                    byte index = led ;
                    // byte led_pwm = shine ;
                     switch ( index ) {//switch
                                       case 0 :  // s0
                                       // Zeroth LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red   off   LHS
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green on..........on
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B01111111); // 1st blue.....off 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue  mono .................. THIS ONE ????
                                        break ; //  end of case 1
                                       case 1 :  // s1
                                       // first LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00111111); // 1st blue .....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 2
                                       case 2 :  // s2
                                       // second LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00011111); // 1st blue.....off 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 3 :  // s3
                                       // Third LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00001111); // 1st blue.....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 3        
                                       case 4 :  // s4
                                       // Fifth LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000111); // 1st blue....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 4       
                                        case 5 :  // s5
                                       // Sixth LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000011); // 1st blue.....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 5       
                                        case 6 :  // s6
                                       // Seventh LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000001); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 6                                   
                                        case 7 :  // s7
                                       // Eigth LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 7       
                                        case 8 :  // s8
                                       // Nineth LED only BLUEE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 8       
                                         case 9 :  // s+9
                                       // Tenth LED only BLUE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 9       
                                          case 10 :  // s+10
                                       // Eleventh LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11011111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 10     
                                           case 11 :  // s+20
                                       // Twelth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11001111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 20                    
                                           case 12 :  // s+30
                                       // Thirteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 30                    
                                           case 13 :  // s+40
                                       // Fourteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000011); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 40                      
                                           case 14 :  // s+50
                                       // Fifteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000001); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 50                        
                                            case 15 :  // s+60
                                       // sixtenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00000000); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11000000); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 60                        
                                        } // end of switch
                    } // end of function

   #endif


                    // save a copy 

 #ifdef LED_8_RGB  

void LED_arc(byte led)
                    {//start
                    byte index = led ;
                    // byte led_pwm = shine ;
                     switch ( index ) {//switch
                                       case 0 :  // s0
                                       // NO LED      OFF OFF OFF OFF OFF OFF OFF OFF 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red   ALL off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 1 :  // s1
                                       // first       BLU OFF OFF OFF OFF OFF OFF OFF 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 2
                                       case 2 :  // s3
                                       // second       BLU BLU OFF OFF OFF OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 3 :  // s5
                                       // Third       BLU BLU BLU OFF OFF OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00011111); // 2nd blue
                                        break ; //  end of case 3      
                                       case 4 :  // s7
                                       // Forth       BLU BLU BLU BLU OFF OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111); // 2nd blue
                                        break ; //  end of case 4       
                                        case 5 :  // s9
                                       // Fifth       BLU BLU BLU BLU GRN OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111); // 2nd blue
                                        break ; //  end of case 5       
                                        case 6 :  // s9+20
                                       // Sixth       BLU BLU BLU BLU GRN RED OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111011); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111); // 2nd blue
                                        break ; //  end of case 6                                    
                                        case 7 :  // s9+40
                                       // Seventh     BLU BLU BLU BLU GRN RED RED OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111001); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111); // 2nd blue
                                        break ; //  end of case 7 
                                       case 8 :  // s9+60
                                       // Eigth       BLU BLU BLU BLU GRN RED RED RED
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111000); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B00001111); // 2nd blue
                                        break ; //  end of case 7 
                                        } // end of switch
                    } // end of function

   #endif



#ifdef LED_8_MONO  
void LED_arc(byte led)
                    {//start
                    byte index = led ;
                    // byte led_pwm = shine ;
                     switch ( index ) {//switch
                                       case 0 :  // s0
                                       // // NO LED   OFF OFF OFF OFF OFF OFF OFF OFF 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 1 :  // s1
                                       // first       BLU OFF OFF OFF OFF OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B01111111); // 2nd blue
                                        break ; //  end of case 2
                                       case 2 :  // s2
                                       // second      BLU BLU OFF OFF OFF OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 3 :  // s3
                                       // Third       BLU BLU BLU OFF OFF OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B00011111); // 2nd blue
                                        break ; //  end of case 3       
                                       case 4 :  // s4
                                       // Fifth       BLU BLU BLU BLU OFF OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00001111); // 2nd blue
                                        break ; //  end of case 4       
                                        case 5 :  // s5...S9
                                       // Sixth       BLU BLU BLU BLU GRN OFF OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00000111); // 2nd blue
                                        break ; //  end of case 5       
                                        case 6 :  // s6
                                       // Seventh     BLU BLU BLU BLU GRN RED OFF OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00000011); // 2nd blue
                                        break ; //  end of case 6                                    
                                        case 7 :  // s7
                                       // Eigth       BLU BLU BLU BLU GRN RED RED OFF
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00000001); // 2nd blue
                                        break ; //  end of case 7  
                                        case 8 :  // s7
                                       // Eigth       BLU BLU BLU BLU GRN RED RED RED
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST,   B00000000); // 2nd blue
                                        break ; //  end of case 7  
                                        } // end of switch
                    } // end of function
 #endif
// END OF LED ARC FUNCTION VARIATIONS

 

// end of tab 6 LED switch
// start of tab 5_LED_loop
//wasin tab5.. void get_S_meter ()


// end of tab_5_led loop


// start tab A RX listen or READ ( solicited messages )
//  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//       RX  listen serial 1 FUNCTION
//  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

byte listen_TTL_Serial_port () 
                          {
// this function returns the number of bytes in the Arduino serial buffer (which has a capacity of 64 bytes)
// ... it does NOT remve any

 #ifdef MEGA2560
                          {
                              return Serial1.available();  // returns true if bytes ARE in h/w buffer
                          }
  #endif



 #ifdef PRO_MINI
                          {
                              return Serial.available();  // returns true if bytes ARE in h/w buffer
                          }
  #endif

                          }

// this next Function accepts CI-V data via the 'one wire' hardware interface and RX pin 19 
// AND FILLS the ARDUINO's RX BUFFER ... returns the count of bytes received

// the rx buffer may contain UNSOLICITED messages ie. bytes received which may be BROADCAST 
//or a copy of bytes SENT (by this controller device) 
//
//data should be :-   FE (254)  FE (254)  to(anyone  id 00 = 000)   from(my radio 96 = 150).
//.....ends with FD (253) 
//
//

// but not SOLICITED reply messages i.e. replies to requests sent by this device
// 
// FE (254)  FE (254)  to(usuall me this id E0 = 224)   from(my radio 96 = 150)......ends with FD (253) 


/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//         END OF listen serial 1 FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//         RX serial 1 RECEIVE UNSOLICITED MESSAGE FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 


            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
            //        RR  RR  RR  RR  RR  RR                   XX XX XX                XX XX XX
            //        RR                    RR                     XX                    XX
            //        RR                     RR                      XX                 XX
            //        RR                     RR                       XX               XX
            //        RR                     RR                         XX            XX
            //        RR                    RR                            XX        XX
            //        RR                  RR                               XX      XX
            //        RR                RR                                  XX    XX
            //        RR  RR  RR  RR  R                                        XX       
            //        RR               RR                                       XX     
            //        RR                 RR                                  XX  XX
            //        RR                   RR                               XX     XX
            //        RR                    RR                             XX       XX
            //        RR                     RR                           XX         XX
            //        RR                      RR                         XX           XX
            //        RR                      RR                        XX             XX
            //        RR                      RR                   XX XX XX          XX XX XX
            //
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



// start of tab B new CI-V in ( unsolicited messages)
byte read_Unsolicited_Message()
//Unsolicited_CI_V_input()
                             {//CI_V in
                               #ifdef MEGA2560
                               BlueLED.on();
                               #endif
                               delay(1);
                               #ifdef MEGA2560
                               BlueLED.off(); 
                               #endif                             
                               boolean message_flag = false ;
                               byte count = 0 ;                      
                               byte last_incoming_FD =0; 
                               byte frame_end_FD_count = 0 ;
                               byte frame_start_FE_count = 0 ;
                               byte packet_terminator_count = 0 ;
                               byte incoming_TTL1_Byte;
                               boolean one_star = false ;

                                                   //clear buffer
                                                   //memset(packetBuffer, 0, NTP_PACKET_SIZE);
                                                   //memset(NET_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);
                                                   
                              
                                                  //  *******************************************************
                                                  //                  CI_V Received ???
                                                  //  *******************************************************

      #ifdef MEGA2560                   
                    while (Serial1.available() > 0) // while bytes from CI-V intrefaceare waiting to be read do it  
                          { 
                          message_flag = true ;
                          incoming_TTL1_Byte = Serial1.read();  // get first or next byte  <<<--------------------- 
                          //saves incoming byte in 'incoming_TTL1_Byte' variable
                          RX1_MESSAGE_BUFFER[count] = incoming_TTL1_Byte ;  // copy incomming byte into an rx array
                          ci_v_ring_Buffer.push(RX1_MESSAGE_BUFFER[count]);
                            count++ ; // count bytes
                            if ( count >= 46 ) 
                                              { 
                                              break ; 
                                              } // bug out  was 46 for 4 lines of CI-v messages but lost part of
                                              // third line after 23 chars
                          } // end of while 

                          if ( message_flag == true ) 
                            {//begin_if message flag  
                              startMillis=millis() ; 
                              message_flag = false ;

                              //  *******************************************************
                              //              read the packet into packetBufffer
                              //  *******************************************************

                            //  ************************************************************************************************************
                            //                               format check
                            //  ************************************************************************************************************
                            for (int i=0 ; i<=47 ;  i++)   // begin_for copy
                                {
                                // now print & simple check on what was SENT
                                  if ( byte(RX1_MESSAGE_BUFFER[i]) == 0xFD ) {
                                                                              frame_end_FD_count ++ ;
                                                                              last_incoming_FD = i+1 ;
                                                                             // Serial.println ('.') ;
                                                                             // Serial.println("FD") ;
                                                                              }

                                  if(byte(RX1_MESSAGE_BUFFER[i]) ==0xFE)    // look for FE
                                                                            {
                                                                            frame_start_FE_count ++ ; // this should be even
                                                                            // and twice the FD count
                                                                           // Serial.println("FE") ;
                                                                            }

                                  if(byte(RX1_MESSAGE_BUFFER[i]) ==0xFA)    // look for FE
                                                                            {
                                                                            frame_start_FE_count ++ ; // this should be even 
                                                                            //and twice the FD count
                                                                            //Serial.println("FA") ;
                                                                            }

                                } // end of for copy
                              }//message flag true
                                            
                                                  else 
                                                      {
                                                      
                                                      }
                               #endif

                               #ifdef PRO_MINI                  
                                      while (Serial.available() > 0) // while bytes from CI-V intrefaceare waiting to be read do it  
                                            { 
                                            message_flag = true ;
                                            incoming_TTL1_Byte = Serial.read();  // get first or next byte 
                                            // <<<--------------------- saves incoming byte in 'incoming_TTL1_Byte' variable
                                            RX1_MESSAGE_BUFFER[count] = incoming_TTL1_Byte ;  // copy incomming byte into an rx array 
                                            ci_v_ring_Buffer.push(RX1_MESSAGE_BUFFER[count]);
                                            count++ ; // count bytes
                                            if ( count >= 46 ) { break ; } // bug out  was 46 for 4 lines
                                            // of CI-v messages but lost part of third line after 23 chars
                                            } // end of while                                              
                                if ( message_flag == true ) 
                                    {//begin_if message flag 
                                              
                                              startMillis=millis() ; 
                                              message_flag = false ;
                                              //  *******************************************************
                                              //              read the packet into packetBufffer
                                              //  *******************************************************

                                            //  ************************************************************************************************************
                                            //                               format check
                                            //  ************************************************************************************************************

                                            for (int i=0 ; i<=47 ;  i++)   // begin_for copy
                                                                          {
                                                                          // now print & simple check on what was SENT                                                                                                                                             
                                                                            if ( byte(RX1_MESSAGE_BUFFER[i]) == 0xFD ) {
                                                                                                                        frame_end_FD_count ++ ;
                                                                                                                        last_incoming_FD = i+1 ;
                                                                                                                        //Soft_Serial.println ('.') ;
                                                                                                                        }
                                                                            if(byte(RX1_MESSAGE_BUFFER[i]) ==0xFE)    // look for FE
                                                                                                                      {
                                                                                                                      frame_start_FE_count ++ ; 
                                                                                                                      // this should be even and twice the FD count
                                                                                                                      }
                                                                          } // end of for copy
                                    }//message flag true                    
                                else 
                                    {
                                    
                                    }               
                              #endif                                             
                          return count ;                   
                         } // end of CI_V listen


            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            
            //        TT TT TT TT TT TT TT TT                  XX XX XX                XX XX XX
            //                  TT                                 XX                    XX
            //                  TT                                   XX                 XX
            //                  TT                                    XX               XX
            //                  TT                                      XX            XX
            //                  TT                                        XX        XX
            //                  TT                                         XX      XX
            //                  TT                                          XX    XX
            //                  TT                                             XX       
            //                  TT                                              XX     
            //                  TT                                           XX  XX
            //                  TT                                          XX     XX
            //                  TT                                         XX       XX
            //                  TT                                        XX         XX
            //                  TT                                       XX           XX
            //                  TT                                      XX             XX
            //                  TT                                 XX XX XX          XX XX XX
            //
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// end of tab B (unsolicited)

// start of tab C CI-V comms ( basically TX via serial1)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//         TX serial 1 FUNCTION
//  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//

  byte tx_ci_v_via_serial_ttl (byte sender)  // 'sender' is the CI-V CMD byte   
                              // note : this function returns the DATA VALID FLAG                              
                              // all precedded by FE  FE  96  E0 ( 253  253  150  224 ...
                              //    { ...  3, 253,   0,   0,   0,   0}, //  8 what current frequency ... 6 bytes sent 
                              //    { .... 4, 253,   0,   0,   0,   0}, //  9 what current mode ... 6 bytes sent 
                              //    { ... 21,   2, 253,   0,   0,   0}, //  10 what S-meter value ? ..... 7 bytes sent
                              //    { ... 0, a, b, c, d, e, 253      }  // 11 bytes sent  reply would be 6 bytes FE FE E0  96 FB FD

     {////01 TX1  - start of  function       >>>>>>>>>>>>>>>>>>>>>>>>>>>>-------------------------------------------01---------------->>>   
        byte result ;
        byte tx_count = 0 ; // will hold the number of bytes sent
        byte rx_count = 0 ; // will hold the number of bytes sent (looped back) and bytes received
        byte incoming_TTL1_Byte;
        byte FEat = 0 ;  // use to move along RX buffer array in tests below tx
        byte TOat = 0 ;
        byte FROMat = 0 ;
        byte type = 0 ;  // reply type  o = none  1 = FB or FA   2=DATA 
        byte FDat = 0 ;  // where the FD byte should be
        byte FBat = 0 ;
        startMillis=millis() ; 
             
        if ( sender == 0) 
                        {//02
                        return 0 ; 
                        }//\\02 block TX of '0th' table entry
                                                                    //  *******************************
        if (Arduino.get_CI_V_TX_inhibit_flag() 
        == false) 
        { //03 false... then TX is permitted  -------------------------------------------------------------------------------03------->>>

                                                                    //  *******************************  

            // *****************************************************************************************************************
            // ASSEMBLE CI-V message to send ... use control character  FIRST load TX1_MESSAGE_BUFFER FE  FE  to..  from ...  etc 
            // *****************************************************************************************************************

            TX1_MESSAGE_BUFFER[0] = 254 ;  //  FE
            TX1_MESSAGE_BUFFER[1] = 254 ;  //  FE
            TX1_MESSAGE_BUFFER[2] = Icom.get_hex_id() ;  // 8600 = 96   7300 = 94
            TX1_MESSAGE_BUFFER[3] = 224 ;  //  E0       from ....assumes E0H controller id
            #ifdef MEGA2560

            //  ********************************************************************************************************************
            //          Load the PAYLOAD into the transmit buffer 'TX1_MESSAGE_BUFFER' from ci_v_TX1_RX1_MESSAGEs[25][11]
            //  ********************************************************************************************************************

            for (int i=0 ; i<15 ;  i++) // note : 'break' will exit the loop before END is reached
                                        // prepare buffer ... i steps through the ci_v_TX1_RX1_MESSAGEs[row = sender] 
                                        //[column = bytes to copy]  ) array ... load correct ROW into TX1_MESSAGE_BUFFER
                                        {//04
                                        //                                           row    col
                                        TX1_MESSAGE_BUFFER[i+4] = ci_v_TX1_RX1_MESSAGEs[sender][i+2] ; // i+ 4 skips the tx 
                                        //message header that is pre loaded above , i+2 skips the tx and rx anticipated byte count values
                                        if (TX1_MESSAGE_BUFFER[i+4] == 253) // step through the array (and SEND) up to and
                                                                            // including '253' = 'FD'
                                        //....time to bail out of for loop
                                                                            {//05
                                                                              break ; // exit for loop 
                                                                            }//\\05
                                        } //\\ 04 end for loop 

                     
                      //    *************************************************************************************************
                      //        NOW SEND the whole CI-V TX1_MESSAGE_BUFFER     
                      //    *************************************************************************************************
                      // i steps through the array // ..sending each byte  
                      //note : 'break' will exit the loop WHEN 0XFD (253) is seen before END is reached
                      for (int i=0 ; i<15 ;  i++)         
                            {//06 
                              Serial1.write (TX1_MESSAGE_BUFFER[i]) ; // THIS ACTUALLY SENDS THE CI-V DATA via pin 18 TTL 1 TX
                              if (TX1_MESSAGE_BUFFER[i] == 253) // step through the array (and SEND) up to and including '253' = 'FD'.
                                      //               ..then time to bail out of for loop
                                 {//07
                                  result = i ; //  result =  0,1,2,3,4,5,....n  but n+1 are sent
                                  tx_count = i+1 ; // adjust the tcx count for the jupmed loop   
                                  break ; // exit for loop // note, again that 'break' will exit the loop before 15 is reached
                                 }//\\07
                            } // \\06   end for loop
                                               
              #endif  // end of ifdef 2560            

                                        //  duplicate code but for pro mini serial port

              #ifdef PRO_MINI
                              
                              //  ************************************************************

                              //  THEN load the PAYLOAD into the transmit buffer 
                              //  'TX1_MESSAGE_BUFFER' from ci_v_TX1_RX1_MESSAGEs[25][11]

                              //  ************************************************************

                              for (int i=0 ; i<15 ;  i++) // note : 'break' will exit the loop before END is reached
                                  // prepare buffer ... i steps through the ci_v_TX1_RX1_MESSAGEs[row = sender] 
                                  //[column = bytes to copy]  ) array ... load correct ROW into TX1_MESSAGE_BUFFER
                                  {//08
                                  //                                           row    col
                                  TX1_MESSAGE_BUFFER[i+4] = ci_v_TX1_RX1_MESSAGEs[sender][i+2] ; // i+ 4 skips the tx
                                  // message header that is pre loaded above , i+2 skips the tx and rx anticipated byte count values
                                  if (TX1_MESSAGE_BUFFER[i+4] == 253) // step through the array (and SEND) up to and including
                                  // '253' = 'FD'....time to bail out of for loop
                                                                      {//09
                                                                          break ; // exit for loop 
                                                                      }//\\09
                                  } //\\08 end for loop 

                                Soft_Serial.print ( "TX1 (5335):  ") ;
                                //    *************************************************************
                                //        NOW SEND the whole CI-V TX1_MESSAGE_BUFFER     
                                //    *************************************************************
                                // i steps through the array // ..sending each byte  
                        
                                for (int i=0 ; i<15 ;  i++)         
                                    { //10
                                      Serial.write (TX1_MESSAGE_BUFFER[i]) ; // THIS ACTUALLY SENDS THE CI-V DATA via pin 18 TTL 1 TX
                                      // show TX
                                                 
                                      if ( byte(TX1_MESSAGE_BUFFER[i]) <10 ) 
                                              {
                                              
                                              Soft_Serial.print ("0") ; // print leading zero
                                              Soft_Serial.print (byte(TX1_MESSAGE_BUFFER[i]),HEX) ;
                                              Soft_Serial.print (" ") ;
                                              }
                                                  
                                    else { 
                                          Soft_Serial.print( byte(TX1_MESSAGE_BUFFER[i]),HEX) ;
                                          Soft_Serial.print (" ") ;
                                          }                    
                                      


                                      if (TX1_MESSAGE_BUFFER[i] == 253) // step through the array (and SEND) up to and 
                                                                        //including '253' = 'FD'....time to bail out of for loop
                                          {//11
                                            result = i ; //  result =  0,1,2,3,4,5,....n  but n+1 are sent
                                            tx_count = i+1 ; // adjust the tcx count for the jupmed loop  
                                            break ; // exit for loop // note, again that 'break'
                                                    //  will exit the loop before 15 is reached
                                          }//\\11
                                    } //\\10 end for loop

                                     Soft_Serial.println() ;
                                

                    #endif  // -pro mini

                                  // *********************************************************
                                  //              CI_V message sent  - now set TX flags 
                                  // *********************************************************
                                  // set flags...  moved from tab A 'main loop'
                                  Arduino.set_CI_V_TX_inhibit_flag(true) ; // STOP further tx until a VALID loop back
                                  // AND reply received and checked 
                                  // i.e. TX is a ONE SHOT until inhibit flipped
                                  Arduino.set_just_sent_CI_V_message_flag(true) ;  // we have 'just sent' a message 
                                  //(loop back needs to be veirfied)
                                  Arduino.set_listen_for_CI_V_message_flag(false) ;   //  stop listen ... don't 
                                  //transfer any othe recived bytes into 
                                  //'our' rx buffer until processing / checking / validation process completed
        
                                  // *********************************************************
                                  //  start hold off counter : 6 loops  ?    need to limit polling to about 300ms intervals
                                  // *********************************************************                         
                                  Arduino.set_CI_V_TX_HOLDOFF_counter(HOLDOFF_RATIO) ; // e.g. 20 ... now cannot send a 
                                  //polling message for 20 loops
                                  
                                  // *********************************************************                                 
                                  delay (CIV_RX_SOLICITED_BUFFER_FILL_DELAY) ; // about 15ms after TX of a request for data,
                                  // the delay is to allow reply to arrive in the RX buffer
                                  // *********************************************************

          } //\\03 eof  TX inhibit false  <<<-----------------------------------------------------------------------------------------<<<---03

                              else { //12 skip through when ...Arduino.get_CI_V_TX_inhibit_flag() == false

                                    return ;  // normally drop past here to RX  below  OR exit  function when X inhib 
                                    //was true ( during HOLD OFF loops )
    
                                    }//\\12 eof false TX inhibit true
                                                                      
                                      // *********************************************************
                                      // CI-V request AND the Reply should be in rx buffer
                                      // *********************************************************
                                
                                      // what if no reply ( radio not connected ? )  .... RX buffer length should be same 
                                      //as TX buffer, or may also contain collision rubbish 
                                      //
                                      // or every thing normal ...
                              
                              //  ************    NOW READ the whole CI-V RX1_MESSAGE_BUFFER      **********

                              #ifdef MEGA2560              
                                             

                                    while (Serial1.available() > 0) //read AND copy into buffer... rx_count starts at ZERO
                                          {///13   
                                          incoming_TTL1_Byte = Serial1.read();  // get first or next byte  <<<---- 
                                          //moves incoming byte in 'incoming_TTL1_Byte' variable..  buffer now empty ?
                                          RX1_MESSAGE_BUFFER[rx_count]  = incoming_TTL1_Byte ;  // copy incomming byte 
                                          //into the global RX array   rx_count 0..1..2.. 
                                          // print out the HEX values when the ifdef allows                                            
                                          
                                          // could add ring buffer here ?
                                          rx_count++ ; // count them
                                          } //\\13 end of while
                                          
                                              
                              #endif // -2560

                              #ifdef PRO_MINI 
                                              
                                              Soft_Serial.print ( "RX1 (5440): ") ;
                                              while (Serial.available() > 0) //read AND copy and EMPTY   all bytes in buffer ... that will be SENT message and Reply message from buffer
                                                    {//14   
                                                                                                  
                                                      incoming_TTL1_Byte = Serial.read();  // get first or next byte  <<<--
                                                      //------------------- saves incoming byte in 'incoming_TTL1_Byte' variable
                                                      RX1_MESSAGE_BUFFER[rx_count]  = incoming_TTL1_Byte ;  // copy incomming byte into the global RX array
                                                     
                                                         
                                                      if ( byte(RX1_MESSAGE_BUFFER[rx_count]) <10 ) 
                                                                                          {
                                                                                          
                                                                                          Soft_Serial.print ("0") ;
                                                                                          Soft_Serial.print (byte(RX1_MESSAGE_BUFFER[rx_count]),HEX) ;
                                                                                          Soft_Serial.print (" ") ;
                                                                                          }
                                                                                                  
                                                                                    else { 
                                                                                          Soft_Serial.print( byte(RX1_MESSAGE_BUFFER[rx_count]),HEX) ;
                                                                                          Soft_Serial.print (" ") ;
                                                                                          }
                                                                                          

                                                      rx_count++ ; // count them
                                                    } //\\14 end of while
                                         
                                              Soft_Serial.println() ;

                              #endif // -pro mini

                              //   *********************************************************
                              //   do validity chech HERE then reset flags - compare TX (sent bytes) and part of RX bytes ...
                              //   i.e are the looped back 'TX bytes' uncorrupted
                              //    *********************************************************
                            
                                // first test ... is RX count greater than TX count ?   i.e. if TX = n and RX = 0 ... no device reply ? or no link to radio

                              //   *********************************************************
                              //             CHECK SENT MESSAGE IS NOT CORRUPTED
                              //   *********************************************************

                                if ( rx_count >= tx_count )
                                                      {//15
                                                      Arduino.set_TX_MESSAGE_NOT_CORRUPT_flag(true) ;  //assume it's ok  
                                                      //... maybe qualify that e.g. if RXcount >= TX count set it true
                                                      }//\\16

                                for (int i=0 ; i<tx_count ;  i++)  //  
                                      // i steps through first part of the array  which is the TX message  .......................WHAT if TX=0 and RX=0 ?
                                    {//17
                                          // move this to RX check below Arduino.set_TX_MESSAGE_NOT_CORRUPT_flag(true) ; 
                                          // set flag to true - i.e. assume ok unless error detectec in 'compare' below
                                          // RX1_MESSAGE BUFFER will contain TX'd and RX'd message
                                      if (RX1_MESSAGE_BUFFER[i]
                                        != TX1_MESSAGE_BUFFER[i])         // if not same PRINT SIDE BY SIDE
                                        {//18
                                          
                                          Arduino.set_TX_MESSAGE_NOT_CORRUPT_flag(false) ;  // assume it is false if 
                                          //rx and tx lenght are same ?  or always until proven ?
                                          
                                          #ifdef MEGA2560 
                                                          RedLED.on() ;  // led off after last tx related action 
                                                          Serial.println(" collisssion !!! "); // create collision error if TX and looped RX are different
                                                          Serial.print ( "TX1 :") ;
                                                          for (int i=0 ; i<tx_count ;  i++)
                                                                                          {//19
                                                                                            Serial.print (TX1_MESSAGE_BUFFER[i],HEX) ;
                                                                                          }//\\19
                                                                                          Serial.println() ;
                                                          Serial.print ( "RX1 :") ;
                                                          for (int i=0 ; i<tx_count ;  i++)
                                                                                          {//20
                                                                                            Serial.print (RX1_MESSAGE_BUFFER[i],HEX) ;
                                                                                          }//\\20
                                          #endif 
                                         
                                          #ifdef PRO_MINI 
                                                          //Soft_Serial.println(" collisssion !!! "); // create collision error if TX and looped RX are different
                                                          Soft_Serial.print ( "TX1 :  ") ;
                                                          for (int i=0 ; i<tx_count ;  i++)
                                                                                          {//21
                                                                                            Soft_Serial.print(TX1_MESSAGE_BUFFER[i],HEX) ;
                                                                                          }//\\21
                                                                                          Soft_Serial.println() ;
                                                          Soft_Serial.print ( "RX1 : ") ;
                                                          for (int i=0 ; i<tx_count ;  i++)
                                                                                          {//22
                                                                                            Soft_Serial.print (RX1_MESSAGE_BUFFER[i],HEX) ;
                                                                                          }//\\22
                                          #endif 

                                        }//\\18
                                        else
                                        {//23
                                          
                                          Arduino.set_TX_MESSAGE_NOT_CORRUPT_flag(true) ;  // assume it is false if rx and tx lenght are same ?  
                                                                                                //or always until proven ?
                                        }//\\23
                                          
                                    }

                                        if (Arduino.get_TX_MESSAGE_NOT_CORRUPT_flag() == true)
                                                                                            //  move all the stuss below into here
                                                                                          {//24
                                                                                             #ifdef MEGA2560
                                                                                            //  
                                                                                            Serial.println ( "TX1 :  good ") ;

                                                                                            #endif


                                                                                            #ifdef PRO_MINI   
                                                                                            Soft_Serial.println ( "TX1 :  good ") ;

                                                                                            #endif

                                                                                          }//\\24

                                          // *******************************
                                          //       clear TX buffer 
                                          // ******************************* 
                                        
                                            memset(TX1_MESSAGE_BUFFER, 0, 20); 

                                          //  SHOULD ANY FURTHER PROCESSING BE CONDITIONAL ON passing the VALIDITY CHECK ?
                                          //  yes !
                                          // *****************************************************************
                                          // ************    NOW process received CI-V data payloads    ******
                                          // *****************************************************************
                                            
                                          if (( Arduino.get_TX_MESSAGE_NOT_CORRUPT_flag() == true) 
                                                                    & ( rx_count >0 )& (tx_count > 0)  )
                                                                                                        
                                                                                                        
                                {//25  // conditional start of processing payload

                                          // ci_v_TX1_RX1_MESSAGEs[snder][13] is the reply type
                                          // reply FE1 is at tx_count ... FEat this
                                        
                                            FEat = ci_v_TX1_RX1_MESSAGEs[sender][0]   ;  // the position in the RX Buffer where the first FE should be located
                                            TOat = FEat + 2 ;
                                            FROMat = FEat + 3 ;



                                            type = ci_v_TX1_RX1_MESSAGEs[sender][13] ; // the position in the RX Buffer where the Reply type id should be located

                                            FDat = (  (  ci_v_TX1_RX1_MESSAGEs[sender][0] 
                                                      + ci_v_TX1_RX1_MESSAGEs[sender][1]) -1 ); // the position in the RX Buffer where FD should be located

                                            if (FDat > 47) 
                                              { //26  
                                              Serial.print (   "HELP !!!  ") ; 

                                              
                                              }//\\26

                                            FBat = FEat + 4 ; // the position in the RX Buffer where FB or FA should be located

                                            
                                            if ( RX1_MESSAGE_BUFFER[ FEat ]  == 0xFE ) 
                                                {  //27  is this value  = FE ?
                                              
                                                Icom.set_FE1_flag (true) ; 
                                                Icom.set_RX_Message_validity (128) ;
                                                //Icom.set_RX_Message_validity ( Icom.get_RX_Message_validity () && 255 ) ;                          
                                                
                                                }//\\27
                                                                
                                            if ( RX1_MESSAGE_BUFFER[ FEat +1 ]  == 0xFE ) 
                                              {  //28  is this value  = FE ?
                                                Icom.set_FE2_flag (true) ; 
                                                } //\\28

                                            if ( RX1_MESSAGE_BUFFER[ TOat ]  == 0xE0 ) 
                                              {  //29  is this value  = CONTROLLER ID ?
                                                Icom.set_to_flag (true) ; 
                                                }//\\29


                                            if ( RX1_MESSAGE_BUFFER[ FROMat ]  == Icom.get_hex_id()  ) 
                                              {  //30  is this value  = RADIO ID ?
                                                Icom.set_from_flag (true) ; 
                                                }//\\30


                                            if ( type== 0 )  // reply type 0
                                                {//31
                                                }//\\32

                                            if (type == 1 )  // reply type 1
                                                {//33
                                                  if ( RX1_MESSAGE_BUFFER[ FBat ]  == 0xFB ) 
                                                      {  //34  is this value  = FB
                                                      Icom.set_FB_flag (true) ;
                                                      Icom.set_FA_flag (false) ;  
                                                      }//\\34

                                                  if ( RX1_MESSAGE_BUFFER[ FBat ]  == 0xFA )
                                                            {  //35  is this value  = FA ?
                                                              Icom.set_FB_flag (false) ; 
                                                              Icom.set_FA_flag (true) ; 
                                                              } //\\35

                                                }//\\33

                                              if (type == 2 )  // reply type 2
                                                            {//36
                                                              if ( RX1_MESSAGE_BUFFER[ FEat + 4 ]  == 0xFB )
                                                                  {  //37  is this value  = FB ?
                                                                    
                                                                  }//\\37

                                                                if ( RX1_MESSAGE_BUFFER[ FEat + 4 ]  == 0xFA )
                                                                    {  //38  is this value  = FA ?
                                                    
                                                                    } //\\38

                                                            }//\\36 


                                              if ( RX1_MESSAGE_BUFFER[ FDat ]  == 0xFD ) 
                                                {  // 39 is this value  = FE ?
                                                  Icom.set_FD_flag (true) ; 
                                                  }//\\39
                                   }  //\\25 end of conditional processing of payload


              //  *********************************************************
              //  code ok to here ... TX is now 'one shot' until tx1_inhibit is reset (to false) ... also need to reset 
              //  ( to false ) the 'just sent flag' somewhere ... after verification process
              //  *********************************************************
              //                                                *
              //                                                *
              //                                                *
              //                                           *    *    *
              //                                             *  *  *
              //                                                *
              //                                    ***********************
              //                                       done it here :-)
              //                                    ***********************      

                                                    // wrong logic here ?                  
                                                    
                                                    Arduino.set_CI_V_TX_inhibit_flag(true) ; // stop further tx 
                                                    
                                                    Arduino.set_just_sent_CI_V_message_flag(false) ;  // no longer true buffers now empty 
                                                    
                                                    Arduino.set_listen_for_CI_V_message_flag(true) ;   //  stop listen ... don't transfer any othe 
                                                    //recived bytes into 'our' rx buffer until processing / checking / validation process completed

                                  return   Arduino.get_TX_MESSAGE_NOT_CORRUPT_flag() ;  // change this to an aggregate flag that indicates the data is valid
                                  
      }// 01 end of serial ttl tx function - which SENDS CI-V data to the One Wire Bus  <<<<<<<<---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//         END OF tx serial 1 FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//
// end of tab C (serial via TX1)

// start tab D data conversion

                                                
byte high_nibble_unpacker (byte high_target) {  // unpacks the high nibble by using a bit mask
                                        
                                        byte two_nibbles =0 ;

                                        boolean bits4 =LOW ;
                                        boolean bits5 =LOW ;
                                        boolean bits6 =LOW ;
                                        boolean bits7 =LOW ; 

                                        byte eights = 0 ;
                                        byte fours = 0 ;
                                        byte twos = 0 ;
                                        byte ones = 0 ;
 
                                        byte high_nibble_unpacked_bcd = 0 ;                                                                                                              
                                       
                                       two_nibbles = high_target ; // get hold of the packed BCD byte
                  
                                       //now pass the byte through and read the bits 

   //now pass the byte through and read the bits 
                                       // assumes bigendian
                                       bits7= two_nibbles & 0x80 ;
                                       high_nibble_unpacked_bcd = high_nibble_unpacked_bcd + bits7 ;high_nibble_unpacked_bcd = high_nibble_unpacked_bcd <<1 ; //get bit 7 ?

                                       bits6= two_nibbles & 0x40 ;
                                       high_nibble_unpacked_bcd = high_nibble_unpacked_bcd + bits6 ;high_nibble_unpacked_bcd = high_nibble_unpacked_bcd <<1 ;
                                       
                                       bits5= two_nibbles & 0x20 ;
                                       high_nibble_unpacked_bcd = high_nibble_unpacked_bcd + bits5 ;high_nibble_unpacked_bcd = high_nibble_unpacked_bcd <<1 ;

                                       bits4= two_nibbles & 0x10 ; 
                                       high_nibble_unpacked_bcd = high_nibble_unpacked_bcd + bits4 ;
                                    
                                       return high_nibble_unpacked_bcd ;
                                       }// eof


/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//         END OF HIGH NIBBLE FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//
                  
byte low_nibble_unpacker (byte low_target) { // unpacks the low nibble by using a bit mask

                                        byte two_nibbles =0 ;
                                        
                                        boolean bits0 =LOW ;
                                        boolean bits1 =LOW ;
                                        boolean bits2 =LOW ;
                                        boolean bits3=LOW ;

                                        byte eights = 0 ;
                                        byte fours = 0 ;
                                        byte twos = 0 ;
                                        byte ones = 0 ; 
                                        byte low_nibble_unpacked_bcd = 0 ;                                                                                                              
 
                                        two_nibbles = low_target ;

                                         // assumes bigendian
                                        two_nibbles = low_target ;
                    
                                        bits3= two_nibbles & 0x08 ;
                                        low_nibble_unpacked_bcd =low_nibble_unpacked_bcd + bits3 ;low_nibble_unpacked_bcd = low_nibble_unpacked_bcd <<1 ;
                                      
                                        bits2= two_nibbles & 0x04 ;
                                        low_nibble_unpacked_bcd =low_nibble_unpacked_bcd + bits2 ;low_nibble_unpacked_bcd = low_nibble_unpacked_bcd <<1 ;

                                        bits1= two_nibbles & 0x02;
                                        low_nibble_unpacked_bcd =low_nibble_unpacked_bcd + bits1 ;low_nibble_unpacked_bcd = low_nibble_unpacked_bcd <<1 ;
                                        
                                        bits0= two_nibbles & 0x01;
                                        low_nibble_unpacked_bcd =low_nibble_unpacked_bcd + bits0 ;
                                        
                                        return low_nibble_unpacked_bcd ;

                     } // eofumctiom

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//         END OF BIT LOW NIBBLE FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//


void bit_bash(byte control) {
 // basically this function will unpack BCD bytes... in conjunction with HIGH an LOW nibble unpacker  
                 
                  byte target =0 ;
                  byte local_command  =  control ;     // determins if we are interested in frequ, mode or s-meter
                  byte data_starts_at = ci_v_TX1_RX1_MESSAGEs  [local_command]   [11] ; // gets the start position of REAL DATA indicated by entry in ci_v_RX1_MESSAGEs
                  byte data_ends_at   = ci_v_TX1_RX1_MESSAGEs  [local_command]   [12] ; //  gets the end position of REAL DATA...

                  byte low_nibble_unpacked_bcd = 0 ;
                  byte high_nibble_unpacked_bcd = 0 ;

                  //unsigned long vco_frequency = 0 ;
                  
                  boolean drawDots = false;

                  // clear array 
                  
                   memset(ci_v_rx_DATA, 0, 15); 

                    for ( byte i = data_starts_at ; i <= data_ends_at ; i++ )
                                                                            {
                                                                            /* 
                                                                              if (local_command == 8)  // frequency
                                                                                                                  {
                                                                                                                  band_store[Icom.get_current_band()][i-data_starts_at]=RX1_MESSAGE_BUFFER[ i ] ; // copy frequ data bytes into band store
                                                                                                                  }
                                                                              */


                                                                            target = RX1_MESSAGE_BUFFER[ i ] ;  //         [8] to [12]  gets a packed BYTE from a global array....freq = [11] to [15]
                                                                              
                                                                            high_nibble_unpacked_bcd = high_nibble_unpacker (target) ; // calls unpacker function to extract bcd data from the packed byte and place it in the LOWER nibble of a new unpacked byte called 'high_nibble_unpacked_bcd'
                                                                            
                                                                            low_nibble_unpacked_bcd = low_nibble_unpacker (target) ;   // calls unpacker function to extract bcd data from the packed byte and place it in the LOWER nibble of a new unpacked byte called 'low_nibble_unpacked_bcd'
                                                                           
                                                                            ci_v_rx_DATA[((i-data_starts_at)*2)+0] = high_nibble_unpacked_bcd ; // write to  [   ]  [0] ... 2...4...6...8...
                                                                            ci_v_rx_DATA[((i-data_starts_at)*2)+1] = low_nibble_unpacked_bcd ; // write to  [   ]  [1] ... 3...5...7...9...
                                                            
                                                                            }  // end of for loop

                                        
 /*                                        
                     // *******************************
                     // now loop to clear RX buffer 
                     // ******************************* 


                     for (int i=0 ; i<15 ;  i++) {
                                                 RX1_MESSAGE_BUFFER[i] = 0 ;  // or nul ?... not here need buffer for crash test
                                                 }
*/
                } //eof                                                

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//          BIT BASH FUNCTION calls hihg & low nibble functions ABOVE
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
//

 
 
 unsigned long get_frequency() {  // pass it the command character ... why not just use '8' ..the actual command character

  // puts ascci vaules of the radio's VCO frequency into the data transmission buffer (Y_messageHolder1[ ]) while also re-ordering the bytes into ascending magnitude 10^0 ... 10^9 ... Hz to GHz 
  
                                            boolean drawDots = false;
                                            unsigned long frequency =0 ; 

                                         

                                           frequency = frequency + ci_v_rx_DATA[0]  * 10  ; // 1   VCOF1=34
                                           //Y_messageHolder1[VCOF1] = ci_v_rx_DATA[0]+48; //  48   42     34
                                           frequency = frequency + ci_v_rx_DATA[1]  * 1  ;  // 0   VCOF0=35
                                           //Y_messageHolder1[VCOF0] = ci_v_rx_DATA[1]+48; //  49   43     35
                                           frequency = frequency + ci_v_rx_DATA[2]  * 1000  ;  //3  
                                           //Y_messageHolder1[VCOF3] = ci_v_rx_DATA[2]+48; //  46   40     32
                                           frequency = frequency + ci_v_rx_DATA[3]  * 100  ; //2
                                          // Y_messageHolder1[VCOF2] = ci_v_rx_DATA[3]+48; //   47   41     33
                                           frequency = frequency + ci_v_rx_DATA[4]  * 100000UL  ;//5
                                           //Y_messageHolder1[VCOF5] = ci_v_rx_DATA[4]+48; //   44   38     30
                                           frequency = frequency + ci_v_rx_DATA[5]  * 10000UL  ; //4  // 
                                          // Y_messageHolder1[VCOF4] = ci_v_rx_DATA[5]+48; //   45   39      31
                                           frequency = frequency + ci_v_rx_DATA[6]  * 10000000  ;//7
                                           // Y_messageHolder1[VCOF7] = ci_v_rx_DATA[6]+48; //  42  36      28 
                                           frequency = frequency + ci_v_rx_DATA[7]  * 1000000  ;//6
                                          // Y_messageHolder1[VCOF6] = ci_v_rx_DATA[7]+48; //   43  37      29
                                           frequency = frequency + ci_v_rx_DATA[8]  * 1000000000  ; //9    
                                           // Y_messageHolder1[VCOF9] = ci_v_rx_DATA[8]+48; // 40  34        26
                 
                                           frequency = frequency + ci_v_rx_DATA[9]  * 100000000  ;//8
                                          // Y_messageHolder1[VCOF8] = ci_v_rx_DATA[9]+48; //  41  35        27 

// calculate band 
 
                                          if ((frequency >= LO_160M)  && (frequency <= HI_160M )) //                                         160metre
                                                                                              {
                                                                                                Icom.set_current_band(1) ;
                                                                                              }                                          
                                          if ((frequency >= LO_80M)  && (frequency <= HI_80M )) // 3.400000 - 4.099999 MHz                    80metre
                                                                                              {
                                                                                                Icom.set_current_band(2) ;
                                                                                              }
                                          else if ((frequency >= LO_40M)  && (frequency <= HI_40M )) // 6.900000 - 7.499999 MHz               40metre
                                                                                                    {
                                                                                                      Icom.set_current_band(3) ;
                                                                                                    }
                                          else if ((frequency >= LO_30M ) && (frequency <= HI_30M )) // 13.900000 - 14.499999 MHz             30metre
                                                                                                    {
                                                                                                      Icom.set_current_band(4) ;
                                                                                                    }
                                          else if ((frequency >= LO_20M ) && (frequency <= HI_20M )) //                                       20metre
                                                                                                    {
                                                                                                      Icom.set_current_band(5) ;
                                                                                                    }
                                          else if ((frequency >= LO_17M ) && (frequency <= HI_17M )) // 
                                                                                                    {
                                                                                                      Icom.set_current_band(6) ;
                                                                                                    }
                                          else if ((frequency >= LO_15M ) && (frequency <= HI_15M )) // 
                                                                                                    {
                                                                                                      Icom.set_current_band(7) ;
                                                                                                    }
                                          else if ((frequency >= LO_12M ) && (frequency <= HI_12M )) // 
                                                                                                    {
                                                                                                      Icom.set_current_band(8) ;
                                                                                                    }                                                                                                    
                                          else if ((frequency >= LO_10M ) && (frequency <= HI_10M )) // 
                                                                                                    {
                                                                                                      Icom.set_current_band(9) ;
                                                                                                    }
                                          else if ((frequency >= LO_6M ) && (frequency <= HI_6M )) // 50.000000 - 52.000000 MHz             6metre
                                                                                                    {
                                                                                                      Icom.set_current_band(10) ;
                                                                                                    }
                                          else { Icom.set_current_band( 0 );}  // out of band


// ****************************************************
//         copy frequ data bytes into band store   
// ****************************************************

                         for ( int i = 0 ; i <= 4 ; i++ )
                                                         {
                                                          band_store[Icom.get_current_band()][i]=RX1_MESSAGE_BUFFER[ i+11 ] ; // copy frequ data bytes into band store  11 - 15                                         
                                                         }
                         return frequency ;
                                                               /*
                                                              * Band id's     MY SYMBOL  ( source ICOM )
                                                              *    01            A        1.8MHz  160m
                                                              *    02            B        3.5MHz  80m
                                                              *    03            C        7.0MHz  40m
                                                              *    04            D        10MHz   30m
                                                              *    05            E        14MHz   20m 
                                                              *    06            F        18MHz
                                                              *    07            G        21MHz
                                                              *    08            H        24MHz
                                                              *    09            I        28MHz
                                                              *    10            J        50MHz    6m
                                                              */                                      


                                                              // BAND 

// ***** Frequency (Hz) to Band ******************************

 /* 
  * Band id's     MY SYMBOL  ( source ICOM )
  *    01            A        1.8MHz  160m
  *    02            B        3.5MHz  80m
  *    03            C        7.0MHz  40m
  *    04            D        10MHz   30m
  *    05            E        14MHz   20m 
  *    06            F        18MHz
  *    07            G        21MHz
  *    08            H        24MHz
  *    09            I        28MHz
  *    10            J        50MHz    6m                                    


          
       if (freq >=   LO_160M && freq <=   HI_160M )  {BAND=1;}  // 160m
  else if (freq >=   LO_80M  && freq <=    HI_80M )  {BAND=2;}  //  80m
  else if (freq >=   LO_40M  && freq <=    HI_40M )  {BAND=3;}  //  40m
  else if (freq >=   LO_30M  && freq <=    HI_30M )  {BAND=4;}  //  30m
  else if (freq >=   LO_20M  && freq <=    HI_20M )  {BAND=5;}  //  20m
  else if (freq >=   LO_17M  && freq <=    HI_17M )  {BAND=6;}  //  17m
  else if (freq >=   LO_15M  && freq <=    HI_15M )  {BAND=7;}  //  15m
  else if (freq >=   LO_12M  && freq <=    HI_12M )  {BAND=8;}  //  12m
  else if (freq >=   LO_10M  && freq <=    HI_10M )  {BAND=9;}  //  10m
  else if (freq >=   LO_6M   && freq <=    HI_6M )   {BAND=10;} //   6m

  else {BAND=0;}   // out of band

  
#define LO_160M  1810000       //   BAND=1;        
#define HI_160M  2000000                              

#define LO_80M  3500000       //    BAND=2      
#define HI_80M  3800000

#define LO_40M  7000000        //   BAND=3      
#define HI_40M  7200000

#define LO_30M  10100000        //  BAND=4       
#define HI_30M  10150000

#define LO_20M  14000000       //   BAND=5      
#define HI_20M  14350000                 

#define LO_17M  18068000       //   BAND=6      
#define HI_17M  18168000 

#define LO_15M  21000000       //   BAND=7      
#define HI_15M  21450000 

#define LO_12M  24890000       //   BAND=8       
#define HI_12M  24990000 

#define LO_10M  28000000       //   BAND=9      
#define HI_10M  29700000 

#define LO_6M   50000000       //  BAND=10       
#define HI_6M   52000000 
 */
  
//  ***********************************


                                       }               
     
/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//         END OF BIT FREQUENCY FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

// end of tab D data conv.


// start of tab E & F isnt any !
// end of E&F

// start of tab IJKLM there isnt any
// end of tab JKLM

// start of tab N new logic
void Controller_mode_Logic () {

unsigned long freq = Icom.get_current_frequency() ;
byte band   = Icom.get_current_band();


                               /*
                               Steady_ON_LEDS( mode_2_token( Icom.get_current_rx_mode() )  ) ; // mode2token returns a 'token' 
                               Steady_OFF_LEDS(mode_2_token( Icom.get_current_rx_mode() ) )  ; 

                                // update mode illuminated button LED 
                                if (  mode_2_token( Icom.get_current_rx_mode() )  == LOCAL_KBD.get_requested_rx_mode_token() )
                                                                                                                                    {
                                                                                                                                    Steady_ON_LEDS(mode_2_token( Icom.get_current_rx_mode() ) )  ;
                                                                                                                                    cancel_flash_Button_LEDs (mode_2_token( Icom.get_current_rx_mode() ) )  ;
                                                                                                                                    Steady_OFF_LEDS(mode_2_token( Icom.get_current_rx_mode() ) )  ;
  
                                                                                                                                    }
*/
                                                                                                                                    
 if (LOCAL_KBD.get_button_pressed_NOW_TOKEN() == '`' )  //only do mode change request AFTER button release
                                               {
                              
                                                 if ( LOCAL_KBD.get_requested_rx_mode_token() != 
                                                 mode_2_token ( Icom.get_current_rx_mode() )) 
                                                                                                  {
                                                                                                
                                                                                                   
                                                                                                   switch (LOCAL_KBD.get_requested_rx_mode_token() ) // initiates a CI-V message to the radio
                                                                                                                                                      // look at first tab ... ci_v_TX1_RX1_MESSAGEs[12][6] structure 
                                                                          
                                                                                                   
                                                                                                                                                {// sw 1
                                                                                                                                                  case 'c' :
                                                                                                                                                 
                                                                                                                                                  tx_ci_v_via_serial_ttl(3) ; // request AM   // need some tx control 
                                                                                                                                                  LOCAL_KBD.set_requested_rx_mode_token('`');
                                                                                                                                                  break ;
                                                                                                
                                                                                                                                                  case 'b' :
                                                                                                                                                                                                                                                                                                   tx_ci_v_via_serial_ttl(2) ; // request USB // need some tx control 
                                                                                                                                                  LOCAL_KBD.set_requested_rx_mode_token('`');
                                                                                                                                                  break ;
                                                                                                
                                                                                                                                                  case 'a' :
                                                                                                                                                  
                                                                                                                                                  tx_ci_v_via_serial_ttl(1) ; // request LSB // need some tx control 
                                                                                                                                                  LOCAL_KBD.set_requested_rx_mode_token('`');
                                                                                                                                                  break ;
                                                                                                
                                                                                                                                                  case 'f' :
                                                                                                                                                 
                                                                                                                                                  tx_ci_v_via_serial_ttl(6) ; // request FM // need some tx control 
                                                                                                                                                  LOCAL_KBD.set_requested_rx_mode_token('`');
                                                                                                                                                  break ;
                                                                                                
                                                                                                                                                  default : 
                                                                                                
                                                                                                                                                  break ;
                                                                                                
                                                                                                                                                } // eos 1

                                                                                                   } 
                                                              
                                                }                                                                
                          
                      } // end of function

// end of tab N

// start of tab O time sync
byte initialise_time() {

                        ci_v_TX1_RX1_MESSAGEs[23][6]=Arduino.get_yearHighz() ;
                        ci_v_TX1_RX1_MESSAGEs[23][7]=Arduino.get_yearLowz() ;
                        ci_v_TX1_RX1_MESSAGEs[23][8]=Arduino.get_monthz() ;
                        ci_v_TX1_RX1_MESSAGEs[23][9]=Arduino.get_dayz() ;
                        
                         delay (200) ;
                                          //GreenLED.off() ;
                                          Arduino.set_CI_V_TX_inhibit_flag(false) ; // ..... TRANSMIT INHIBIT DISABLED 
                                          delay (100) ;
                                          //GreenLED.on() ;
                                          tx_ci_v_via_serial_ttl(23) ; // send CI-V message to set date
                                          delay (100) ; 
                                         // GreenLED.off() ;      
                                          Arduino.set_CI_V_TX_inhibit_flag(true) ; // ..... TRANSMIT INHIBIT ENABLED
                                          delay (100) ;

                        ci_v_TX1_RX1_MESSAGEs[24][6]=Arduino.get_hourz() ;
                        ci_v_TX1_RX1_MESSAGEs[24][7]=Arduino.get_minz() ;
                        
                         delay (200) ;
                                          //RedLED.off() ; 
                                          Arduino.set_CI_V_TX_inhibit_flag(false) ; // ..... TRANSMIT INHIBIT DISABLED 
                                          delay (100) ;
                                          //RedLED.on() ;
                                          tx_ci_v_via_serial_ttl(24) ; // send CI-V message to set time
                                          delay (100) ; 
                                          // RedLED.off() ;      
                                          Arduino.set_CI_V_TX_inhibit_flag(true) ; // ..... TRANSMIT INHIBIT ENABLED
                                          delay (100) ;
                                         // RedLED.on() ;

                                           //  *********************************************************
                                           //  
                                           //  *********************************************************
                                                   
                        }


bool test_time()
                {
                  if( ( Arduino.get_yearHighz() != Icom.get_yearHighz() )  || (Arduino.get_yearLowz() != Icom.get_yearLowz() )  || 
                  (Arduino.get_monthz() != Icom.get_monthz() )  ||    (Arduino.get_dayz() != Icom.get_dayz() )  ||  
                  (Arduino.get_hourz() != Icom.get_hourz() )   ||  (  Arduino.get_minz() != Icom.get_minz() )  )
                                                                                                                                            {
                                                                                                                                            /* 

                                                                                                                                            RedLED.off() ;
                                                                                                                                              
                                                                                                                                             delay (100) ;
                                                                                                                                              RedLED.on() ;
                                                                                                                                              
                                                                                                                                             delay (200) ; 
                                                                                                                                             RedLED.off() ;      
                                                                                                                                              
                                                                                                                                             delay (100) ;
                                                                                                                                             RedLED.on() ;

                                                                                                                                             delay (200) ;

                                                                                                                                              RedLED.off() ;
                                                                                                                                              
                                                                                                                                              delay (100) ;
                                                                                                                                             RedLED.on() ;
                                                                                                                                              
                                                                                                                                              delay (200) ; 
                                                                                                                                             RedLED.off() ;      
                                                                                                                                              
                                                                                                                                              delay (100) ;
                                                                                                                                              RedLED.on() ;

                                                                                                                                              delay (200) ;

                                                                                                                                            */
                                                                                                                                              initialise_time() ;

                                                                                                                                              return true ;
                                                                                                                                            }
                else 
                {
                 return false ; 
                }
 
                }

/*
 uint8_t HexToBCD(uint8_t number)
 {
    unsigned char i=0; // will count the 4 bit groups
    unsigned int k = 0; // the final result to be built up
    // while the number still has digits to go..
    while(number)
    {
        // shift the bottom digit into the right 4 bit group
        k = ( k ) | ((number%10) << i*4);
        // lose the lower digit
        number = number / 10;
        // move to the next 4 bit group
        i++;
    }
    return(k);
 }
 */
// end of tab O

// start of tab X parser remote control
byte remote_control ( byte incomingUSByte ) // now returns ZERO if out of acceptable range or index 1 - 18 
                                        {
                                        byte index = 0 ;  // the default return value

                                         // could try initialising an array of the messages, then returning an index to the array to main loop to then
                                         // do the tx functon by reading the elements from the array
                                            
                    // LOWERCASE a         1.8MHz 160m band
                    
                                           if (incomingUSByte == 97) {// 
                                                              //     BlueLED.on();
                                                                   index = 1 ;                         
                                                                   // 
                                                                   //        
                                                                   } //end if 97
                    
                    // LOWERCASE  b       3.5MHz 80m band 
                    
                                          if (incomingUSByte == 98) {// 
                                                                   // BlueLED.on();         
                                                                   index = 2 ;
                                                                   // 
                                                                   //       
                                                                    } //end if 98
                                                                                                                
                   // LOWERCASE  c        7.0MHz 40m band
                
                                          if (incomingUSByte == 99) {// 
                                                                    
                                                                //   BlueLED.on();
                                                                   index = 3 ;
                                                                   // 
                                                                   //
                                                                   } //end if 99
                                     
                   // LOWERCASE  d        10.0MHz 30m band
                
                                          if (incomingUSByte == 100) {// 
                                                                   // BlueLED.on(); 
                                                                   index = 4 ;
                                                                   // 
                                                                   //
                                                                   } //end if 100

                   // LOWERCASE  e        14.0MHz 20m band
                
                                          if (incomingUSByte == 101) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 5 ;
                                                                   // 
                                                                   //
                                                                   } //end if 101

                   // LOWERCASE  f        18.0MHz 15m band
                
                                          if (incomingUSByte == 102) {  // 
                                                                   // BlueLED.on();   
                                                                   index = 6 ;
                                                                   // 
                                                                   //
                                                                   } //end if 102

                   // LOWERCASE  g        21.0MHz  --m band
                
                                          if (incomingUSByte == 103) {  // 
                                                                   // BlueLED.on();  
                                                                   index = 7 ;
                                                                   // 
                                                                   //
                                                                   } //end if 103

                   // LOWERCASE  h        24.0MHz  --m band
                
                                          if (incomingUSByte == 104) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 8 ;
                                                                   
                                                                   //
                                                                   } //end if 104  

                   // LOWERCASE  i        28.0MHz  --m band
                
                                          if (incomingUSByte == 105) {  // 
                                                                   // BlueLED.on();    
                                                                   index = 9 ;
                                                                   // 
                                                                   //
                                                                   } //end if 105
// j .... 106
// k .... 107                                                           

                   // LOWERCASE  l        50.0MHz  --m band
                
                                          if (incomingUSByte == 108) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 10 ;
                                                                   // 
                                                                   //
                                                                   } //end if 108
                   // LOWERCASE  m        mode CW
                
                                          if (incomingUSByte == 109) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 11 ;
                                                                   // 
                                                                   //
                                                                   } //end if 109

                   // LOWERCASE  n        mode AM
                
                                          if (incomingUSByte == 110) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 12 ;
                                                                   // 
                                                                   //
                                                                   } //end if 110

                   // LOWERCASE  o        mode LSB
                
                                          if (incomingUSByte == 111) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 13 ;
                                                                   // 
                                                                   // 
                                                                   } //end if 111
                                              
                   // LOWERCASE  p        mode USB
                
                                          if (incomingUSByte == 112) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 14 ;
                                                                   // 
                                                                   //
                                                                   } //end if 112

                   // LOWERCASE  q        mode FM
                
                                          if (incomingUSByte == 113) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 15 ;
                                                                   // 
                                                                   //
                                                                   } //end if 113

                   // LOWERCASE  r        record start
                
                                          if (incomingUSByte == 114) {  // 
                                                                   // BlueLED.on();    
                                                                   index = 16 ;
                                                                   // 
                                                                   //
                                                                   } //end if 114

                   // LOWERCASE  s        record stop
                
                                          if (incomingUSByte == 115) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 17 ;
                                                                   // 
                                                                   //
                                                                   } //end if 115

                   // LOWERCASE  x        encoder dial lock
                
                                          if (incomingUSByte == 120) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 18 ;
                                                                   // 
                                                                   //
                                                                   } //end if 116  ....disable the rotary encoder.... might use upper case too to unlock

                   // LOWERCASE  q        test message
                
                                          if (incomingUSByte == 117) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 19 ;
                                                                   // 
                                                                   //
                                                                   } //end if 115


                   // UPPERCASE  t        what frequency
                
                                          if (incomingUSByte == 116) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 20 ;
                                                                   // 
                                                                   //
                                                                   } //end if 115

                   // UPPERCASE  T        what mode
                
                                          if (incomingUSByte == 84) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 21 ;
                                                                   // 
                                                                   //
                                                                   } //end if 115



                   // UPPERCASE  S        get S meter value
                
                                          if (incomingUSByte == 83) {  // 
                                                                   // BlueLED.on();     
                                                                   index = 22 ;
                                                                   // 
                                                                   //
                                                                   } //end if 115

                  // NUBMER 0   
                                       if (incomingUSByte == 48) {  // number '0' in ascii  
                                                                index = 23 ;  

                                                                //   
                                                               }  // end if 48                
                 
             
                 // NUBMER 1   
                                       if (incomingUSByte == 49) {  // number '1' in ascii  
                                                                index = 24 ;  

                                                                //   
                                                               }  // end if 49
                                              
                 // NUBMER 2   
                                       if (incomingUSByte == 50) {  // number '2' in ascii  
                                                                index = 25 ;
                                    
                                                                //
                                                               }  // end if 50
                                                               
                                                                                                     
                // NUBMER 3   
                                       if (incomingUSByte == 51) {  // number '3' in ascii  
                                                                   index = 26 ;
 
                                                                   //
                                                               }  // end if 51
                                                                                 
                                                                                   
                                                                                                                 
                // NUBMER 4   
                                       if (incomingUSByte == 52) {  // number '4' in ascii  
                                                                   index = 27 ;

                                                                 //
                                                               }  // end if 54


              return index ;
     
              } // end of Remote Control function

void clear_buffers() 
                    
                  
                    {

                    }
// end of tab X 

          
/*
void LED_test_white(byte led)
                    {
                    byte index = led ;
                    
                    // byte led_pwm = shine ;
                     switch ( index ) {
                                       case 0 :  // s0
                                       // Zeroth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B01111111); // 1st red   off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B01111111); // 1st green on..........on
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B01111111); // 1st blue.....off 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 1 :  // s1
                                       // first LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B10111111); // 1st red....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B10111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B10111111); // 1st blue .....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 2
                                       case 2 :  // s2
                                       // second LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11011111); // 1st red....off
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11011111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11011111); // 1st blue.....off 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 1
                                       case 3 :  // s3
                                       // Third LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11101111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11101111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11101111); // 1st blue.....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 3   
                                       case 4 :  // s4
                                       // Fifth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11110111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11110111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11110111); // 1st blue....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 4       
                                        case 5 :  // s5
                                       // Sixth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111011); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111011); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111011); // 1st blue.....blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 5       
                                        case 6 :  // s6
                                       // Seventh LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111101); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111101); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111101); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 6       
                                        case 7 :  // s7
                                       // Eigth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111110); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111110); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111110); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                        break ; //  end of case 7       
                                        case 8 :  // s8
                                       // Nineth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B01111111); // 2nd blue
                                       break ; //  end of case 8       
                                       case 9 :  // s+9
                                       // Tenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B10111111); // 2nd blue
                                        break ; //  end of case 9       
                                        case 10 :  // s+10
                                       // Eleventh LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11011111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11011111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11011111); // 2nd blue
                                        break ; //  end of case 10     
                                        case 11 :  // s+20
                                       // Twelth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11101111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11101111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11101111); // 2nd blue
                                        break ; //  end of case 20                    
                                        case 12 :  // s+30
                                       // Thirteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11110111); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111011); // 2nd blue
                                        break ; //  end of case 30                    
                                        case 13 :  // s+40
                                       // Fourteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111011); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111011); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111111); // 2nd blue
                                       break ; //  end of case 40                      
                                       case 14 :  // s+50
                                       // Fifteenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111101); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111101); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111101); // 2nd blue
                                        break ; //  end of case 50                        
                                        case 15 :  // s+60
                                       // sixtenth LED only WHITE
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st green 
                                                      shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111); // 1st blue
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111110); // 2nd red
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111110); // 2nd green
                                                      shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, B11111110); // 2nd blue
                                        break ; //  end of case 60                        
                                        } // end of switch
                    } // end of function 
*/
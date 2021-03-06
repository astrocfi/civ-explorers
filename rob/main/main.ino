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
#include "civ_controller.h"
#include "radio.h"
#include "led_arc_board.h"
#include "s_meter.h"
#include "util.h"

#include <Arduino.h>
#include <SoftwareSerial.h>

CIVController CIV(0);
Radio Icom(RADIO);
LEDArcBoard LEDArc(0);

#define COMMUTATOR 5
byte commutator = 0 ;

void setup()
{
#ifdef MEGA2560
  GreenLED.begin();
  BlueLED.begin();
  RedLED.begin();
  Yellow1LED.begin();
  Yellow2LED.begin();
  WhiteLED.begin();
#endif

  util_init();
  CIV.begin();
  Icom.begin();
  LEDArc.begin() ;

#ifdef MEGA2560
  Serial.begin(57600);    //Start the serial connection with the laptop
  Serial1.begin(19200);    //Start the serial1 connection with the radio to tx and rx data via CI-V interface
  Serial3.begin(9600);    //Start the serial3 connection with the controller / REMOTE_KBD
#endif
#ifdef PRO_MINI
  Serial.begin(19200);
#endif

#ifdef PRO_MINI
  host_println(REV_STRING);
#endif
#ifdef MEGA2560
  Serial.println(REV_STRING);
#endif

#define DEBUG_LED_TEST_2
#ifdef DEBUG_LED_TEST_1
  unsigned long cur_time;
  for (;;) {
    LEDArc.set_led_brightness(240);
    for (int i=0; i<=16; i++) {
      for (int j=3; j>=0; j--) {
        if (i+j > 16) {
          continue;
        }
        LEDArc.set_current_val(i+j);
        cur_time = millis();
        while (millis() < cur_time+200)
          LEDArc.update_leds();
      }
    }
    cur_time = millis();
    while (millis() < cur_time+1000)
      LEDArc.update_leds();
    LEDArc.set_led_brightness(250);
    for (int i=15; i>=0; i--) {
      LEDArc.set_current_val(i);
      LEDArc.update_leds();
      cur_time = millis();
      while (millis() < cur_time+200)
        LEDArc.update_leds();
    }
    while (millis() < cur_time+1000)
      LEDArc.update_leds();
  }
#endif /* DEBUG_LED_TEST_1 */

#ifdef DEBUG_LED_TEST_2
  unsigned long cur_time;
  LEDArc.set_led_brightness(240);
  for (;;) {
    LEDArc.set_current_val(0);
    cur_time = millis();
    while (millis() < cur_time+5000)
      LEDArc.update_leds();
    LEDArc.set_current_val(16);
    cur_time = millis();
    while (millis() < cur_time+5000)
      LEDArc.update_leds();
    LEDArc.set_current_val(12);
    cur_time = millis();
    while (millis() < cur_time+5000)
      LEDArc.update_leds();
    LEDArc.set_current_val(6);
    cur_time = millis();
    while (millis() < cur_time+5000)
      LEDArc.update_leds();
  }
#endif /* DEBUG_LED_TEST_2

#ifdef MEGA2560
  WhiteLED.on();delay(140) ; WhiteLED.off();delay(10) ;
  Yellow1LED.on();delay(140) ;Yellow1LED.off(); delay(10) ;
  Yellow2LED.on(); delay(140) ; Yellow2LED.off(); delay(10) ;
  GreenLED.on();  delay(140) ;  GreenLED.off();delay(10) ;
  BlueLED.on();delay(140) ;  BlueLED.off();delay(10) ;
  RedLED.on(); delay(140) ; RedLED.off();delay(10) ;
#endif

CIV.set_listen_for_CI_V_message_flag(false) ;            // init sets to false ...
CIV.set_just_sent_CI_V_message_flag(false) ;           // init sets to false ...
CIV.set_CI_V_TX_inhibit_flag(true) ;                 // init sets to true ...
CIV.set_TX_MESSAGE_NOT_CORRUPT_flag(false) ;  // init sets to false ...
CIV.set_collision_flag(false) ;                  // init sets to false ...
CIV.set_tx_jam_code_flag(false) ;              // init sets to false ...
CIV.set_rx_jam_code_flag(false) ;            // init sets to false ...


int flush_count ;

//..........................................

 #ifdef PRO_MINI
                while (Serial.available() > 0) // while bytes from CI-V intrefaceare waiting to be read do it
                      {
                      Serial.read();  // get first or next byte
                      flush_count++ ; // count bytes
                      if ( flush_count >= 1046 ) { break ; } // bug out  1046
                      }

                host_print ( "flush count : ") ;
                host_println (flush_count);

//..........................................

                host_print ( "flush 1 : ") ;
                host_print (RX1_COUNTER = Serial.available());

                Serial.read(); // empty RX hardware buffer now
                delay(100) ;
                host_print ( "flush 2 : ") ;
                host_print (RX1_COUNTER = Serial.available());

                /*
                RX1_COUNTER = Serial.available();
                Serial.read(); // empty RX hardware buffer now
                delay(100) ;
                */
// #endif

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

    #ifdef MEGA2560
                  GreenLED.on() ;
                  RedLED.on() ;
                  WhiteLED.on() ;
                  delay (200) ;
   #endif

                  CIV.set_CI_V_TX_inhibit_flag(false) ; // ..... TRANSMIT INHIBIT DISABLED
                  delay (100) ;

                  tx_ci_v_via_serial_ttl(21) ; // send CI-V message to request date
                  delay (100) ;
                  CIV.set_CI_V_TX_inhibit_flag(true) ; // ..... TRANSMIT INHIBIT ENABLED
                  delay (100) ;
                  bit_bash(21) ;  // sort out the nibbles
                  delay (100) ;

//   **********************************************************************

//    check is radio connected ?   year or month or day shouuld no be zero

//   **********************************************************************

// if (   Icom.get_dayz() == 0 )  // also use .get_dayz to toggle pollling
//
//                                 {
//                                  #ifdef MEGA2560
//                                  Serial.print ("dayz = " );
//                                  Serial.print (Icom.get_dayz(),DEC );
//                                  Serial.println ("    no radio connected      ");
//                                  #endif
//
//                                   #ifdef PRO_MINI
//                                   host_print ("dayz = " );
//                                   host_print (Icom.get_dayz(),DEC);
//                                   host_print ( "   no radio connected    ") ;
//                                   #endif
//
//                                   Icom.set_RADIO_connected(false)  ;
//                                 }
//
//                                 else
//                                 {
//                                   #ifdef MEGA2560
//                                   Serial.print ("dayz = " );
//                                   Serial.print (Icom.get_dayz(), HEX );
//                                   Serial.println ("    radio connected      ");
//                                   #endif
//
//                                   #ifdef PRO_MINI
//                                   host_print ("dayz = " );
//                                   host_print (Icom.get_dayz(),HEX);
//                                   host_print ( "    radio connected    ") ;
//                                   #endif
//                                   Icom.set_RADIO_connected(true)  ;
//
//                                 }
//
//
// memset(ci_v_rx_DATA, 0, 15);    // clear the array
// memset(RX1_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);// clear the array
// memset(TX1_MESSAGE_BUFFER, 0, 20);// clear the array
//
// delay (200) ;

// memset(RX1_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);// clear the array
// memset(TX1_MESSAGE_BUFFER, 0, 20);// clear the array


} // end of set up



void loop() {
  unsigned char tm_buttons;
  byte button_scan_index = 0;
  boolean valid_data_flag = false;
  unsigned long vco_frequency = 0;
  int mean_average;
#ifdef MEGA2560
  WhiteLED.on();
 #endif
  delay(1);
#ifdef MEGA2560
  WhiteLED.off();
#endif

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++     LOOP DELAY    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  startMillis = millis();

  delay(LOOP_DELAY);

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++     MACHINE MODE switch    ++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  switch (CIV.get_controller_operating_mode())
  {
  case 0:              //   MACHINE FAULT mode
    break;

  case 1:              //    MACHINE mode : get NTP
    delay(400);
    CIV.set_controller_operating_mode(4);            // exit this state on next loop
    break;

  case 4:              //    MACHINE mode :  CI-V / Ehternet Bridge
    delay(400);
    CIV.set_controller_operating_mode(6);            // exit this state on next loop
    break;

  case 6:             // MACHINE DATA mode  ...  stays in '6' after initialisation unless fault detected


    RX1_COUNTER = listen_TTL_Serial_port ();                       // 2020JAN12: 0k : listening to CI-V circuit for input (or loop back) on serial1 TTL port  (2560 pin 19)
                                     // returns the number of bytes in the from the default arduino 64 byte buffer
                                     // DOESN'T remove any bytes from the default arduino 64 byte buffer
                                     // i.e. if line was quiet, RX1_COUNTER = 0
    if (RX1_COUNTER >= 1)
    {                         // busy line .... take FAIL path to the left
      delay(5);
    }

    // *******************************************************************************************************************************
    //  PRE CURSOR to conditional TRANSMIT ... if permitted   .. will tx on CI-V one wire bus via hardware interface to TTL TX (pin 18)
    // *******************************************************************************************************************************
    //Icom.set_RADIO_connected(true) ;  // temp force polling on
    // ....check THREE CONDITIONS ... FIRST nothing recently sent ...AND (logical AND) ...
    //     SECOND nothing recently received ..i.e. if no bytes in buffer ... 3rd only do it when CI-V holdoff is expired

    if (!CIV.get_just_sent_CI_V_message_flag()
        && RX1_COUNTER == 0
        && !CIV.get_CI_V_TX_HOLDOFF_flag()                 // polling radio and S-meter update is stopped by the holdoff flag too
        && Icom.get_RADIO_connected())                 // if radio not connected dayz will be zero therefore inhibit polling
      {
      //                           |
      //                           |
      //                           |
      //                       <-----------false     |       true---------->
      //                           V
      //            <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< UNSOLICITED  <<<<<<<<<<<<<      >>>>>> SOLICITED >>>>>     ( nothing in the RX buffer, and I haven't recently sent a data request to the radio
      //           |                      |    ( most often down this rhs route ... i.e line is mostly quiet with few unsolicited messages )
      //           |                      |    ( unless you turn the VCO knob ! .. in which case a flood of unso;icited data arrives )
      //           V                      V
      //           V                      V
      //                   DOWN through here if ALL tests passed : this controller CAN send s CI-V request for data
      // i.e. RX1_COUNTER is NOT zero etc
      //    down HERE if all 3 tests failed !              DOWN this route mostly !!


      //  down here if 3 tests failed             //********************************************************************************************************************************
      // new logic here ... while VCO is not rotated i.e. while no unsloicited messages are received
      // allow the polling of the radio ( get VCO, get MODE, get S-METER )  and allow reception of NETWORK packets
      //
      // But when VCO rotated and many unsolicited CI-V messages are received STOP polling the radio and STOP listening for NETWORK packets


      CIV.set_CI_V_TX_inhibit_flag(false);                       // ..... TRANSMIT INHIBIT DISABLED .... 9 dec temporary rem out and copy to else below to turn of polling while vco rotated
      //                  CAN NOW TRANMIT CI-V MESSAGE ...  inhibit is DISABLED .... i.e. false !
      //********************************************************************************************************************************


//  down here if 3 tests failed and this will
//  fail too as inhib should be set after last TX            //  DOWN through here if ALL tests passed

//         ||                  ||
//         ||                  ||
//       \ || /                    \ || /
//         \/                  \/
//  down here if 3 tests plus 1 failed
      //                *****************************************************************************
      //                       enable CI-V transmission of polling messages
      //                *****************************************************************************


      //                ***********************************************************************************
      //                  and rotate through Request VCO(2) , Request S-meter(1,3,5), Request MODE(4) , Request S-meter
      //                ***********************************************************************************

      switch (commutator)
      {                        // OPEN CASES

      case 2:                    // request VCO

        valid_data_flag  = tx_ci_v_via_serial_ttl(8);                     // send ci-v tx !

        if(valid_data_flag== true) {

          bit_bash(8);                              // sort out the nibbles

          // Icom.set_current_frequency( get_frequency() );                              //  THE RADIO's CURRENT FREQUENCY SAVED IT IN OBJECT (RADIO)

          // *******************************
          // now loop to clear RX buffer
          // *******************************

          memset(ci_v_rx_DATA, 0, 15);


        }
        break;                  // end case 2


      case 4:                    // request mode

        valid_data_flag  = tx_ci_v_via_serial_ttl(9);                        // ci-v tx !

        if(valid_data_flag== true) {

          bit_bash(9);                             // sort out the nibbles
          Icom.set_current_rx_mode(ci_v_rx_DATA[1]);                            //
          // Buttons.set_current_rx_mode_token( mode_2_token (  ci_v_rx_DATA[1]  ) );

          // *******************************************************************************
          // ***************************    GET RADIO MODE   *******************************
          // *******************************************************************************


          //  ... use mode data from radio

          // request_mode();                       // obtains the current mode from the radio ... and then drives the approriate button led

          // *******************************
          // now loop to clear RX buffer
          // *******************************


          memset(ci_v_rx_DATA, 0, sizeof(ci_v_rx_DATA));

        }
        break;                    // end case 4

      // *******************************************************************************
      // *********************    WRITE TO LED S METER DISPLAY @ ***********************
      // *******************************************************************************

      case ( 1 ):                     // do this for 3 alternate loops request S meter data

        request_s_meter ();
        S_meter2LED_index();                        // @ updates LED S-meter

        break;                    // end case 1

      case ( 3 ):                     // do this for 3 alternate loops request S meter data

        request_s_meter ();
        S_meter2LED_index();                        // @ updates LED S-meter

        break;                     // end case 3


      }                      // CLOSES CASES    end of switch


      commutator++;                    //
      if (commutator >= COMMUTATOR)
      {
        commutator = 0;
      }


//     down here
//        ||
//        \/

//     down here
//        ||
//        \/


    }                           // end of 3 condition test :  THIS SHOULD BE CLOSE BRACKET 01

    else {


      delay (CIV_RX_SOLICITED_BUFFER_FILL_DELAY);
      // hopefully to preserve timing when polling is disabled by dayz = 0


    }

    // *********************************************************
    // RETURNS TO HERE AFTER CI-V data Transmitted
    // *********************************************************

    // ******************************************************************************************************************
    // now check LOGIC of mode switching in TAB N
    // ******************************************************************************************************************

    // Controller_mode_Logic ();  // needs work in new logic ..see tab N .. new logic

    // ******************************************************************************************************************
    // *********************      ANYTHING  BELOW HERE IS NOT THE DESIRED PATH       ******************************
    // ******************************************************************************************************************

    // ******************************************************************************************************************
    // we know TX bytes will NOW  be in Rx buffer...and also REPLY BYTES TOO unless radio is off or CI-V link is broken
    // ******************************************************************************************************************

// down here if 3 tests plus 1 failed------------>


    //CIV.set_listen_for_CI_V_message_flag(true)... use his to turn on unsoloiced listening

    if (( CIV.get_just_sent_CI_V_message_flag() == false ) &&    // .....NOTHING recently sent ..AND
        (RX1_COUNTER!= 0 )) // .....................................         SOMETHING has recently been received ......this is UN-SOLICITED DATA - MAYBE A BROADCAST MESSAGE - check 'from id' ?
    {          //  unsolicited message processing
               //  ***************************************************************************
               //  ***************************************************************************
               //  NETWORK LISTEN INHIBIT ENABLED ... cannot now listen for TIMETOLIVE loops counted down in the SOLICITED path with the .get command

      // ************************************************************************************************************
      // break; //switch_off during debug
      delay(CIV_RX_UNSOLICITED_BUFFER_FILL_DELAY);    // was 125ms

      CIV.set_CI_V_TX_HOLDOFF_counter(UNSOLICITED_HOLD_OFF);    // stop polling
      // stop scanning buttons
      // stop updating the S-meter LEDs

      ci_v_ring_Buffer.clear();

      byte udp_count = 0;
      CIV.read_unsolicited_message();    // empty h/w buffer into rx_buffer and return
      // number of bytes... this needs a specific routine to handle unsolicited data rather than just flushing it ! ... WRITE NEW FUNCTION !

      // *********************************************************
      //    send udp packet !
      // *********************************************************

      RX1_COUNTER = 0;

      // *******************************
      // now loop to clear RX buffer
      // *******************************
      // set all bytes in the buffer to 0
      // memset(packetBuffer, 0, NTP_PACKET_SIZE);



    }                  // 2020jan09 or use 'listen false' flag (above in line 206)
    //to allow us to just dump this received unsolicited (perhaps broadcast) message




    // ************************************************************************************************************************************
    //      at this point we may have 'looped sent data' AND 'reply data' in the buffer ... the length will depend on the message type ... which we know !
    // ************************************************************************************************************************************
    //    this SHOULDNT HAPPEN ?



    if (( CIV.get_just_sent_CI_V_message_flag() == true ) &&          //...SOMEthing recently sent ..AND
        (RX1_COUNTER >= 1 ))           // SOMEthing recently received ..i.e. there are bytes in the rx buffer
                     //... should be both the TX bytes and (by now the Reply) RX bytes this is probably SOLICITED DATA
    {
      // TAB_R();
    }

    // if (address check ok )
    //   TAB T

    // *********************************************************
    //       NOW check TO / FROM addresses
    // *********************************************************

    if (CIV.get_collision_flag() == false) {

      //TAB_S
    }

    // *********************************************************
    //       send it to PROCESING via usb serial
    // *********************************************************

    //SEND SERIAL packets

    // send_usb_serial(); // this outputs serial data for the PROCESSING s/w running on other PC / MAC / LAPTOP

    //SEND NETWORK packets


    // *********************************************************
    //       collision occured ?
    //  *********************************************************

    if (CIV.get_collision_flag() == true) {

      //TAB_U
    }

    break;             // end of case 6 MACHINE MODE


    // *********************************************************
    //      end of mode 6
    // *********************************************************


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++  End of MACHINE MODE switch   +++++++++++++++++++++++++++++++++++++++++++++
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  }              // end of switch MACHINE controller operating mode

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++  End of MACHINE MODE switch   +++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


  CIV.set_collision_flag(false);     // reset it .. why here

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++     COMMUTATOR SWITCH    ++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  /*
           switch (commutator)
             {
     // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
               case 0:

               break;
     // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
               case 1:


               break;
     // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
               case 2 :


               break;
     // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
               case 3 :


               break;
     // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

             } // end of switch (commutator)
   */

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // +++++++++++++++++++++++++++++++++++++++  End of COMMUTATOR SWITCH   ++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




  stopMillis=millis();

  diffMillis =stopMillis - startMillis;



}     // end of MAIN LOOP    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++++++++++++++++    end of main loop  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

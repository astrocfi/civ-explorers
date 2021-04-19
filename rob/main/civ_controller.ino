/*******************************************************************************
 * Author: Robert S. French <rfrench@rfrench.org>
 * Derived from work by John Rowling
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative
 * Commons, PO Box 1866, Mountain View, CA 94042, USA.
 ******************************************************************************/

#include "civ_controller.h"
#include "util.h"

CIVController::CIVController(byte id)
{
  _id = id;
}

void CIVController::begin()
{
  _controller_operating_mode = 1 ;    // now start in mode 1 = initialising
  _controller_loop_interval = 10 ;
  _controller_hardware_local_remote_toggle = 0 ;//IP_toggle_pin ;  // use this to read pin31 high or low to set IP and MAC
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
}


void CIVController::set_id(byte id)
{
  _id = id;
}

byte CIVController::get_id()
{
  return _id;
}


        void CIVController::set_controller_operating_mode(int8_t mode)
                  {
                  _controller_operating_mode = mode;
                   //   - sets operationg MODE value  for THIS device
                  }

        int8_t CIVController::get_controller_operating_mode()
                    {
                return _controller_operating_mode ;
                //   - gets operationg MODE value  for this device... and returns it
                    }

        void CIVController::set_controller_loop_interval(long duration)   //
                      {
                        _controller_loop_interval=duration;
                        // allows adjustment of the LED flash rate
                      }

        long CIVController::get_controller_loop_interval()    //
                {
                  return _controller_loop_interval ;
                }

        void CIVController::set_local_IP_port_and_MAC_swap(bool mode)
                  {
                  _controller_local_IP_port_mac_flag = mode ;
                  // sets local or remote, depending on physical state of pin 31 ( IP_toggle_pin  )
                  }

        bool CIVController::get_local_IP_port_and_MAC_swap()
              {
              return _controller_local_IP_port_mac_flag ;
              }

      void CIVController::set_listen_for_CI_V_message_flag(bool state)  //
              {
                _listen_for_CI_V_message_flag = state ;
              }

          bool CIVController::get_listen_for_CI_V_message_flag()
              {
                return _listen_for_CI_V_message_flag ;
              }

      void CIVController::set_just_sent_CI_V_message_flag(bool state)  //
                {
                  _just_sent_CI_V_message_flag = state ;
                }

        bool CIVController::get_just_sent_CI_V_message_flag()
              {
                return _just_sent_CI_V_message_flag ;
              }

      void CIVController::set_CI_V_TX_inhibit_flag(bool state)
                  {
                    _tx1_inhibit_flag = state ;
                  }

        bool CIVController::get_CI_V_TX_inhibit_flag()
                {
                  return _tx1_inhibit_flag ;
                }

      void CIVController::set_tx_verified_flag(bool state)
                    {
                _tx_verified_flag = state ;
                    }

        bool CIVController::get_tx_verified_flag()
                {
                  return _tx_verified_flag ;
                }

        void CIVController::set_TX_MESSAGE_NOT_CORRUPT_flag(bool state)
                      {
                  _TX_MESSAGE_NOT_CORRUPT_flag = state ;
                      }

        bool CIVController::get_TX_MESSAGE_NOT_CORRUPT_flag()
                    {
                return _TX_MESSAGE_NOT_CORRUPT_flag ;
                    }

      void CIVController::set_collision_flag(bool state)
                  {
                    _collision_flag = state ;
                  }

        bool CIVController::get_collision_flag()
              {
                return _collision_flag ;
              }

      void CIVController::set_tx_jam_code_flag(bool state)
                    {
                _tx_jam_code_flag = state ;
                    }

        bool CIVController::get_tx_jam_code_flag()
                  {
              return _tx_jam_code_flag ;
                  }

      void CIVController::set_rx_jam_code_flag(bool state)
                    {
                _rx_jam_code_flag = state ;
                    }

        bool CIVController::get_rx_jam_code_flag()
                {
                  return _rx_jam_code_flag ;
                }

    bool CIVController::get_CI_V_TX_HOLDOFF_flag()  // .. decrements the time 2 live counter and returns state
        {
          if (_CI_V_time2live > 0 )
            {
              _CI_V_time2live-- ;    // each time 'read' decrement until zero
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

      byte CIVController::set_CI_V_TX_HOLDOFF_counter(byte count)  // sets the time 2 live counter
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

          byte CIVController::read_CI_V_TX_HOLDOFF_counter()  // read the time 2 live value
                    {
                      return _CI_V_time2live ;

                    }



/*******************************************************************************
 * Author: Robert S. French <rfrench@rfrench.org>
 * Derived from work by John Rowling
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative
 * Commons, PO Box 1866, Mountain View, CA 94042, USA.
 ******************************************************************************/

byte ci_v_rx_DATA[15] =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

RingBuf<uint8_t, 79> ci_v_ring_Buffer;

byte ci_v_TX1_RX1_MESSAGEs[35][14] = {
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

     // {   8,  6,   0x16,   0x40,  0x00,  0xFD,     0,      0,     0,     0,    0,   12,   12,   1}, // 35 ... set  NR OFF / ON ... reply is only 6 bytes FE FE E0 96 FB FD .
     // {   9,  6,   0x14,   0x06,  0x00,  0x00,  0xFD,      0,     0,     0,    0,    0,    0,   1}, // 36 ... set NR LEVEL  0 - 255          ditto
     // {   8,  6,   0x16,   0x22,  0x00,  0xFD,     0,      0,     0,     0,    0,    0,    0,   1}, // 37 ... set NB OFF / ON                ditto
     // {   9,  6,   0x1A,   0x05,  0x01,  0x76,  0xFD,      0,     0,     0,    0,    0,    0,   1}, // 38 ... set NB DEPTH  00- 09  (8600 M4=76) different for (7300 M4=89)
     // {   9,  6,   0x1A,   0x05,  0x01,  0x77,  0xFD,      0,     0,     0,    0,    0,    0,   1}, // 39 ... set NB WIDTH  0 - 255 (8600 M4=77) different for (7300 M4=90)
     // {   9,  6,   0x14,   0x12,  0x00,  0x00,  0xFD,      0,     0,     0,    0,    0,    0,   1}, // 40 ... set NB LEVEL  0 - 255          ditto
     // {   8,  6,   0x16,   0x41,  0x00,  0xFD,     0,      0,     0,     0,    0,    0,    0,   1}, // 41 ... set NOTCH AUTO OFF / ON        ditto
     // {   8,  6,   0x16,   0x48,  0x00,  0xFD,     0,      0,     0,     0,    0,    0,    0,   1}, // 42 ... set NOTCH MANUAL               ditto
     // {   8,  6,   0x16,   0x57,  0x00,  0xFD,     0,      0,     0,     0,    0,    0,    0,   1}, // 43 ... set NOTCH WIDTH 00 - 02        ditto
     // {   9,  6,   0x14,   0x0D,  0x00,  0x00,  0xFD,      0,     0,     0,    0,    0,    0,   1}  // 44 ... set NOTCH POSITION 0 - 255     ditto
};

byte CIVController::read_unsolicited_message()
{
#ifdef MEGA2560
  BlueLED.on();
#endif
  delay(1);
#ifdef MEGA2560
  BlueLED.off();
#endif
  boolean message_flag = false;
  byte count = 0;
  byte last_incoming_FD = 0;
  byte frame_end_FD_count = 0;
  byte frame_start_FE_count = 0;
  byte packet_terminator_count = 0;
  byte incoming_TTL1_Byte;
  boolean one_star = false;

  //clear buffer
  //memset(packetBuffer, 0, NTP_PACKET_SIZE);
  //memset(NET_MESSAGE_BUFFER, 0, NTP_PACKET_SIZE);


  //  *******************************************************
  //      CI_V Received ???
  //  *******************************************************

#ifdef MEGA2560
  while (Serial1.available() > 0)
  {
    message_flag = true;
    incoming_TTL1_Byte = Serial1.read();      // get first or next byte  <<<---------------------
    //saves incoming byte in 'incoming_TTL1_Byte' variable
    RX1_MESSAGE_BUFFER[count] = incoming_TTL1_Byte;       // copy incomming byte into an rx array
    ci_v_ring_Buffer.push(RX1_MESSAGE_BUFFER[count]);
    count++;        // count bytes
    if ( count >= 46 )
    {
      break;
    }             // bug out  was 46 for 4 lines of CI-v messages but lost part of
            // third line after 23 chars
  }       // end of while

  if ( message_flag == true )
  {        //begin_if message flag
    startMillis=millis();
    message_flag = false;

    //  *******************************************************
    //        read the packet into packetBufffer
    //  *******************************************************

    //  ************************************************************************************************************
    //             format check
    //  ************************************************************************************************************
    for (int i=0; i<=47; i++)      // begin_for copy
    {
      // now print & simple check on what was SENT
      if ( byte(RX1_MESSAGE_BUFFER[i]) == 0xFD ) {
        frame_end_FD_count++;
        last_incoming_FD = i+1;
        // Serial.println ('.') ;
        // Serial.println("FD") ;
      }

      if(byte(RX1_MESSAGE_BUFFER[i]) ==0xFE)        // look for FE
      {
        frame_start_FE_count++;                 // this should be even
        // and twice the FD count
        // Serial.println("FE") ;
      }

      if(byte(RX1_MESSAGE_BUFFER[i]) ==0xFA)        // look for FE
      {
        frame_start_FE_count++;                 // this should be even
        //and twice the FD count
        //Serial.println("FA") ;
      }

    }     // end of for copy
  }          //message flag true

  else
  {

  }
 #endif /* MEGA2560 */

 #ifdef PRO_MINI
  while (Serial.available() > 0)
  {
    message_flag = true;
    incoming_TTL1_Byte = Serial.read();
    RX1_MESSAGE_BUFFER[count] = incoming_TTL1_Byte;
    ci_v_ring_Buffer.push(RX1_MESSAGE_BUFFER[count]);
    count++;
    if (count >= 46) { break; }            // bug out  was 46 for 4 lines
    // of CI-v messages but lost part of third line after 23 chars
  }
  if (message_flag)
  {
    startMillis = millis();
    message_flag = false;

    for (int i=0; i<=47; i++)
    {
      // now print & simple check on what was SENT
      if (byte(RX1_MESSAGE_BUFFER[i]) == 0xFD)
      {
        frame_end_FD_count++;
        last_incoming_FD = i+1;
      }
      if (byte(RX1_MESSAGE_BUFFER[i]) == 0xFE)
      {
        frame_start_FE_count++;
      }
    }
  }
#endif /* PRO_MINI */

  return count;
}

byte tx_ci_v_via_serial_ttl (byte sender)    // 'sender' is the CI-V CMD byte
// note : this function returns the DATA VALID FLAG
// all precedded by FE  FE  96  E0 ( 253  253  150  224 ...
//    { ...  3, 253,   0,   0,   0,   0}, //  8 what current frequency ... 6 bytes sent
//    { .... 4, 253,   0,   0,   0,   0}, //  9 what current mode ... 6 bytes sent
//    { ... 21,   2, 253,   0,   0,   0}, //  10 what S-meter value ? ..... 7 bytes sent
//    { ... 0, a, b, c, d, e, 253      }  // 11 bytes sent  reply would be 6 bytes FE FE E0  96 FB FD

{     ////01 TX1  - start of  function       >>>>>>>>>>>>>>>>>>>>>>>>>>>>-------------------------------------------01---------------->>>
  byte result;
  byte tx_count = 0;  // will hold the number of bytes sent
  byte rx_count = 0;  // will hold the number of bytes sent (looped back) and bytes received
  byte incoming_TTL1_Byte;
  byte FEat = 0;   // use to move along RX buffer array in tests below tx
  byte TOat = 0;
  byte FROMat = 0;
  byte type = 0;   // reply type  o = none  1 = FB or FA   2=DATA
  byte FDat = 0;   // where the FD byte should be
  byte FBat = 0;
  startMillis=millis();

  if ( sender == 0)
  {    //02
    return 0;
  }    //\\02 block TX of '0th' table entry
  //  *******************************
  if (!CIV.get_CI_V_TX_inhibit_flag())
  { //03 false... then TX is permitted  -------------------------------------------------------------------------------03------->>>

    //  *******************************

    // *****************************************************************************************************************
    // ASSEMBLE CI-V message to send ... use control character  FIRST load TX1_MESSAGE_BUFFER FE  FE  to..  from ...  etc
    // *****************************************************************************************************************

    TX1_MESSAGE_BUFFER[0] = 254; //  FE
    TX1_MESSAGE_BUFFER[1] = 254; //  FE
    TX1_MESSAGE_BUFFER[2] = Icom.get_id(); // 8600 = 96   7300 = 94
    TX1_MESSAGE_BUFFER[3] = 224; //  E0       from ....assumes E0H controller id
      #ifdef MEGA2560

    //  ********************************************************************************************************************
    //    Load the PAYLOAD into the transmit buffer 'TX1_MESSAGE_BUFFER' from ci_v_TX1_RX1_MESSAGEs[25][11]
    //  ********************************************************************************************************************

    for (int i=0; i<15; i++) // note : 'break' will exit the loop before END is reached
           // prepare buffer ... i steps through the ci_v_TX1_RX1_MESSAGEs[row = sender]
           //[column = bytes to copy]  ) array ... load correct ROW into TX1_MESSAGE_BUFFER
    {      //04
      //             row    col
      TX1_MESSAGE_BUFFER[i+4] = ci_v_TX1_RX1_MESSAGEs[sender][i+2];      // i+ 4 skips the tx
      //message header that is pre loaded above , i+2 skips the tx and rx anticipated byte count values
      if (TX1_MESSAGE_BUFFER[i+4] == 253)     // step through the array (and SEND) up to and
                      // including '253' = 'FD'
      //....time to bail out of for loop
      {                //05
        break;            // exit for loop
      }                //\\05
    }       //\\ 04 end for loop


    //    *************************************************************************************************
    //  NOW SEND the whole CI-V TX1_MESSAGE_BUFFER
    //    *************************************************************************************************
    // i steps through the array // ..sending each byte
    //note : 'break' will exit the loop WHEN 0XFD (253) is seen before END is reached
    for (int i=0; i<15; i++)
    {      //06
      Serial1.write (TX1_MESSAGE_BUFFER[i]);  // THIS ACTUALLY SENDS THE CI-V DATA via pin 18 TTL 1 TX
      if (TX1_MESSAGE_BUFFER[i] == 253)       // step through the array (and SEND) up to and including '253' = 'FD'.
      //         ..then time to bail out of for loop
      {   //07
        result = i;    //  result =  0,1,2,3,4,5,....n  but n+1 are sent
        tx_count = i+1;    // adjust the tcx count for the jupmed loop
        break;    // exit for loop // note, again that 'break' will exit the loop before 15 is reached
      }   //\\07
    }       // \\06   end for loop

        #endif  // end of ifdef 2560

    //  duplicate code but for pro mini serial port

        #ifdef PRO_MINI

    //  ************************************************************

    //  THEN load the PAYLOAD into the transmit buffer
    //  'TX1_MESSAGE_BUFFER' from ci_v_TX1_RX1_MESSAGEs[25][11]

    //  ************************************************************

    for (int i=0; i<15; i++)      // note : 'break' will exit the loop before END is reached
    // prepare buffer ... i steps through the ci_v_TX1_RX1_MESSAGEs[row = sender]
    //[column = bytes to copy]  ) array ... load correct ROW into TX1_MESSAGE_BUFFER
    {      //08
      //             row    col
      TX1_MESSAGE_BUFFER[i+4] = ci_v_TX1_RX1_MESSAGEs[sender][i+2];      // i+ 4 skips the tx
      // message header that is pre loaded above , i+2 skips the tx and rx anticipated byte count values
      if (TX1_MESSAGE_BUFFER[i+4] == 253)     // step through the array (and SEND) up to and including
      // '253' = 'FD'....time to bail out of for loop
      {                //09
        break;              // exit for loop
      }                //\\09
    }       //\\08 end for loop

    host_print ( "TX1 (5335):  ");
    //    *************************************************************
    //  NOW SEND the whole CI-V TX1_MESSAGE_BUFFER
    //    *************************************************************
    // i steps through the array // ..sending each byte

    for (int i=0; i<15; i++)
    {         //10
      Serial.write (TX1_MESSAGE_BUFFER[i]);    // THIS ACTUALLY SENDS THE CI-V DATA via pin 18 TTL 1 TX
      // show TX

      if ( byte(TX1_MESSAGE_BUFFER[i]) <10 )
      {

        host_print ("0");    // print leading zero
        host_print (byte(TX1_MESSAGE_BUFFER[i]),HEX);
        host_print (" ");
      }

      else {
        host_print( byte(TX1_MESSAGE_BUFFER[i]),HEX);
        host_print (" ");
      }



      if (TX1_MESSAGE_BUFFER[i] == 253)         // step through the array (and SEND) up to and
                  //including '253' = 'FD'....time to bail out of for loop
      {      //11
        result = i;        //  result =  0,1,2,3,4,5,....n  but n+1 are sent
        tx_count = i+1;        // adjust the tcx count for the jupmed loop
        break;        // exit for loop // note, again that 'break'
                //  will exit the loop before 15 is reached
      }      //\\11
    }         //\\10 end for loop

    host_println();


        #endif  // -pro mini

    // *********************************************************
    //        CI_V message sent  - now set TX flags
    // *********************************************************
    // set flags...  moved from tab A 'main loop'
    CIV.set_CI_V_TX_inhibit_flag(true);        // STOP further tx until a VALID loop back
    // AND reply received and checked
    // i.e. TX is a ONE SHOT until inhibit flipped
    CIV.set_just_sent_CI_V_message_flag(true);         // we have 'just sent' a message
    //(loop back needs to be veirfied)
    CIV.set_listen_for_CI_V_message_flag(false);          //  stop listen ... don't
    //transfer any othe recived bytes into
    //'our' rx buffer until processing / checking / validation process completed

    // *********************************************************
    //  start hold off counter : 6 loops  ?    need to limit polling to about 300ms intervals
    // *********************************************************
    CIV.set_CI_V_TX_HOLDOFF_counter(HOLDOFF_RATIO);        // e.g. 20 ... now cannot send a
    //polling message for 20 loops

    // *********************************************************
    delay (CIV_RX_SOLICITED_BUFFER_FILL_DELAY);        // about 15ms after TX of a request for data,
    // the delay is to allow reply to arrive in the RX buffer
    // *********************************************************

  }   //\\03 eof  TX inhibit false  <<<-----------------------------------------------------------------------------------------<<<---03

  else {           //12 skip through when ...CIV.get_CI_V_TX_inhibit_flag() == false

    return;           // normally drop past here to RX  below  OR exit  function when X inhib
    //was true ( during HOLD OFF loops )

  }          //\\12 eof false TX inhibit true

  // *********************************************************
  // CI-V request AND the Reply should be in rx buffer
  // *********************************************************

  // what if no reply ( radio not connected ? )  .... RX buffer length should be same
  //as TX buffer, or may also contain collision rubbish
  //
  // or every thing normal ...

  //  ************    NOW READ the whole CI-V RX1_MESSAGE_BUFFER      **********

            #ifdef MEGA2560


  while (Serial1.available() > 0)           //read AND copy into buffer... rx_count starts at ZERO
  {          ///13
    incoming_TTL1_Byte = Serial1.read();          // get first or next byte  <<<----
    //moves incoming byte in 'incoming_TTL1_Byte' variable..  buffer now empty ?
    RX1_MESSAGE_BUFFER[rx_count]  = incoming_TTL1_Byte;           // copy incomming byte
    //into the global RX array   rx_count 0..1..2..
    // print out the HEX values when the ifdef allows

    // could add ring buffer here ?
    rx_count++;          // count them
  }           //\\13 end of while


            #endif // -2560

            #ifdef PRO_MINI

  host_print ( "RX1 (5440): ");
  while (Serial.available() > 0)               //read AND copy and EMPTY   all bytes in buffer ... that will be SENT message and Reply message from buffer
  {              //14

    incoming_TTL1_Byte = Serial.read();          // get first or next byte  <<<--
    //------------------- saves incoming byte in 'incoming_TTL1_Byte' variable
    RX1_MESSAGE_BUFFER[rx_count]  = incoming_TTL1_Byte;           // copy incomming byte into the global RX array


    if ( byte(RX1_MESSAGE_BUFFER[rx_count]) <10 )
    {

      host_print ("0");
      host_print (byte(RX1_MESSAGE_BUFFER[rx_count]),HEX);
      host_print (" ");
    }

    else {
      host_print( byte(RX1_MESSAGE_BUFFER[rx_count]),HEX);
      host_print (" ");
    }


    rx_count++;          // count them
  }               //\\14 end of while

  host_println();

            #endif // -pro mini

  //   *********************************************************
  //   do validity chech HERE then reset flags - compare TX (sent bytes) and part of RX bytes ...
  //   i.e are the looped back 'TX bytes' uncorrupted
  //    *********************************************************

  // first test ... is RX count greater than TX count ?   i.e. if TX = n and RX = 0 ... no device reply ? or no link to radio

  //   *********************************************************
  //       CHECK SENT MESSAGE IS NOT CORRUPTED
  //   *********************************************************

  if ( rx_count >= tx_count )
  {                //15
    CIV.set_TX_MESSAGE_NOT_CORRUPT_flag(true);           //assume it's ok
    //... maybe qualify that e.g. if RXcount >= TX count set it true
  }                //\\16

  for (int i=0; i<tx_count; i++)           //
  // i steps through first part of the array  which is the TX message  .......................WHAT if TX=0 and RX=0 ?
  {          //17
    // move this to RX check below CIV.set_TX_MESSAGE_NOT_CORRUPT_flag(true) ;
    // set flag to true - i.e. assume ok unless error detectec in 'compare' below
    // RX1_MESSAGE BUFFER will contain TX'd and RX'd message
    if (RX1_MESSAGE_BUFFER[i]
        != TX1_MESSAGE_BUFFER[i])           // if not same PRINT SIDE BY SIDE
    {      //18

      CIV.set_TX_MESSAGE_NOT_CORRUPT_flag(false);         // assume it is false if
      //rx and tx lenght are same ?  or always until proven ?

            #ifdef MEGA2560
      RedLED.on();             // led off after last tx related action
      Serial.println(" collisssion !!! ");           // create collision error if TX and looped RX are different
      Serial.print ( "TX1 :");
      for (int i=0; i<tx_count; i++)
      {                  //19
        Serial.print (TX1_MESSAGE_BUFFER[i],HEX);
      }                  //\\19
      Serial.println();
      Serial.print ( "RX1 :");
      for (int i=0; i<tx_count; i++)
      {                  //20
        Serial.print (RX1_MESSAGE_BUFFER[i],HEX);
      }                  //\\20
            #endif

            #ifdef PRO_MINI
      //host_println(" collisssion !!! "); // create collision error if TX and looped RX are different
      host_print ( "TX1 :  ");
      for (int i=0; i<tx_count; i++)
      {                  //21
        host_print(TX1_MESSAGE_BUFFER[i],HEX);
      }                  //\\21
      host_println();
      host_print ( "RX1 : ");
      for (int i=0; i<tx_count; i++)
      {                  //22
        host_print (RX1_MESSAGE_BUFFER[i],HEX);
      }                  //\\22
            #endif

    }      //\\18
    else
    {      //23

      CIV.set_TX_MESSAGE_NOT_CORRUPT_flag(true);         // assume it is false if rx and tx lenght are same ?
      //or always until proven ?
    }      //\\23

  }

  if (CIV.get_TX_MESSAGE_NOT_CORRUPT_flag() == true)
  //  move all the stuss below into here
  {                      //24
                           #ifdef MEGA2560
    //
    Serial.println ( "TX1 :  good ");

                          #endif


                          #ifdef PRO_MINI
    host_println ( "TX1 :  good ");

                          #endif

  }                      //\\24

  // *******************************
  //       clear TX buffer
  // *******************************

  memset(TX1_MESSAGE_BUFFER, 0, 20);

  //  SHOULD ANY FURTHER PROCESSING BE CONDITIONAL ON passing the VALIDITY CHECK ?
  //  yes !
  // *****************************************************************
  // ************    NOW process received CI-V data payloads    ******
  // *****************************************************************

  if (( CIV.get_TX_MESSAGE_NOT_CORRUPT_flag() == true)
      & ( rx_count >0 )& (tx_count > 0)  )


  {      //25  // conditional start of processing payload

    // ci_v_TX1_RX1_MESSAGEs[snder][13] is the reply type
    // reply FE1 is at tx_count ... FEat this

    FEat = ci_v_TX1_RX1_MESSAGEs[sender][0];         // the position in the RX Buffer where the first FE should be located
    TOat = FEat + 2;
    FROMat = FEat + 3;



    type = ci_v_TX1_RX1_MESSAGEs[sender][13];            // the position in the RX Buffer where the Reply type id should be located

    FDat = (  (  ci_v_TX1_RX1_MESSAGEs[sender][0]
           + ci_v_TX1_RX1_MESSAGEs[sender][1]) -1 );        // the position in the RX Buffer where FD should be located

    if (FDat > 47)
    {             //26
      Serial.print (   "HELP !!!  ");


    }            //\\26

    FBat = FEat + 4;            // the position in the RX Buffer where FB or FA should be located


    if ( RX1_MESSAGE_BUFFER[ FEat ]  == 0xFE )
    {          //27  is this value  = FE ?

      Icom.set_FE1_flag (true);
      Icom.set_RX_Message_validity (128);
      //Icom.set_RX_Message_validity ( Icom.get_RX_Message_validity () && 255 ) ;

    }        //\\27

    if ( RX1_MESSAGE_BUFFER[ FEat +1 ]  == 0xFE )
    {        //28  is this value  = FE ?
      Icom.set_FE2_flag (true);
    }         //\\28

    if ( RX1_MESSAGE_BUFFER[ TOat ]  == 0xE0 )
    {        //29  is this value  = controller ID ?
      Icom.set_to_flag (true);
    }        //\\29


    if ( RX1_MESSAGE_BUFFER[ FROMat ]  == Icom.get_id()  )
    {        //30  is this value  = RADIO ID ?
      Icom.set_from_flag (true);
    }        //\\30


    if ( type== 0 )            // reply type 0
    {        //31
    }        //\\32

    if (type == 1 )            // reply type 1
    {        //33
      if ( RX1_MESSAGE_BUFFER[ FBat ]  == 0xFB )
      {        //34  is this value  = FB
        Icom.set_FB_flag (true);
        Icom.set_FA_flag (false);
      }            //\\34

      if ( RX1_MESSAGE_BUFFER[ FBat ]  == 0xFA )
      {              //35  is this value  = FA ?
        Icom.set_FB_flag (false);
        Icom.set_FA_flag (true);
      }               //\\35

    }        //\\33

    if (type == 2 )        // reply type 2
    {              //36
      if ( RX1_MESSAGE_BUFFER[ FEat + 4 ]  == 0xFB )
      {              //37  is this value  = FB ?

      }            //\\37

      if ( RX1_MESSAGE_BUFFER[ FEat + 4 ]  == 0xFA )
      {                //38  is this value  = FA ?

      }               //\\38

    }              //\\36


    if ( RX1_MESSAGE_BUFFER[ FDat ]  == 0xFD )
    {          // 39 is this value  = FE ?
      Icom.set_FD_flag (true);
    }          //\\39
  }           //\\25 end of conditional processing of payload


  //  *********************************************************
  //  code ok to here ... TX is now 'one shot' until tx1_inhibit is reset (to false) ... also need to reset
  //  ( to false ) the 'just sent flag' somewhere ... after verification process
  //  *********************************************************
  //            *
  //            *
  //            *
  //             *    *    *
  //               *  *  *
  //            *
  //            ***********************
  //               done it here :-)
  //            ***********************

  // wrong logic here ?

  CIV.set_CI_V_TX_inhibit_flag(true);                // stop further tx

  CIV.set_just_sent_CI_V_message_flag(false);                 // no longer true buffers now empty

  CIV.set_listen_for_CI_V_message_flag(true);            //  stop listen ... don't transfer any othe
  //recived bytes into 'our' rx buffer until processing / checking / validation process completed

  return CIV.get_TX_MESSAGE_NOT_CORRUPT_flag();             // change this to an aggregate flag that indicates the data is valid

}      // 01 end of serial ttl tx function - which SENDS CI-V data to the One Wire Bus  <<<<<<<<---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   END OF tx serial 1 FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//
// end of tab C (serial via TX1)

// start tab D data conversion


#ifdef XXX
unsigned long get_frequency() {   // pass it the command character ... why not just use '8' ..the actual command character

  // puts ascci vaules of the radio's VCO frequency into the data transmission buffer (Y_messageHolder1[ ]) while also re-ordering the bytes into ascending magnitude 10^0 ... 10^9 ... Hz to GHz

  boolean drawDots = false;
  unsigned long frequency =0;



  frequency = frequency + ci_v_rx_DATA[0]  * 10;              // 1   VCOF1=34
  //Y_messageHolder1[VCOF1] = ci_v_rx_DATA[0]+48; //  48   42     34
  frequency = frequency + ci_v_rx_DATA[1]  * 1;               // 0   VCOF0=35
  //Y_messageHolder1[VCOF0] = ci_v_rx_DATA[1]+48; //  49   43     35
  frequency = frequency + ci_v_rx_DATA[2]  * 1000;               //3
  //Y_messageHolder1[VCOF3] = ci_v_rx_DATA[2]+48; //  46   40     32
  frequency = frequency + ci_v_rx_DATA[3]  * 100;              //2
  // Y_messageHolder1[VCOF2] = ci_v_rx_DATA[3]+48; //   47   41     33
  frequency = frequency + ci_v_rx_DATA[4]  * 100000UL;             //5
  //Y_messageHolder1[VCOF5] = ci_v_rx_DATA[4]+48; //   44   38     30
  frequency = frequency + ci_v_rx_DATA[5]  * 10000UL;              //4  //
  // Y_messageHolder1[VCOF4] = ci_v_rx_DATA[5]+48; //   45   39      31
  frequency = frequency + ci_v_rx_DATA[6]  * 10000000;             //7
  // Y_messageHolder1[VCOF7] = ci_v_rx_DATA[6]+48; //  42  36      28
  frequency = frequency + ci_v_rx_DATA[7]  * 1000000;             //6
  // Y_messageHolder1[VCOF6] = ci_v_rx_DATA[7]+48; //   43  37      29
  frequency = frequency + ci_v_rx_DATA[8]  * 1000000000;              //9
  // Y_messageHolder1[VCOF9] = ci_v_rx_DATA[8]+48; // 40  34  26

  frequency = frequency + ci_v_rx_DATA[9]  * 100000000;             //8
  // Y_messageHolder1[VCOF8] = ci_v_rx_DATA[9]+48; //  41  35  27

// calculate band

  if ((frequency >= LO_160M)  && (frequency <= HI_160M ))           //           160metre
  {
    Icom.set_current_band(1);
  }
  if ((frequency >= LO_80M)  && (frequency <= HI_80M ))           // 3.400000 - 4.099999 MHz        80metre
  {
    Icom.set_current_band(2);
  }
  else if ((frequency >= LO_40M)  && (frequency <= HI_40M ))           // 6.900000 - 7.499999 MHz         40metre
  {
    Icom.set_current_band(3);
  }
  else if ((frequency >= LO_30M ) && (frequency <= HI_30M ))           // 13.900000 - 14.499999 MHz       30metre
  {
    Icom.set_current_band(4);
  }
  else if ((frequency >= LO_20M ) && (frequency <= HI_20M ))           //               20metre
  {
    Icom.set_current_band(5);
  }
  else if ((frequency >= LO_17M ) && (frequency <= HI_17M ))           //
  {
    Icom.set_current_band(6);
  }
  else if ((frequency >= LO_15M ) && (frequency <= HI_15M ))           //
  {
    Icom.set_current_band(7);
  }
  else if ((frequency >= LO_12M ) && (frequency <= HI_12M ))           //
  {
    Icom.set_current_band(8);
  }
  else if ((frequency >= LO_10M ) && (frequency <= HI_10M ))           //
  {
    Icom.set_current_band(9);
  }
  else if ((frequency >= LO_6M ) && (frequency <= HI_6M ))           // 50.000000 - 52.000000 MHz       6metre
  {
    Icom.set_current_band(10);
  }
  else { Icom.set_current_band( 0 );}            // out of band


// ****************************************************
//   copy frequ data bytes into band store
// ****************************************************

  for ( int i = 0; i <= 4; i++ )
  {
    band_store[Icom.get_current_band()][i]=RX1_MESSAGE_BUFFER[ i+11 ];              // copy frequ data bytes into band store  11 - 15
  }
  return frequency;
  /*
   * Band id's     MY SYMBOL  ( source ICOM )
   *    01      A  1.8MHz  160m
   *    02      B  3.5MHz  80m
   *    03      C  7.0MHz  40m
   *    04      D  10MHz   30m
   *    05      E  14MHz   20m
   *    06      F  18MHz
   *    07      G  21MHz
   *    08      H  24MHz
   *    09      I  28MHz
   *    10      J  50MHz    6m
   */


  // BAND

// ***** Frequency (Hz) to Band ******************************

  /*
   * Band id's     MY SYMBOL  ( source ICOM )
   *    01      A  1.8MHz  160m
   *    02      B  3.5MHz  80m
   *    03      C  7.0MHz  40m
   *    04      D  10MHz   30m
   *    05      E  14MHz   20m
   *    06      F  18MHz
   *    07      G  21MHz
   *    08      H  24MHz
   *    09      I  28MHz
   *    10      J  50MHz    6m



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

   #define LO_40M  7000000  //   BAND=3
   #define HI_40M  7200000

   #define LO_30M  10100000  //  BAND=4
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
#endif /* XXX */

/// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//   END OF BIT FREQUENCY FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


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
    return Serial1.available();    // returns true if bytes ARE in h/w buffer
  }
  #endif



 #ifdef PRO_MINI
  {
    return Serial.available();    // returns true if bytes ARE in h/w buffer
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
//   END OF listen serial 1 FUNCTION
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//

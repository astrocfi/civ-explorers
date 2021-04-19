/*******************************************************************************
 * Author: Robert S. French <rfrench@rfrench.org>
 * Derived from work by John Rowling
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc/4.0/ or send a letter to Creative
 * Commons, PO Box 1866, Mountain View, CA 94042, USA.
 ******************************************************************************/

#ifndef __CIV_CONTROLLER_H__
#define __CIV_CONTROLLER_H__

#include <Arduino.h>

#include <RingBuf.h>

unsigned long startMillis, stopMillis, diffMillis;

byte RX1_MESSAGE_BUFFER[48];
byte TX1_MESSAGE_BUFFER[20];  // used to record WHAT CI-V message was sent (up to 20 characters)... is that enough ?

byte RX1_COUNTER = 0;
byte TX1_COUNTER = 0;


#define CIV_RX_SOLICITED_BUFFER_FILL_DELAY  30  // was 26 if delay too short the reply might be cropped and appear to be a collision !
#define CIV_RX_UNSOLICITED_BUFFER_FILL_DELAY  10  // was 50
#define UNSOLICITED_HOLD_OFF 25
#define HOLDOFF_RATIO 4    //  was 45 number of loops between Sending CI-V messages poling the radio   .... about 22s per loop

extern byte ci_v_rx_DATA[15];
extern RingBuf<uint8_t, 79> ci_v_ring_Buffer;
extern byte ci_v_TX1_RX1_MESSAGEs[35][14];

class CIVController
{
  public:
    CIVController(byte id);
    void begin();

    byte get_id();
    void set_id(byte id);

    void set_controller_operating_mode(int8_t);  // not used as intended
    int8_t get_controller_operating_mode();

    void set_controller_loop_interval(long); // not used
    long get_controller_loop_interval();

    void set_local_IP_port_and_MAC_swap(bool);
    bool get_local_IP_port_and_MAC_swap();

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

    void set_collision_flag(bool);
    bool get_collision_flag();

    void set_tx_jam_code_flag(bool);
    bool get_tx_jam_code_flag();

    void set_rx_jam_code_flag(bool);
    bool get_rx_jam_code_flag();

    //holdoff
    bool get_CI_V_TX_HOLDOFF_flag();    // get the flag state AND decrimentinternal counter
    byte set_CI_V_TX_HOLDOFF_counter(byte count);  // time2live byte
    byte read_CI_V_TX_HOLDOFF_counter();     // read the current value of the internal counter

    byte CIVController::read_unsolicited_message();

  private:
    byte _id;

    long _controller_loop_interval; // new

    int8_t _controller_operating_mode;
    byte _controller_hardware_local_remote_toggle;
     // define pin to test for high / low which sets ip & mac via :  bool _controller_local_IP
    bool _controller_local_IP_port_mac_flag;
     // TRUE = local IP, port & MAC    FALSE = remote IP, port & MAC

    boolean _listen_for_CI_V_message_flag;
     // = false   // listening is monitoring the CI-V one wire bus for traffic
    boolean _receive_flag; // = false
     // this flag indicates receiving rather than listening
    boolean _just_sent_CI_V_message_flag; // = false
// a message has just been sent ... and will be checked by listening / receiving the looped back data stream
    boolean _tx1_inhibit_flag;  // = true
     // default is true i.e. do not transmit ... other checks will set this to false to allow transmission to occur ONCE
    boolean _tx_verified_flag; //
    boolean _TX_MESSAGE_NOT_CORRUPT_flag;
     // = false  // this flag indicates received datagram is valid  i.e. FE FE ... and FD  received ..IS THERE A CHECKSUM too ?
    boolean _collision_flag; // = false
     //  when true = a  was collision detected  .....  false = no collision occured when WE sent data
    boolean _tx_jam_code_flag; //  = false
     // jamming code sent( or to be sent ? ) because a collision was detected.
    boolean _rx_jam_code_flag; // = false
     // jamming code received
    //holdoff
    boolean _CI_V_TX_HOLDOFF_flag;
    byte    _CI_V_time2live;
};

#endif /* __CIV_CONTROLLER_H__ */

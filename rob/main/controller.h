#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_

#include <Arduino.h>

class Controller
{
  public:
    Controller(byte id);
    void begin();

    byte get_id();
    void set_id(byte id);

    void set_CONTROLLER_Operating_Mode(int8_t);  // not used as intended
    int8_t get_CONTROLLER_Operating_Mode();

    void set_CONTROLLER_LOOP_interval(long); // not used
    long get_CONTROLLER_LOOP_interval();

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

    void set_dayz(byte dayz);
    byte get_dayz();

    void set_monthz(byte monthz);
    byte get_monthz();

    void set_yearLowz(byte yearLowz);
    byte get_yearLowz();

    void set_yearHighz(byte yearHighz);
    byte get_yearHighz();

    void set_hourz(byte hourz);
    byte get_hourz();

    void set_minz(byte minz);
    byte get_minz();

  private:
    byte _id;

    long _CONTROLLER_LOOP_interval; // new

    int8_t _CONTROLLER_operating_mode;
    byte _CONTROLLER_hardware_local_remote_toggle;
     // define pin to test for high / low which sets ip & mac via :  bool _CONTROLLER_local_IP
    bool _CONTROLLER_local_IP_port_mac_flag;
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

#endif /* __CONTROLLER_H_ */

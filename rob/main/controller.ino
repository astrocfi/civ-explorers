Controller::Controller(byte id)
{
  _id = id;
}

void Controller::begin()
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
}


void Controller::set_id(byte id)
{
  _id = id;
}

byte Controller::get_id()
{
  return _id;
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

        long Controller::get_CONTROLLER_LOOP_interval()    //
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

    bool Controller::get_CI_V_TX_HOLDOFF_flag()  // .. decrements the time 2 live counter and returns state
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

#ifndef __CIV_H__
#define __CIV_H__

#include <Arduino.h>
#include <RingBuf.h>

#define CIV_RX_SOLICITED_BUFFER_FILL_DELAY  30  // was 26 if delay too short the reply might be cropped and appear to be a collision !
#define CIV_RX_UNSOLICITED_BUFFER_FILL_DELAY  10  // was 50
#define UNSOLICITED_HOLD_OFF 25
#define HOLDOFF_RATIO 4    //  was 45 number of loops between Sending CI-V messages poling the radio   .... about 22s per loop

extern byte ci_v_rx_DATA[15];
extern RingBuf<uint8_t, 79> ci_v_ring_Buffer;
extern byte ci_v_TX1_RX1_MESSAGEs[35][14];

#endif /* __CIV_H__ */

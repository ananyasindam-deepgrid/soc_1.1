#include <stdlib.h>
#include <stdio.h>
#include "../base/dgridlib.h"
#include "../apps/gdi/gdi.h"
#include "soc.h"
#include "can_bridge.h"

extern "C"
{
extern int main(void);
extern void irqCallback(void);
}

// __dso_handle is function pointer to do any cleanup of global object when 
// program exit.
// But this is a baremetal embedded system so we never have a program exit
// except when doing a reboot
// Set __dso_handle to zero

void *__dso_handle=0;

extern int test(void);

extern int vision_ai(void);

extern int chat();

//-----------------------------------------
// Application main entry
// 2 execution cases: vision example or test suites.
//-----------------------------------------

int main() {
   dgridInit();
   GdiInit();

#ifdef DGRID_UNIT_TEST
   // Run unit tests against test vectors
   while(1){
      test();
   }
#endif

#ifdef DGRID_LLM_TEST
   // Run chatbot with smollm2-135M LLM model
   for(;;) {
      chat();
   }
#endif

#ifdef DGRID_VISION_TEST
   // Run various vision tests
   //   - object detection
   //   - image classfication
   //   - optical flow
   //   - Harris-Corner point-of-interests
   //   - Edge detection
   DisplayInit(DISPLAY_WIDTH,DISPLAY_HEIGHT);
   CameraInit(WEBCAM_WIDTH,WEBCAM_HEIGHT);
   for(;;) {
      vision_ai();
   }
#endif

#ifdef DGRID_CAN_TEST
   // CAN bridge control loop
   // Autonomously communicates with WS-TTL-CAN module via UART1
   // All received CAN frames are logged to DDR memory
   {
      CanFrame_t rx, tx;
      uint32_t last_tick;
      uint32_t frame_count = 0;

      CanBridgeInit();
      last_tick = TimeGet();

      // Blink LED to indicate CAN mode active
      LedSetState(0x05);

      printf("CAN bridge started. Log at DDR addr: 0x%08x\n",
             (unsigned int)CanLogGetPtr());

      for (;;) {
         // --- Receive any incoming CAN frames and log to DDR ---
         if (CanReceive(&rx)) {
            CanLogFrame(&rx);
            frame_count++;
            // Toggle LED[0] on each received frame
            LedSetState(frame_count & 1);
         }

         // --- Periodic TX: send a heartbeat/request frame every 100ms ---
         if ((TimeGet() - last_tick) >= 100000) {
            last_tick = TimeGet();
            tx.can_id = 0x00000601;   // Configure to match your motor controller
            tx.dlc    = 8;
            tx.data[0] = 0x01;        // Command byte (customize as needed)
            tx.data[1] = 0x00;
            tx.data[2] = 0x00;
            tx.data[3] = 0x00;
            tx.data[4] = 0x00;
            tx.data[5] = 0x00;
            tx.data[6] = 0x00;
            tx.data[7] = 0x00;
            CanSend(&tx);
         }
      }
   }
#endif
    return 0;
}

void irqCallback() {
}

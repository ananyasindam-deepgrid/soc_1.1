#include <stdlib.h>
#include <string.h>
#include "can_bridge.h"
#include "soc.h"

// DDR-resident CAN log ring buffer
static CanLog_t *can_log = 0;

// RX state machine buffer
static uint8_t rx_buf[CAN_FRAME_SIZE];
static int rx_idx = 0;

void CanBridgeInit(void) {
   can_log = (CanLog_t *)malloc(sizeof(CanLog_t));
   memset(can_log, 0, sizeof(CanLog_t));
}

void CanSend(const CanFrame_t *frame) {
   uint8_t buf[CAN_FRAME_SIZE];

   // Byte 0: Frame Info
   // FF=1 for extended, RTR=0, DLC in bits 3:0
   buf[0] = (1 << 7) | (frame->dlc & 0x0F);

   // Bytes 1-4: CAN ID, big-endian
   buf[1] = (frame->can_id >> 24) & 0xFF;
   buf[2] = (frame->can_id >> 16) & 0xFF;
   buf[3] = (frame->can_id >>  8) & 0xFF;
   buf[4] = (frame->can_id      ) & 0xFF;

   // Bytes 5-12: Data (pad with 0 if DLC < 8)
   for (int i = 0; i < 8; i++) {
      buf[5 + i] = (i < frame->dlc) ? frame->data[i] : 0x00;
   }

   // Write all 13 bytes to UART1
   for (int i = 0; i < CAN_FRAME_SIZE; i++) {
      while (!Uart1WriteAvailable());
      Uart1Write(buf[i]);
   }
}

int CanReceiveAvailable(void) {
   while (Uart1ReadAvailable() && rx_idx < CAN_FRAME_SIZE) {
      rx_buf[rx_idx++] = Uart1Read();
   }
   return (rx_idx >= CAN_FRAME_SIZE);
}

int CanReceive(CanFrame_t *frame) {
   if (!CanReceiveAvailable()) return 0;

   frame->frame_info = rx_buf[0];
   frame->dlc        = rx_buf[0] & 0x0F;
   frame->can_id     = ((uint32_t)rx_buf[1] << 24) |
                        ((uint32_t)rx_buf[2] << 16) |
                        ((uint32_t)rx_buf[3] <<  8) |
                        ((uint32_t)rx_buf[4]      );
   for (int i = 0; i < 8; i++) {
      frame->data[i] = rx_buf[5 + i];
   }
   rx_idx = 0;
   return 1;
}

void CanLogFrame(const CanFrame_t *frame) {
   if (!can_log) return;

   can_log->frames[can_log->write_idx] = *frame;
   can_log->write_idx = (can_log->write_idx + 1) % CAN_LOG_MAX_FRAMES;
   if (can_log->count < CAN_LOG_MAX_FRAMES) {
      can_log->count++;
   } else {
      can_log->read_idx = (can_log->read_idx + 1) % CAN_LOG_MAX_FRAMES;
      can_log->overflow_count++;
   }
}

int CanLogRead(CanFrame_t *frame) {
   if (!can_log || can_log->count == 0) return 0;

   *frame = can_log->frames[can_log->read_idx];
   can_log->read_idx = (can_log->read_idx + 1) % CAN_LOG_MAX_FRAMES;
   can_log->count--;
   return 1;
}

uint32_t CanLogCount(void) {
   return can_log ? can_log->count : 0;
}

CanLog_t *CanLogGetPtr(void) {
   return can_log;
}

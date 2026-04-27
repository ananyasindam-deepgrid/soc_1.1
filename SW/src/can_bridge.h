#ifndef _CAN_BRIDGE_H_
#define _CAN_BRIDGE_H_

#include <stdint.h>

// WS-TTL-CAN Format Conversion mode: fixed 13-byte frame
// Byte 0:    Frame Info (FF, RTR, DLC)
// Bytes 1-4: CAN ID (big-endian)
// Bytes 5-12: Data (up to 8 bytes, padded with 0)

#define CAN_FRAME_SIZE 13

// DDR ring buffer for CAN data logging
#define CAN_LOG_MAX_FRAMES 1024

typedef struct {
   uint8_t  frame_info;   // Bit7=FF(ext/std), Bit6=RTR, Bits3:0=DLC
   uint32_t can_id;       // 29-bit extended or 11-bit standard
   uint8_t  data[8];
   uint8_t  dlc;          // data length (0-8)
} CanFrame_t;

typedef struct {
   uint32_t    write_idx;
   uint32_t    read_idx;
   uint32_t    count;
   uint32_t    overflow_count;
   CanFrame_t  frames[CAN_LOG_MAX_FRAMES];
} CanLog_t;

#ifdef __cplusplus
extern "C" {
#endif

// Initialize CAN bridge (allocates DDR log buffer)
void CanBridgeInit(void);

// Send one CAN frame via UART1 to WS-TTL-CAN module
void CanSend(const CanFrame_t *frame);

// Non-blocking: returns 1 if a complete 13-byte frame is available
int CanReceiveAvailable(void);

// Receive one CAN frame (returns 1 on success, 0 if no frame ready)
int CanReceive(CanFrame_t *frame);

// Log a received frame to DDR ring buffer
void CanLogFrame(const CanFrame_t *frame);

// Read a frame from the DDR log (returns 1 on success)
int CanLogRead(CanFrame_t *frame);

// Get number of frames in log
uint32_t CanLogCount(void);

// Get pointer to the log structure (for DDR inspection)
CanLog_t *CanLogGetPtr(void);

#ifdef __cplusplus
}
#endif

#endif

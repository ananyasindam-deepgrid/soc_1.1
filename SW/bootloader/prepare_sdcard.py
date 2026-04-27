#!/usr/bin/env python3
"""
Prepare firmware binary for SD card booting.

This script takes a raw firmware .bin file and prepends a 16-byte header
that the bootloader expects:
  Bytes 0-3:   Magic "ZTFI" (0x5A544649)
  Bytes 4-7:   Firmware size in bytes (little-endian)
  Bytes 8-11:  Load address (0x00004000)
  Bytes 12-15: Entry address (0x00004000)

Usage:
  python prepare_sdcard.py <input.bin> <output.img>

Then write output.img to SD card:
  Linux:   dd if=output.img of=/dev/sdX bs=512
  Windows: Use Win32DiskImager or similar tool to write output.img to SD card
"""

import sys
import struct
import os

MAGIC = 0x5A544649  # "ZTFI"
LOAD_ADDR = 0x00004000
ENTRY_ADDR = 0x00004000

def main():
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <input.bin> <output.img>")
        print(f"  input.bin:  Raw firmware binary (from objcopy -O binary)")
        print(f"  output.img: Output image to write to SD card")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2]

    with open(input_path, 'rb') as f:
        firmware = f.read()

    fw_size = len(firmware)
    print(f"Firmware size: {fw_size} bytes ({fw_size/1024:.1f} KB)")
    print(f"Load address:  0x{LOAD_ADDR:08X}")
    print(f"Entry address: 0x{ENTRY_ADDR:08X}")

    # Build header (16 bytes)
    header = struct.pack('<IIII', MAGIC, fw_size, LOAD_ADDR, ENTRY_ADDR)

    # Combine header + firmware
    image = header + firmware

    # Pad to sector boundary (512 bytes)
    pad_size = (512 - (len(image) % 512)) % 512
    image += b'\x00' * pad_size

    sectors = len(image) // 512
    print(f"Image size:    {len(image)} bytes ({sectors} sectors)")

    with open(output_path, 'wb') as f:
        f.write(image)

    print(f"Written to:    {output_path}")
    print(f"\nTo write to SD card:")
    print(f"  Linux:   sudo dd if={output_path} of=/dev/sdX bs=512")
    print(f"  Windows: Use Win32DiskImager to write {output_path}")

if __name__ == '__main__':
    main()

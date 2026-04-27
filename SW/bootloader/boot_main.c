/*
 * Minimal SD Card Bootloader for dgrid
 *
 * This bootloader lives entirely in TCM (16KB BRAM).
 * On power-up, it:
 *   1. Waits for DDR calibration (~500ms delay)
 *   2. Initializes SD card via SPI GPIO bit-banging
 *   3. Reads firmware binary from SD card (raw sectors starting at sector 0)
 *   4. Copies firmware to DDR at 0x4000 (the RAM origin in linker.ld)
 *   5. Returns to boot_crt.S which jumps to 0x4000
 *
 * The firmware binary on SD card is created by:
 *   riscv32-unknown-elf-objcopy -O binary --only-section=.bss \
 *     --only-section=.data --only-section=.rodata --only-section=.memory \
 *     --only-section=.ctors --only-section=._user_heap \
 *     dgrid.elf firmware.bin
 *
 * Or more simply, use the .bin output and write raw to SD card:
 *   dd if=firmware.bin of=/dev/sdX bs=512
 */

#include <stdint.h>

/* APB register access (same as soc.h) */
#define APB ((volatile unsigned int *)0xC0000000)
#define APB_LED  0
#define APB_PB   2
#define APB_TIME 19

/* SPI bit-bang via GPIO (same as sdmm.c) */
static uint32_t SPI_STATE = 0;

#define SCK_HIGH()   do { SPI_STATE |=  0x20; APB[APB_LED] = SPI_STATE; } while(0)
#define SCK_LOW()    do { SPI_STATE &= ~0x20; APB[APB_LED] = SPI_STATE; } while(0)
#define MOSI_HIGH()  do { SPI_STATE |=  0x40; APB[APB_LED] = SPI_STATE; } while(0)
#define MOSI_LOW()   do { SPI_STATE &= ~0x40; APB[APB_LED] = SPI_STATE; } while(0)
#define CS_HIGH()    do { SPI_STATE |=  0x80; APB[APB_LED] = SPI_STATE; } while(0)
#define CS_LOW()     do { SPI_STATE &= ~0x80; APB[APB_LED] = SPI_STATE; } while(0)
#define MISO_READ()  ((APB[APB_PB] & 0x10) ? 1 : 0)

/* LED indicators */
#define LED_ON(n)    do { SPI_STATE |= (1 << (n)); APB[APB_LED] = SPI_STATE; } while(0)
#define LED_OFF(n)   do { SPI_STATE &= ~(1 << (n)); APB[APB_LED] = SPI_STATE; } while(0)

/* Firmware is loaded to this DDR address (must match linker.ld RAM origin) */
#define FW_LOAD_ADDR    0x00004000
#define FW_MAX_SECTORS  2048    /* 2048 * 512 = 1MB max firmware */
#define SECTOR_SIZE     512

/* SD card commands */
#define CMD0    0
#define CMD8    8
#define CMD16   16
#define CMD17   17
#define CMD55   55
#define ACMD41  41
#define CMD58   58

static void delay_us(uint32_t us) {
    volatile uint32_t start = APB[APB_TIME];
    while ((APB[APB_TIME] - start) < us);
}

static void delay_ms(uint32_t ms) {
    delay_us(ms * 1000);
}

/* SPI byte transfer (bit-bang, MSB first) */
static uint8_t spi_xfer(uint8_t data) {
    uint8_t recv = 0;
    for (int i = 7; i >= 0; i--) {
        if (data & (1 << i))
            MOSI_HIGH();
        else
            MOSI_LOW();
        SCK_HIGH();
        recv = (recv << 1) | MISO_READ();
        SCK_LOW();
    }
    return recv;
}

/* Send SD command, return R1 response */
static uint8_t sd_cmd(uint8_t cmd, uint32_t arg) {
    uint8_t r;

    if (cmd & 0x80) {
        /* ACMD: send CMD55 first */
        cmd &= 0x7F;
        r = sd_cmd(CMD55, 0);
        if (r > 1) return r;
    }

    CS_HIGH();
    spi_xfer(0xFF);
    CS_LOW();
    spi_xfer(0xFF);

    spi_xfer(0x40 | cmd);
    spi_xfer((arg >> 24) & 0xFF);
    spi_xfer((arg >> 16) & 0xFF);
    spi_xfer((arg >>  8) & 0xFF);
    spi_xfer((arg      ) & 0xFF);

    /* CRC */
    if (cmd == CMD0) spi_xfer(0x95);
    else if (cmd == CMD8) spi_xfer(0x87);
    else spi_xfer(0x01);

    /* Wait for response (up to 10 bytes) */
    for (int i = 0; i < 10; i++) {
        r = spi_xfer(0xFF);
        if (!(r & 0x80)) break;
    }
    return r;
}

/* Initialize SD card in SPI mode. Returns 0 on success. */
static int sd_init(void) {
    uint8_t r;
    int timeout;

    CS_HIGH();
    /* Send 80+ clock pulses with CS high */
    for (int i = 0; i < 10; i++)
        spi_xfer(0xFF);

    /* CMD0: GO_IDLE_STATE */
    timeout = 100;
    do {
        r = sd_cmd(CMD0, 0);
    } while (r != 0x01 && --timeout);
    if (timeout == 0) return -1;

    /* CMD8: SEND_IF_COND (voltage check for SDv2) */
    r = sd_cmd(CMD8, 0x1AA);
    if (r == 0x01) {
        /* SDv2 card - read 4 bytes of R7 response */
        spi_xfer(0xFF); spi_xfer(0xFF);
        spi_xfer(0xFF); spi_xfer(0xFF);

        /* ACMD41: SEND_OP_COND with HCS */
        timeout = 1000;
        do {
            delay_ms(1);
            r = sd_cmd(0x80 | ACMD41, 0x40000000);
        } while (r != 0x00 && --timeout);
        if (timeout == 0) return -2;
    } else {
        /* SDv1 or MMC */
        timeout = 1000;
        do {
            delay_ms(1);
            r = sd_cmd(0x80 | ACMD41, 0);
        } while (r != 0x00 && --timeout);
        if (timeout == 0) return -3;
    }

    /* CMD16: SET_BLOCKLEN to 512 */
    r = sd_cmd(CMD16, SECTOR_SIZE);
    if (r != 0x00) return -4;

    return 0;
}

/* Read one 512-byte sector from SD card. Returns 0 on success. */
static int sd_read_sector(uint32_t sector, uint8_t *buf) {
    uint8_t r;
    int timeout;

    /* CMD17: READ_SINGLE_BLOCK (byte address for standard SD) */
    r = sd_cmd(CMD17, sector * SECTOR_SIZE);
    if (r != 0x00) return -1;

    /* Wait for data token 0xFE */
    timeout = 50000;
    do {
        r = spi_xfer(0xFF);
    } while (r == 0xFF && --timeout);
    if (r != 0xFE) return -2;

    /* Read 512 bytes */
    for (int i = 0; i < SECTOR_SIZE; i++) {
        buf[i] = spi_xfer(0xFF);
    }

    /* Discard 2-byte CRC */
    spi_xfer(0xFF);
    spi_xfer(0xFF);

    CS_HIGH();
    spi_xfer(0xFF);

    return 0;
}

/*
 * Bootloader main entry point.
 *
 * Firmware binary format on SD card (raw sectors starting at sector 0):
 *   Sector 0 (bytes 0-3):   Magic number 0x5A544649 ("ZTFI")
 *   Sector 0 (bytes 4-7):   Firmware size in bytes (little-endian)
 *   Sector 0 (bytes 8-11):  Load address (should be 0x00004000)
 *   Sector 0 (bytes 12-15): Entry address (should be 0x00004000)
 *   Sector 0 (bytes 16-511): First 496 bytes of firmware
 *   Sector 1+: Remaining firmware data
 */
void boot_main(void) {
    uint8_t sector_buf[SECTOR_SIZE];
    uint32_t fw_size, load_addr;
    uint32_t sectors_needed, bytes_loaded;
    uint8_t *dst;

    /* LED[0] on: bootloader started */
    LED_ON(0);

    /* Wait for DDR calibration (~500ms) */
    delay_ms(500);

    /* LED[1] on: DDR should be ready */
    LED_ON(1);

    /* Initialize SD card */
    if (sd_init() != 0) {
        /* SD init failed - blink LED[3] forever */
        for (;;) {
            LED_ON(3);
            delay_ms(200);
            LED_OFF(3);
            delay_ms(200);
        }
    }

    /* LED[2] on: SD card initialized */
    LED_ON(2);

    /* Read sector 0 (firmware header) */
    if (sd_read_sector(0, sector_buf) != 0) {
        /* Read failed */
        for (;;) {
            LED_ON(3); LED_ON(2);
            delay_ms(200);
            LED_OFF(3); LED_OFF(2);
            delay_ms(200);
        }
    }

    /* Parse header */
    uint32_t magic = *(uint32_t *)&sector_buf[0];
    if (magic != 0x5A544649) {  /* "ZTFI" */
        /* Bad magic - blink all LEDs */
        for (;;) {
            APB[APB_LED] = 0x0F;
            delay_ms(100);
            APB[APB_LED] = 0x00;
            delay_ms(100);
        }
    }

    fw_size   = *(uint32_t *)&sector_buf[4];
    load_addr = *(uint32_t *)&sector_buf[8];
    /* entry_addr = *(uint32_t *)&sector_buf[12]; -- not used, we jump from boot_crt.S */

    if (fw_size > FW_MAX_SECTORS * SECTOR_SIZE) {
        fw_size = FW_MAX_SECTORS * SECTOR_SIZE;
    }

    /* Copy first payload from sector 0 (bytes 16-511) */
    dst = (uint8_t *)load_addr;
    uint32_t first_chunk = SECTOR_SIZE - 16;
    if (first_chunk > fw_size) first_chunk = fw_size;
    for (uint32_t i = 0; i < first_chunk; i++) {
        dst[i] = sector_buf[16 + i];
    }
    bytes_loaded = first_chunk;

    /* Read remaining sectors */
    uint32_t sector = 1;
    while (bytes_loaded < fw_size) {
        if (sd_read_sector(sector, sector_buf) != 0) {
            break;  /* Read error, use what we have */
        }
        uint32_t chunk = fw_size - bytes_loaded;
        if (chunk > SECTOR_SIZE) chunk = SECTOR_SIZE;
        for (uint32_t i = 0; i < chunk; i++) {
            dst[bytes_loaded + i] = sector_buf[i];
        }
        bytes_loaded += chunk;
        sector++;

        /* Toggle LED[0] for progress */
        if (sector & 1) LED_ON(0); else LED_OFF(0);
    }

    /* All LEDs on briefly to indicate success */
    APB[APB_LED] = 0x0F;
    delay_ms(200);
    APB[APB_LED] = 0x00;

    /* Return to boot_crt.S which jumps to 0x4000 */
}

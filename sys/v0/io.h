#ifndef __V0_IO_H__
#define __V0_IO_H__

#define V0_NET_ADR_LEN  32

#include <v0/types.h>

#define V0_IO_UX_PERM   (1 << 0)        // user execute permission bit
#define V0_IO_UW_PERM   (1 << 1)        // user write
#define V0_IO_UR_PERM   (1 << 2)        // user read
#define V0_IO_SX_PERM   (1 << 3)        // superuser execute
#define V0_IO_SW_PERM   (1 << 4)        // superuser write
#define V0_IO_SR_PERM   (1 << 5)        // superuser read

#define V0_IO_SYNC      (1 << 0)        // synchronous I/O mode
#define V0_IO_DMA       (1 << 1)        // DMA direct memory transfers
#define V0_IO_POLL      (1 << 2)        // polled I/O devices/files
#define V0_IO_REALTIME  (1 << 3)        // realtime/deadline I/O scheduler

#define V0_IO_CONF_BUS        0
#define V0_IO_INIT_BUS        1
#define V0_IO_CONF_DEV        2
#define V0_IO_INIT_DEV        3
#define V0_IO_OPEN_DEV        4
#define V0_IO_SEEK_DEV        5
#define V0_IO_READ_DEV        6
#define V0_IO_WRITE_DEV       7
#define V0_IO_READ_BUF        8
#define V0_IO_WRITE_BUF       9
#define V0_IO_READ_BYTE       10
#define V0_IO_READ_HALF       11
#define V0_IO_READ_WORD       12
#define V0_IO_WRITE_BYTE      13
#define V0_IO_WRITE_HALF      14
#define V0_IO_WRITE_WORD      15

/* I/O control register IDs */
#define V0_PIC_REG      0x00    // programmable interrupt controller
#define V0_TMR_REG      0x01    // timer configuration
#define V0_RTC_REG      0x02    // real-time clock
#define V0_KBD_REG      0x03    // keyboard configuration
#define V0_MOUSE_REG    0x04    // mouse configuration
#define V0_DISPLAY_REG  0x05    // display configuration
#define V0_BUS_REG      0x06    // bus configuration
#define V0_DEV_REG      0x07    // device configuration
/* I/O device classes */
#define V0_NO_DEV       0x00    // invalid/unset device
#define V0_TIME_DEV     0x01    // [interrupt] timers
#define V0_AUDIO_DEV    0x02    // audio device
#define V0_VIDEO_DEV    0x03    // video device
#define V0_HID_DEV      0x04    // human interface device
#define V0_MEDIA_DEV    0x05    // media [such as disk] controller
#define V0_NET_DEV      0x06    // network interfaces

/* prec-member */
#define V0_TIME_NANO    1000000000
#define V0_TIME_MICRO   1000000
#define V0_TIME_MILLI   1000
/* flg-member */
#define V0_TIME_SYNC    (1 << 0)
#define V0_TIME_UTC     (1 << 1)
struct v0timedev {
    long        prec;           // precision/frequency in Hz
    long        drift;          // clock drift if applicable
    long        bus;            // bus ID
    long        dev;            // device ID
    long        flg;            // device flag-bits
};

#define V0_TMR_HZ       250
#define V0_TMR_SLICE_MS 4

/* adrtype-member */
#define V0_NET_IPV4_ADR 0x00
#define V0_NET_IPV6_ADR 0x01
struct v0netdev {
    uint8_t     adr[V0_NET_ADR_LEN]; // interface network address such as IP
    long        adrsize;        // size of address structure in bytes
    long        adrtype;        // address family (IPv4, IPv6, ...)
    long        type;           // interface type
    long        flg;            // permissions + interface-specific flag-bits
    long        irq;            // interrupt-request channel
    long        dma;            // direct memory access channel
};

#endif /* __V0_IO_H__ */


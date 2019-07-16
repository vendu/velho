#ifndef __V0_SYS_H__
#define __V0_SYS_H__

#include <v0/types.h>

/* prec-member */
#define V0_TIME_NANO            1000000000L
#define V0_TIME_MICRO           1000000L
#define V0_TIME_MILLI           1000L
/* flg-member */
#define V0_TIME_SYNC            (1L << 0)
#define V0_TIME_UTC             (1L << 1)

#define V0_DEV_FLAG_BITS        16L
#define V0_BUS_BITS             8L
#define V0_DEV_BITS             8L
#define v0busid()               ((dev) >> V0_DEV_BITS)
#define v0devid(dev)            ((dev) & 0x00ffffffL)
struct v0timedev {
    long        prec;           // precision/frequency in Hz
    long        drift;          // clock drift if applicable
    long        dev;            // bus + device IDs
    long        flg;            // device flag-bits
};

/* IOC-commands */
#define V0_IO_CONF_BUS          0
#define V0_IO_INIT_BUS          1
#define V0_IO_CONF_DEV          2
#define V0_IO_INIT_DEV          3
#define V0_IO_OPEN_DEV          4
#define V0_IO_SEEK_DEV          5
#define V0_IO_READ_DEV          6
#define V0_IO_WRITE_DEV         7
#define V0_IO_SYNC__BUF         8
#define V0_IO_FLUSH_BUF         9

/* I/O permission-bits */
#define V0_IO_RAW               (1L << 31)     // raw/character-based I/O
#define V0_IO_BLK               (1L << 30)     // buffered block-based I/O
#define V0_IO_NO_BUF            (1L << 29)     // do not cache blocks
#define V0_IO_DIRECT            (1L << 28)     // direct memory access
#define V0_IO_SYNC              (1L << 27)     // synchronous I/O
#define V0_IO_POLL              (1L << 26)     // pollable object
#define V0_IO_BUSY              (1L << 25)     // I/O in progress
#define V0_IO_SIGIO             (1L << 24)     // send SIGIO on objects ready
#define V0_IO_SIGURG            (1L << 23)     // send SIGURG for ZEN_IO_SYNC
#define V0_IO_UR_PERM   	(1L << 8)      // user read permission bit
#define V0_IO_UW_PERM   	(1L << 7)      // user write
#define V0_IO_UX_PERM           (1L << 6)      // user execute
#define V0_IO_GR_PERM   	(1L << 5)      // group read
#define V0_IO_GW_PERM   	(1L << 4)      // group write
#define V0_IO_GX_PERM   	(1L << 3)      // group execute
#define V0_IO_AR_PERM   	(1L << 2)      // world read
#define V0_IO_AW_PERM   	(1L << 1)      // world write
#define V0_IO_AX_PERM   	(1L << 0)      // world execute

/* I/O control register IDs */
#define V0_PIC_REG      	0x00    // programmable interrupt controller
#define V0_TMR_REG      	0x01    // timer configuration
#define V0_RTC_REG      	0x02    // real-time clock
#define V0_KBD_REG      	0x03    // keyboard configuration
#define V0_MOUSE_REG    	0x04    // mouse configuration
#define V0_DISPLAY_REG  	0x05    // display configuration
#define V0_BUS_REG      	0x06    // bus configuration
#define V0_DEV_REG      	0x07    // device configuration
/* I/O device classes */
#define V0_NO_DEV       	0x00    // invalid/unset device
#define V0_TIME_DEV     	0x01    // [interrupt] timers
#define V0_AUDIO_DEV    	0x02    // audio device
#define V0_VIDEO_DEV    	0x03    // video device
#define V0_HID_DEV      	0x04    // human interface device
#define V0_MEDIA_DEV    	0x05    // media [such as disk] controller
#define V0_NET_DEV      	0x06    // network interfaces

#define V0_NET_ADR_LEN          32

/* adrtype-member */
#define V0_NET_IPV4_ADR 	0x00
#define V0_NET_IPV6_ADR 	0x01
struct v0netdev {
    uint8_t     adr[V0_NET_ADR_LEN]; // interface network address such as IP
    long        adrsize;        // size of address structure in bytes
    long        adrtype;        // address family (IPv4, IPv6, ...)
    long        type;           // interface type
    long        flg;            // permissions + interface-specific flag-bits
    long        irq;            // interrupt-request channel
    long        dma;            // direct memory access channel
};

#endif /* __V0_SYS_H__ */


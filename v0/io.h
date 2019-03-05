#ifndef __V0_IO_H__
#define __V0_IO_H__

#include <v0/types.h>

/* I/O control register IDs */
#define V0_PIC_REG      0x00     // programmable interrupt controller
#define V0_TMR_REG      0x01     // timer configuration
#define V0_RTC_REG      0x02     // real-time clock
#define V0_KBD_REG      0x03     // keyboard configuration
#define V0_MOUSE_REG    0x04     // mouse configuration
#define V0_DISPLAY_REG  0x05     // display configuration
#define V0_BUS_REG      0x06     // bus configuration
#define V0_DEV_REG      0x07     // device configuration
/* I/O device classes */
#define V0_NO_DEV       0x00     // invalid/unset device
#define V0_AUDIO_DEV    0x01     // audio device
#define V0_VIDEO_DEV    0x02     // video device
#define V0_HID_DEV      0x03     // human interface device
#define V0_MEDIA_DEV    0x04     // media [such as disk] controller
#define V0_NETWORK_DEV  0x05     // network interfaces
#define V0_TIME_DEV     0x06     // clock; nano, micro, milli, second

struct v0networkdev {
    uint8_t adr[32];            // interface network address such as IP
    long type;                  // interface type
    long adrtype;               // address family (IPv4, IPv6, ...)
    long flg;                   // interface-specific flag-bits
};

#define V0_TIME_NANO    1000000000
#define V0_TIME_MICRO   1000000
#define V0_TIME_MILLI   1000
struct v0timedev {
    long prec;                  // precision/frequency in Hz
    long drift;                 // clock drift if applicable
    long bus;                   // bus ID
    long dev;                   // device ID
};

#define V0_TMR_HZ       250
#define V0_TMR_SLICE_MS 4

#endif /* __V0_IO_H__ */


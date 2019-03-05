#ifndef __V0_IO_H__
#define __V0_IO_H__

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

/* values for the flg-member to denote presence of parameters */
#define V0_IO_DEV_BIT   (1U << 0)
#define V0_IO_BUS_BIT   (1U << 1)
#define V0_IO_PERM_BIT  (1U << 2)
#define V0_IO_BASE_BIT  (1U << 3)
#define V0_IO_SIZE_BIT  (1U << 4)

#define V0_IO_DESC_SIZE (8 * V0_WORD_SIZE)
struct v0i0desc {
    v0reg  flg;         // flags listing parameters present
    v0reg  dev;         // device ID
    v0reg  bus;         // bus ID
    v0reg  perm;        // I/O-permission bits
    v0ureg base;        // I/O-map such as framebuffer base address
    v0ureg buf;         // I/O read buffer base address
    v0ureg size;        // I/O-map size in bytes
    v0reg  _pad;        // pad to boundary of 8 v0regs
};

struct v0iofuncs {
    int     *creat(const char *path, mode_t mode);
    int     *open(const char *path, int flg, ...);
    int     *close(int fd);
    off_t   *lseek(int fd, off_t ofs, int whence);
    int      link(const char *src, const char *dest);
    int      wsymlink(const char *src, const char *dest);
    int      unlink(const char *path);
    ssize_t *read(int fd, void *buf, size_t nb);
    ssize_t *write(int fd, void *buf, size_t nb);
    void    *mmap(void *adr, size_t nb, int prot, int flg, int fd, off_t ofs);

};

#endif /* __V0_IO_H__ */


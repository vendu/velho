#ifndef __ZEN_IO_H__
#define __ZEN_IO_H__

#define V0_IO_SYNC      (1 << 0)        // synchronous I/O mode
#define V0_IO_DMA       (1 << 1)        // DMA direct memory transfers
#define V0_IO_POLL      (1 << 2)        // polled I/O devices/files
#define V0_IO_REALTIME  (1 << 3)        // realtime/deadline I/O scheduler

struct zenioblkdev {
    struct zenvfsfuncs *funcs;
    uint32_t            dev;    // 16-bit major + 16-bit minor device IDs
    uint16_t            bus;
    uint16_t            flg;
};

struct zeniodesc {
    m_adr_t    buf;             // buffer [page] address
    m_adr_t    ofs;             // buffer offset
    uint32_t   flg;
};

};

#endif /* __ZEN_IO_H__ */


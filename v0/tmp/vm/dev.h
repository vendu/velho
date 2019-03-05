#ifndef __V0_VM_DEV_H__
#define __V0_VM_DEV_H__

#define V0_TMR_HIRES    0
#define V0_TMR_INTERVAL 1
#define V0_HIRES_TMR    (1 << V0_TMR_HIRES)
#define V0_INTERVAL_TMR (1 << V0_TMR_INTERVAL)

struct v0tmr {
    unsigned long mode;
    unsigned long freq;
};

#endif /* __V0_VM_DEV_H__ */


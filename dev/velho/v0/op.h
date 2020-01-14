#ifndef __VO_OP_H__
#define __VO_OP_H__

#define opgetunit(op)   ((op)->info & 0x07)
#define opgetimst(op)   (((op)->info >> 3) & 0x07)
#define opgetflg(op)    ((op)->info >> 6)
#define opgetreg0(op)   ((op)->parm & 0x0f)
#define opgetreg1(op)   ((op)->parm & 0x0f)
#define opgetarg8(op)   ((op)->args & 0xff)
#define opgetimm8(op)   ((op)->parm)
#define opgetarg16(op)  ((op)->args)
#define opgetarg32(op)         \
    ((int32_t)((op)->args[0])| ((int32_t)((op)->args)[1]))

struct op {
    uint8_t     code;
    uint8_t	parm;
    int16_t     args[VLA];	 
};

#endif /* __VO_OP_H__ *{


#ifndef __ZERO_QRAND_H__
#define __ZERO_QRAND_H__

#include <stdint.h>

int32_t qrand32(void);
int32_t qrand32_r(int32_t *seed);
float   qrandf(int32_t *seed);

#endif /* __ZERO_QRAND_H__ */


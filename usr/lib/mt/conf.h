#ifndef __MT_CONF_H_
#define __MT_CONF_H_

#define POSIX_THREAD 0
#define ZERO_THREAD  1
#define ZERO_MUTEX   1
#if !defined(PTHREAD)
#define PTHREAD 0
#endif

#if (ZERO_MUTEX)
#include <mt/mtx.h>
#endif
#if (ZERO_THREAD)
#include <mt/thr.h>
#endif
#if (POSIX_THREAD)
#include <pthread.h>
#endif

#endif /* __MT_CONF_H_ */


#ifndef __ZEN_TYPE_H__
#define __ZEN_TYPE_H__

#include <stddef.h>
#include <stdint.h>
#if defined(__v0__)
#include <zen/sys/v0.h>
#endif

typedef intmax_t        zenlong;        // system maximum
typedef uintmax_t       zenulong;       // unsigned system maximum
typedef int32_t         zenid_t;        // generic object ID
typedef uint32_t        zendev_t;       // device type
typedef int64_t         zenoff_t;       // file-system offset
typedef int32_t         zenuid_t;       // user ID
typedef int32_t         zengid_t;       // group ID
typedef uint32_t        zenperm_t;      // I/O permission flags

#endif /* __ZEN_TYPE_H__ */


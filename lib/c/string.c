#include <features.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#if defined(_GNU_SOURCE)
#include <alloca.h>
#endif
#include <zero/cdefs.h>
#include <zero/trix.h>
#include <mach/param.h>

#if defined(__GLIBC__)

#undef strdupa
#undef strndupa
#undef mempcpy
#undef __mempcpy

#else

extern const int8_t stringcolltab_c_en_US[256];

//static THREADLOCAL struct _string  _string;
static const  int8_t  *colltab = stringcolltab_c_en_US;
static const  int8_t  *localecolltab = stringcolltab_c_en_US;
const unsigned char    *collnametab[STRING_LANGS]
= {
    (unsigned char *)"en_US"
};

#endif /* __GLIBC__ */

#if 0
/* TESTED OK */
void *
memcpy(void *dest,
       const void *src,
       size_t n)
{
    long       *lptr1;
    long       *lptr2;
    int8_t     *i8ptr1;
    int8_t     *i8ptr2;
    size_t       cnt;
    size_t       nleft;
    size_t       val;

    if (!n) {

        return dest;
    }
    nleft = n;
    u8ptr1 = (void *)src;
    u8ptr2 = dest;
    val = sizeof(long);
    if (nleft > (val << 3)
        && (((uintptr_t)u8ptr1 & (val - 1))
            == ((uintptr_t)u8ptr2 & (val - 1)))) {
        cnt = (uintptr_t)u8ptr1 & (val - 1);
        if (cnt) {
            cnt = val - cnt;
            nleft -= cnt;
            while (cnt--) {
                *i8ptr2++ = *i8ptr1++;
            }
        }
        val = LONGSIZELOG2 + 3;
        lptr2 = (long *)u8ptr2;
        cnt = nleft >> val;
        lptr1 = (long *)u8ptr1;
        nleft -= cnt << val;
        val = 8;
        while (cnt--) {
            lptr2[0] = lptr1[0];
            lptr2[1] = lptr1[1];
            lptr2[2] = lptr1[2];
            lptr2[3] = lptr1[3];
            lptr2[4] = lptr1[4];
            lptr2[5] = lptr1[5];
            lptr2[6] = lptr1[6];
            lptr2[7] = lptr1[7];
            lptr2 += val;
            lptr1 += val;
        }
        u8ptr2 = (int8_t *)lptr2;
        u8ptr1 = (int8_t *)lptr1;
    }
    while (nleft--) {
        *i8ptr2++ = *i8ptr1++;
    }

    return dest;
}
#endif

/* TESTED OK */
static void *
_memcpybk(void *dest, const void *src, size_t len)
{
    int8_t     *bsrc = (int8_t *)src + len;
    int8_t     *bdest = (int8_t *)dest + len;
    size_t      nb = len;
    size_t      nw;
    long       *lsrc;
    long       *ldest;
    size_t      n;

    n = (uintptr_t)bdest & (sizeof(long) - 1);
    if (n == ((uintptr_t)bsrc & (sizeof(long) - 1))) {
        n = sizeof(long) - n;
        n = min(n, nb);
        nb -= n;
        if (n) {
            switch (n) {
                case 8:
                    bdest[-1] = bsrc[-1];
                case 7:
                    bdest[-2] = bsrc[-2];
                case 6:
                    bdest[-3] = bsrc[-3];
                case 5:
                    bdest[-4] = bsrc[-4];
                case 4:
                    bdest[-5] = bsrc[-5];
                case 3:
                    bdest[-6] = bsrc[-6];
                case 2:
                    bdest[-7] = bsrc[-7];
                case 1:
                    bdest[-8] = bsrc[-8];
                case 0:

                    break;
            }
            bdest -= n;
            bsrc -= n;
        }
        nw = nb / sizeof(long);
        if (nw) {
            ldest = (long *)bdest;
            lsrc = (long *)bsrc;
            nb -= nw * sizeof(long);
            while (nw--) {
                n = min(nw, 8);
                switch (n) {
                    case 8:
                        ldest[-1] = lsrc[-1];
                    case 7:
                        ldest[-2] = lsrc[-2];
                    case 6:
                        ldest[-3] = lsrc[-3];
                    case 5:
                        ldest[-4] = lsrc[-4];
                    case 4:
                        ldest[-5] = lsrc[-5];
                    case 3:
                        ldest[-6] = lsrc[-6];
                    case 2:
                        ldest[-7] = lsrc[-7];
                    case 1:
                        ldest[-8] = lsrc[-8];
                    case 0:

                        break;
                }
                nw -= n;
                ldest -= n;
                lsrc -= n;
            }
            bdest = (int8_t *)ldest;
            bsrc = (int8_t *)lsrc;
        }
    }
    while (nb) {
        n = min(nb, 8);
        switch (n) {
            case 8:
                bdest[-1] = bsrc[-1];
            case 7:
                bdest[-2] = bsrc[-2];
            case 6:
                bdest[-3] = bsrc[-3];
            case 5:
                bdest[-4] = bsrc[-4];
            case 4:
                bdest[-5] = bsrc[-5];
            case 3:
                bdest[-6] = bsrc[-6];
            case 2:
                bdest[-7] = bsrc[-7];
            case 1:
                bdest[-8] = bsrc[-8];
            case 0:

                break;
        }
        nb -= n;
        bdest -= n;
        bsrc -= n;
    }

    return dest;
}

#if 0
/* TESTED OK */
static void *
_memcpybk(void *dest,
          const void *src,
          size_t n)
{
    long               *lptr1;
    long               *lptr2;
    const int8_t       *i8ptr1;
    int8_t             *i8ptr2;
    size_t              cnt;
    size_t              nleft;
    size_t              val;

    if (!n) {

        return dest;
    }
    nleft = n;
    i8ptr1 = src;
    i8ptr2 = dest;
    i8ptr1 += n;
    i8ptr2 += n;
    val = sizeof(long);
    if (labs(i8ptr1 - i8ptr2) >= (long)(val << 3)
        && nleft >= (val << 3)
        && (((uintptr_t)i8ptr1 & (val - 1))
            == ((uintptr_t)i8ptr2 & (val - 1)))) {
        cnt = (uintptr_t)i8ptr1 & (val - 1);
        if (cnt) {
            //            cnt = val - cnt;
            nleft -= cnt;
            while (cnt--) {
                *--i8ptr2 = *--i8ptr1;
            }
        }
        val = LONGSIZELOG2 + 3;
        lptr2 = (long *)i8ptr2;
        cnt = nleft >> val;
        lptr1 = (long *)i8ptr1;
        nleft -= cnt << val;
        val = 8;
        while (cnt--) {
            lptr2 -= val;
            lptr1 -= val;
            lptr2[0] = lptr1[0];
            lptr2[1] = lptr1[1];
            lptr2[2] = lptr1[2];
            lptr2[3] = lptr1[3];
            lptr2[4] = lptr1[4];
            lptr2[5] = lptr1[5];
            lptr2[6] = lptr1[6];
            lptr2[7] = lptr1[7];
        }
        i8ptr2 = (int8_t *)lptr2;
        i8ptr1 = (int8_t *)lptr1;
    }
    while (nleft--) {
        *--i8ptr2 = *--i8ptr1;
    }

    return dest;
}
#endif

/* TESTED OK */
void *
memmove(void *RESTRICT dest,
        const void *RESTRICT src,
        size_t n)
{
    if (!n) {

        return dest;
    }
    if ((uintptr_t)dest < (uintptr_t)src
        || (uintptr_t)src + n < (uintptr_t)dest) {
        memcpy(dest, src, n);
    } else {
        _memcpybk(dest, src, n);
    }

    return dest;
}

/* TESTED OK */
char *
strcpy(char *dest,
       const char *RESTRICT src)
{
    char *RESTRICT      cptr = dest;

    while (*src) {
        *cptr++ = *src++;
    }
    *cptr = *src;

    return dest;
}

/* TESTED OK */
char *
strncpy(char *dest,
        const char *src,
        size_t n)
{
    char *RESTRICT      cptr = dest;

    if (n) {
        while ((*src) && (n--)) {
            *cptr++ = *src++;
        }
        if (n) {
            *cptr = *src;
        }
    }

    return dest;
}

/* TESTED OK */
char *
strcat(char *RESTRICT dest,
       const char *src)
{
    char *RESTRICT      cptr = dest;

    while (*cptr) {
        cptr++;
    }
    while (*src) {
        *cptr++ = *src++;
    }
    *cptr = *src;

    return dest;
}

/* TESTED OK */
char *
strncat(char *dest,
        const char *src,
        size_t n)
{
    char *RESTRICT 	cptr = dest;

    if (n) {
        while (*cptr) {
            cptr++;
        }
        while ((*src) && (n--)) {
            *cptr++ = *src++;
        }
        if (n) {
            *cptr = *src;
        }
    }

    return dest;
}

/* TODO: optimise the *cmp() functions with word-size access */

/* TESTED OK */
PURE int
memcmp(const void *ptr1,
       const void *ptr2,
       size_t len)
{
    const unsigned char *ucptr1 = ptr1;
    const unsigned char *ucptr2 = ptr2;
    size_t               nb = len;
    size_t               nw;
    const unsigned long *ulptr1;
    const unsigned long *ulptr2;
    int                  byte;
    size_t               n;

    if (!len) {

        return 0;
    }
    n = (uintptr_t)ucptr1 & (sizeof(long) - 1);
    if (n == ((uintptr_t)ucptr2 & (sizeof(long) - 1))) {
        n = sizeof(long) - n;
        nb -= min(n, nb);
        while (n--) {
            byte = *ucptr1 - *ucptr2;
            ucptr1++;
            ucptr2++;
            if (byte) {

                return byte;
            }
        }
        nw = nb / sizeof(long);
        ulptr1 = (const unsigned long *)ucptr1;
        ulptr2 = (const unsigned long *)ucptr2;
        nb -= nw * sizeof(long);
        while (nw--) {
            if (*ulptr1 != *ulptr2) {
                ucptr1 = (const unsigned char *)ulptr1;
                ucptr2 = (const unsigned char *)ulptr2;
                do {
                    byte = *ucptr1 - *ucptr2;
                } while (!byte);

                return byte;
            }
            ulptr1++;
            ulptr2++;
        }
    }
    if (nb) {
        do {
            byte = *ucptr1 - *ucptr2;
            ucptr1++;
            ucptr2++;
        } while (!byte && --nb);
    }

    return byte;
}

#if !defined(__GLIBC__)

/* TESTED OK */
PURE int
strcmp(const char *str1,
       const char *str2)
{
    char       *ptr1 = (char *)str1;
    char       *cptr2 = (char *)str2;
    int         retval = 0;

    while ((*cptr1) && *cptr1 == *cptr2) {
        cptr1++;
        cptr2++;
    }
    if (*cptr1) {
        retval = (int)*cptr1 - (int)*cptr2;
    }

    return retval;
}

/* TESTED OK */
PURE int
strncmp(const char *str1,
        const char *str2,
        size_t n)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2 = (char *)str2;
    int         retval = 0;

    if (n) {
        while ((*cptr1) && (*cptr1 == *cptr2) && (n--)) {
            cptr1++;
            cptr2++;
        }
        if (n) {
            retval = (int)*cptr1 - (int)*cptr2;
        }
    }

    return retval;
}

#endif /* !__GLIBC__ */

PURE int
strcoll(const char *str1,
        const char *str2)
{

    return -1;
}

size_t
strxfrm(char *dest, const char *src, size_t n)
{
    //    fprintf(stderr, "TODO: strxfrm() not implemented\n");

    return 0;
}

/* TESTED OK */
void *
memchr(const void *ptr,
       int ch,
       size_t n)
{
    char       *cptr = (char *)ptr;
    char        uc = (char)ch;
    void       *retval = NULL;

    if (n) {
        while ((*cptr != uc) && (n--)) {
            cptr++;
        }
        if (*cptr == uc && (n)) {
            retval = cptr;
        }
    }

    return retval;
}

#if !defined(__GLIBC__)

/* TESTED OK */
void *
strchr(const char *str,
       int ch)
{
    char       *cptr = (char *)str;
    char        c = (char)ch;
    void       *retval = NULL;

    while ((*cptr) && *cptr != c) {
        cptr++;
    }
    if (*cptr == c) {
        retval = cptr;
    }

    return retval;
}

#endif /* !__GLIBC__ */

/* TESTED OK */
size_t
strcspn(const char *str1,
        const char *str2)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2 = (char *)str2;
    size_t      len = 0;

    while (*cptr1) {
        while ((*cptr2) && *cptr1 != *cptr2) {
            cptr2++;
        }
        if (*cptr2) {

            break;
        }
        cptr1++;
        cptr2 = (char *)str2;
        len++;
    }

    return len;
}

char *
strpbrk(const char *str1,
        const char *str2)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2;
    char       *retptr = NULL;

    if (*str1) {
        while (!retptr && (*cptr1)) {
            cptr2 = (char *)str2;
            while (!retptr && (*cptr2)) {
                if (*cptr1 == *cptr2) {
                    retptr = (char *)cptr1;
                }
                cptr2++;
            }
            cptr1++;
        }
    }

    return retptr;
}

/* TESTED OK */
char *
strrchr(const char *str,
        int ch)
{
    char       *cptr = (char *)str;
    char        c = (char)ch;

    while (*cptr) {
        cptr++;
    }
    while (cptr >= str && *cptr != c) {
        cptr--;
    }
    if (cptr < str) {
        cptr = NULL;
    }

    return cptr;
}

/* TESTED OK */
size_t
strspn(const char *str1,
       const char *str2)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2;
    size_t      len = 0;

    while (*cptr1) {
        cptr2 = (char *)str2;
        while ((*cptr2) && *cptr1 != *cptr2) {
            cptr2++;
        }
        if (!*cptr2) {

            break;
        }
        cptr1++;
        len++;
    }

    return len;
}

char *
strstr(const char *str1,
       const char *str2)
{
    char       *cptr1 = (char *)str1;
    char       *cptr2;
    char       *cptr3;
    char       *retptr = NULL;

    while (!retptr && (*cptr1)) {
        cptr2 = cptr1;
        cptr3 = (char *)str2;
        while (*cptr2 == *cptr3) {
            cptr2++;
            cptr3++;
        }
        if (!*cptr2) {
            retptr = cptr1;
        }
        cptr1++;
    }

    return retptr;
}

#if !defined(__GLIBC__)

char *
strtok(char *str1,
       const char *str2)
{
    static char        *curtok = NULL;
    char               *cptr = curtok;
    char               *retptr = NULL;
    size_t              nspn;

    if ((str1) && (*str1)) {
        nspn = strspn(str1, str2);
        str1 += nspn;
        cptr = str1;
    }
    if (cptr) {
        retptr = cptr;
        nspn = strcspn(cptr, str2);
        cptr += nspn;
        *cptr = '\0';
        curtok = ++cptr;
        if (!*cptr) {
            curtok = NULL;
        }
        if (!retptr) {
            retptr = cptr;
            nspn = strcspn(cptr, str2);
            cptr += nspn;
            if (*cptr) {
                *cptr = '\0';
            } else {
                curtok = NULL;
            }
        }
    }

    return retptr;
}

#endif /* !__GLIBC__ */

/* TESTED OK */
void *
memset(void *ptr, int byte, size_t len)
{
    int8_t     *bptr = ptr;
    char        bval = (char)byte;
    long        val = bval;
    long        tmp = bval;
    size_t      nb = len;
    size_t      nw;
    long       *lptr;
    size_t      n;
    size_t      total = 0;

    val <<= 8;
    val |= tmp;
    tmp = val;
    val <<= 16;
    val |= tmp;
    if (sizeof(long) == 8) {
        tmp = val;
        val <<= 32;
        val |= tmp;
    }
    n = (uintptr_t)bptr & (sizeof(long) - 1);
    if (n) {
        n = sizeof(long) - n;
        n = min(n, nb);
        switch (n) {
            case 8:
                bptr[7] = bval;
            case 7:
                bptr[6] = bval;
            case 6:
                bptr[5] = bval;
            case 5:
                bptr[4] = bval;
            case 4:
                bptr[3] = bval;
            case 3:
                bptr[2] = bval;
            case 2:
                bptr[1] = bval;
            case 1:
                bptr[0] = bval;
            case 0:

                break;
        }
        total += n;
        nb -= n;
        bptr += n;
    }
    nw = nb / sizeof(long);
    nb -= nw * sizeof(long);
    if (nw) {
        total += nw * sizeof(long);
        lptr = (long *)bptr;
        while (nw--) {
            n = min(nw, 8);
            switch (n) {
                case 8:
                    lptr[7] = val;
                case 7:
                    lptr[6] = val;
                case 6:
                    lptr[5] = val;
                case 5:
                    lptr[4] = val;
                case 4:
                    lptr[3] = val;
                case 3:
                    lptr[2] = val;
                case 2:
                    lptr[1] = val;
                case 1:
                    lptr[0] = val;
                case 0:

                    break;
            }
            nw -= n;
            lptr += n;
        }
        bptr = (int8_t *)lptr;
    }
    if (nb) {
        switch (nb) {
            case 8:
                bptr[7] = bval;
            case 7:
                bptr[6] = bval;
            case 6:
                bptr[5] = bval;
            case 5:
                bptr[4] = bval;
            case 4:
                bptr[3] = bval;
            case 3:
                bptr[2] = bval;
            case 2:
                bptr[1] = bval;
            case 1:
                bptr[0] = bval;
            case 0:

                break;
        }
        total += nb;
    }

    return ptr;
}

char *
strerror(int errnum)
{
    //    fprintf(stderr, "TODO: strerror() not implemented\n");

    return NULL;
}

size_t
strlen(const char *str)
{
    size_t      len = 0;

    while (*str++) {
        len++;
    }

    return len;
}

#if ((defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 700))                 \
     || (defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE >= 200809L)))

size_t
strnlen(const char *str, size_t maxlen)
{
    size_t      len = 0;

    while (str[0] && (maxlen--)) {
        len++;
    }

    return len;
}

#endif

#if (defined(_SVID_SOURCE) || defined(_BSD_SOURCE)                      \
     || (defined(_XOPEN_SOURCE)                                         \
         && (_XOPEN_SOURCE >= 500 || defined(_XOPEN_SOURCE_EXTENDED))))
char *
strdup(const char *str)
{
    size_t      len = strlen(str);
    char       *buf = (len) ? malloc(len + 1) : NULL;

    if (buf) {
        memcpy(buf, str, len);
        buf[len] = '\0';
    } else {
#if defined(ENOMEM)
        errno = ENOMEM;
#endif
    }

    return buf;
}
#endif

#if !defined(__GLIBC__)

#if ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L)           \
     || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 700))
char *
strndup(const char *str, size_t maxlen)
{
    size_t      len = strlen(str);
    size_t      sz = min(len, maxlen);
    char       *buf = (sz) ? malloc(sz + 1) : NULL;

    if (buf) {
        memcpy(buf, str, sz);
        buf[sz] = '\0';
    } else {
#if defined(ENOMEM)
        errno = ENOMEM;
#endif
    }

    return buf;
}
#endif

#endif /* !__GLIBC__ */

#if defined(_GNU_SOURCE)

char *
strdupa(const char *str)
{
    size_t      len = strlen(str);
    char       *buf = (len) ? alloca(len + 1) : NULL;

    if (buf) {
        memcpy(buf, str, len);
        buf[len] = '\0';
    } else {
#if defined(ENOMEM)
        errno = ENOMEM;
#endif
    }

    return buf;
}

char *
strndupa(const char *str, size_t maxlen)
{
    size_t      len = strlen(str);
    size_t      sz = min(len, maxlen);
    char       *buf = (sz) ? alloca(sz + 1) : NULL;

    if (buf) {
        memcpy(buf, str, sz);
        buf[sz] = '\0';
    } else {
#if defined(ENOMEM)
        errno = ENOMEM;
#endif
    }

    return buf;
}

#endif


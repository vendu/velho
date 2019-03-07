#ifndef __MJOLNIR_DNG_H__
#define __MJOLNIR_DNG_H__

#include <mjolnir/conf.h>
//#include <mjolnir/cell.h>
#include <stddef.h>

/* data structures */
struct dnggame {
    char  *name;        // name of the game
    int    argc;        // cmdline argument count
    char **argv;        // cmdline arguments
};

struct dngobj {
    long  id;   // object ID
    long  type; // character type
    long  flg;  // flag bits
    long  x;    // level X-coordinate
    long  y;    // level Y-coordinate
    void *prev; // previous in queue, NULL for head item
    void *next; // next in queue, NULL for tail item
};

#endif /* __MJOLNIR_DNG_H__ */


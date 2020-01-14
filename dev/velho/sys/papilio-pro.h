#define MEM_PT_ENTRIES  (1 << MEM_PTSHIFT)

#define SYS_PROCS       16
#define SYS_THREADS     64

#define MEM_PAGE_SIZE   (1 << MEM_PGSHIFT)
#define MEM_PGSHIFT     12
#define MEM_PTSHIFT	10

typedef v0quad_t        v0wreg_t;
typedef v0long_t        v0reg_t;
typedef uint32_t        v0page_t;
typedef uint32_t        v0trap_t;


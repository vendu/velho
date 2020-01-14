#define ZEN_MEM_NULL_SEG            0
#define ZEN_MEM_CODE_SEG            1
#define ZEN_MEM_RODATA_SEG          2
#define ZEN_MEM_DATA_SEG            3
#define ZEN_MEM_HEAP_SEG            4
#define ZEN_MEM_STK_SEG             5
#define ZEN_MEM_SYS_SEG             6
#define ZEN_MEM_ZERO_BIT            0x01

struct zenmemseg {
    m_adr_t             base;
    m_adr_t             lim;
    m_uword_t           size;
    m_uword_t           flags;
};

struct zenmemmap {
    m_adr_t             adr;
    m_uword_t           size;
    m_uword_t           flags;
    struct zencred      cred;
};


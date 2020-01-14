struct zensys {
    struct zentask     *systasktab[ZEN_SYS_TASKS];
    struct zentask     *tasktab[ZEN_PROC_TASKS];
    m_page_t           *pagedir[ZEN_PAGE_DIR_ITEMS];
    m_uword_t           ndesc;
    m_desc_t           *desctab;
    m_uword_t           nnodehash;
    struct zenvfsnode  *nodehash;
    m_uword_t           nmembuf;
    struct zenmembuf   *membuftab;
};


static void             *pagetab[MEM_PT_ENTRIES];
static struct page     **pagephystsb;
static long	         pagemaxphys;
static void            **pagelru;

__inline__
pzero(void *adr)
{
    intmax_t       *ptr = adr;
    intmax_t        zero = 0;
    size_t	    n = MEM_PAGE_SIZE / sizeof(intmax_t);
	
    n >>= 3;
    while (n--) {
        ptr[0] = zero;
        ptr[1] = zero;
        ptr[2] = zero;
        ptr[3] = zero;
        ptr[4] = zero;
        ptr[5] = zero;
        ptr[6] = zero;
        ptr[7] = zero;
        ptr += 8;
    }
}

static void *
pmap(void *adr, v0page_t flg)
{
    long        pd = (long)adr >> (MEM_PTSHIFT + MEM_PGSHIFT);
    long        pt = ((long)adr >> MEM_PTSHIFT) & MEM_PTMASK;
    v0page_t   *ptr = pagetab[pd];
    v0page_t	pg;

    if (!ptr) {
        ptr = pdalloc();
        if (pdptr) {
            prockill(PROC_CUR);
        }
        tab[pd] = ptr;
    }
    tab = &ptr[pt];
    ptr = *tab;
    pg |= flg;
    *ptr = pg;

    return ptr;
}

static v0page_t
punmap(void *adr)
{
    v0page_t   *ptr;
    v0page_t	pg;
	
    ptr = pmap2(adr, 0);
    pg = *ptr;
    *ptr = 0;
	
    return pg;
}
	
static void
pqueue(struct page *pg)
{
    v0long_t	nf = pg->nflt;
    v0long_t	q = pg->slot;
	
    nf++;
    if (powerof2(nf)) {
        q++;
        pg->slot = q;
    }
    pg->nflt = nf;
    next = pagelru[q];
    pv->prev = NULL;
    if (next) {
        next->prev = pg;
    }
    pg->next = next;
    pagelru[q] = pg;

    return;
}

pdeque(long q)
{
    
}

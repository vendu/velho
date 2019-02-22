static struct taoschedq[TAO_SCHED_CLASSES][TAO_SCHED_PRIOS];

static void
taoqueuethr(struct taoschedq *queue, struct taothr *thr)
{
    long one;

    thr->mtx = ZEN_MTX_INIT_VAL;
    thr->next = NULL;
    zenlkmtx(&queue->tail->mtx);
    one = (m_atomread(&queue->tail) == &queue->dummy);
    if (!one) {
        thr->prev = queue->tail;
    } else {
        thr->prev = NULL;
        m_atomwrite(&queue->head, thr);
    }
    m_atomwrite(&queue->tail, thr);
    zenunlkmtx(&queue->tail->mtx);

    return;
}

static struct taothr *
taopopthr(struct taoschedq *queue)
{
    struct taothr *thr;
    long           one;

    zenlkmtx(&queue->head->mtx);
    thr = queue->head;
    one = (thr == &queue->dummy);
    if (!one) {
        thr->next->prev = NULL;
        m_atomwrite(&queue->head, thr->next);
    } else {
        zenlkmtx(&queue->dummy.mtx);
        m_atomwrite(&queue->tail, &queue->dummy);
        m_atomwrite(&queue->head, &queue->dummy);
        zenunlkmtx(&queue->dummy.mtx);
    }

    return thr;
}

static void
taodequethr(struct taoschedq *queue, struct taothr *thr)
{
    struct taothr *tp;

    zenlkmtx(&thr->mtx);
    tp = thr->prev;
    if (tp) {
        tp->next = thr->next;
    } else {
        tp = t->next;
        if (tp) {
        }
    }
}


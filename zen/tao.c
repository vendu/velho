static struct taoschedq[TAO_SCHED_CLASSES][TAO_SCHED_PRIOS];

static void
taoqueuethr(struct taoschedq *queue, struct taothr *thr)
{
    struct thr *prev;
    long        one;

    thr->next = NULL;
    zenlkmtx(&queue->mtx);
    prev = queue->tail;
    thr->prev = prev;
    queue->tail = thr;
    if (!prev) {
        queue->head = thr;
    }
    zenunlkmtx(&queue->mtx);

    return;
}

static struct taothr *
taopopthr(struct taoschedq *queue)
{
    struct taothr *thr;
    struct taothr *next;
    long           one;

    zenlkmtx(&queue->mtx);
    thr = queue->head;
    if (thr) {
        next = thr->next;
        queue->head = next;
        if (!next) {
            queue->tail = NULL;
        }
    }
    zenunlkmtx(&queue->mtx);

    return thr;
}

static void
taodequethr(struct taoschedq *queue, struct taothr *thr)
{
    struct taothr *prev;
    struct taothr *next;

    zenlkmtx(&queue->mtx);
    prev = thr->prev;
    next = thr->next;
    if (prev) {
        prev->next = next;
    } else {
        queue->head = next;
    }
    if (next) {
        next->prev = thr->prev;
    } else {
        queue->tail = prev;
    }
    zenunlkmtx(&queue->mtx);

    return;
}


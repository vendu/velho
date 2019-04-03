const uint8_t   g_clztab4bit[16]
= { 4U, 3U, 2U, 2U, 1U, 1U, 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U };
const uint8_t   g_ctztab4bit[32];
const uint8_t   g_clztab8bit[256]
= {
    32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
    28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U,
    27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
    27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
    26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
    26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
    26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
    26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
    24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U
};

static __inline__ long
bitinitctz(void)
{
    long l;

    for (l = 0 ; l < 32 ; l++) {
        ndx = (0x077cb531 * (1 << l)) >> 27;
        g_ctztab4bit[ndx] = l;
    }

    return;
}


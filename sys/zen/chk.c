#include <stdlib.h>
#include <assert.h>
#include <zen/fs0.h>

int
main(int argc, char *argv[])
{
    assert(sizeof(struct zenfs0blktabs) == 64);
    assert(sizeof(union zenfs0link) == 64);
    assert(sizeof(struct zenfs0common) == 128);

    exit(0);
}


#include "block.h"
#include "free.h"
#include "mkfs.h"

void mkfs(void)
{
    unsigned char block[BLOCK_SIZE];
    int i;

    for (i = 0; i < BLOCK_SIZE; i++)
        block[i] = 0;
    bwrite(0, block);
    bwrite(1, block);
    bwrite(3, block);
    bwrite(4, block);
    bwrite(5, block);
    bwrite(6, block);

    for (i = 0; i < 7; i++)
        alloc();
}
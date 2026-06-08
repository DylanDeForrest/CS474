#include <string.h>
#include "block.h"
#include "free.h"
#include "inode.h"
#include "mkfs.h"
#include "pack.h"
#include "dir.h"

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

    struct inode *in = ialloc();
    int data_block = alloc();

    in->flags = 2;
    in->size = DIR_ENTRY_SIZE * 2;
    in->block_ptr[0] = data_block;

    unsigned char dir_block[BLOCK_SIZE];
    for (i = 0; i < BLOCK_SIZE; i++)
        dir_block[i] = 0;

    write_u16(dir_block + 0, in->inode_num);
    strcpy((char *)(dir_block + 2), ".");

    write_u16(dir_block + DIR_ENTRY_SIZE, in->inode_num);
    strcpy((char *)(dir_block + DIR_ENTRY_SIZE + 2), "..");

    bwrite(data_block, dir_block);
    iput(in);
}
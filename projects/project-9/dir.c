#include <stdlib.h>
#include <string.h>
#include "dir.h"
#include "block.h"
#include "inode.h"
#include "pack.h"

struct directory *directory_open(int inode_num)
{
    struct inode *in = iget(inode_num);
    if (in == NULL)
        return NULL;

    struct directory *dir = malloc(sizeof(struct directory));
    dir->inode = in;
    dir->offset = 0;
    return dir;
}

int directory_get(struct directory *dir, struct directory_entry *ent)
{
    unsigned char block[BLOCK_SIZE];

    if (dir->offset >= dir->inode->size)
        return -1;

    int data_block_index = dir->offset / BLOCK_SIZE;
    int data_block_num = dir->inode->block_ptr[data_block_index];
    bread(data_block_num, block);

    int offset_in_block = dir->offset % BLOCK_SIZE;

    ent->inode_num = read_u16(block + offset_in_block);
    strcpy(ent->name, (char *)(block + offset_in_block + 2));

    dir->offset += DIR_ENTRY_SIZE;
    return 0;
}

void directory_close(struct directory *d)
{
    iput(d->inode);
    free(d);
}

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

struct inode *namei(char *path)
{
    struct directory *dir;
    struct directory_entry ent;
    struct inode *in;
    char name[DIR_NAME_SIZE];
    int i;
 
    if (strcmp(path, "/") == 0)
        return iget(ROOT_INODE_NUM);
 
    i = 0;
    while (path[i + 1] != '\0' && i < DIR_NAME_SIZE - 1) {
        name[i] = path[i + 1];
        i++;
    }
    name[i] = '\0';
 
    dir = directory_open(ROOT_INODE_NUM);
    if (dir == NULL)
        return NULL;
 
    while (directory_get(dir, &ent) != -1) {
        if (strcmp(ent.name, name) == 0) {
            directory_close(dir);
            in = iget(ent.inode_num);
            return in;
        }
    }
 
    directory_close(dir);
    return NULL;
}
 
int directory_make(char *path)
{
    unsigned char block[BLOCK_SIZE];
    unsigned char dir_block[BLOCK_SIZE];
    struct inode *parent_in;
    struct inode *new_in;
    int new_data_block;
    int parent_block_num;
    int offset_in_block;
    int i;
    char name[DIR_NAME_SIZE];
 
    if (path[0] != '/')
        return -1;
 
    i = 0;
    while (path[i + 1] != '\0' && i < DIR_NAME_SIZE - 1) {
        name[i] = path[i + 1];
        i++;
    }
    name[i] = '\0';
 
    parent_in = namei("/");
    if (parent_in == NULL)
        return -1;
 
    new_in = ialloc();
    if (new_in == NULL) {
        iput(parent_in);
        return -1;
    }
 
    new_data_block = alloc();
    if (new_data_block == -1) {
        iput(new_in);
        iput(parent_in);
        return -1;
    }
 
    for (i = 0; i < BLOCK_SIZE; i++)
        dir_block[i] = 0;
 
    write_u16(dir_block + 0, new_in->inode_num);
    strcpy((char *)(dir_block + 2), ".");
 
    write_u16(dir_block + DIR_ENTRY_SIZE, parent_in->inode_num);
    strcpy((char *)(dir_block + DIR_ENTRY_SIZE + 2), "..");
 
    bwrite(new_data_block, dir_block);
 
    new_in->flags = 2;
    new_in->size = DIR_ENTRY_SIZE * 2;
    new_in->block_ptr[0] = new_data_block;
 
    write_inode(new_in);
 
    parent_block_num = parent_in->block_ptr[parent_in->size / BLOCK_SIZE];
    offset_in_block = parent_in->size % BLOCK_SIZE;
 
    if (offset_in_block == 0) {
        parent_block_num = alloc();
        parent_in->block_ptr[parent_in->size / BLOCK_SIZE] = parent_block_num;
        for (i = 0; i < BLOCK_SIZE; i++)
            block[i] = 0;
    } else {
        bread(parent_block_num, block);
    }
 
    write_u16(block + offset_in_block, new_in->inode_num);
    strcpy((char *)(block + offset_in_block + 2), name);
 
    bwrite(parent_block_num, block);
 
    parent_in->size += DIR_ENTRY_SIZE;
 
    iput(new_in);
    iput(parent_in);
 
    return 0;
}
#include <stddef.h>
#include "block.h"
#include "free.h"
#include "inode.h"
#include "pack.h"

static struct inode incore[MAX_SYS_OPEN_FILES] = {0};

struct inode *incore_find_free(void)
{
	int i = 0;
	while (i < MAX_SYS_OPEN_FILES) {
		if (incore[i].ref_count == 0)
			return &incore[i];
		i++;
	}
	return NULL;
}

struct inode *incore_find(unsigned int inode_num)
{
	int i = 0;
	while (i < MAX_SYS_OPEN_FILES) {
		if (incore[i].ref_count != 0 && incore[i].inode_num == inode_num)
			return &incore[i];
		i++;
	}
	return NULL;
}

void incore_free_all(void)
{
	int i = 0;
	while (i < MAX_SYS_OPEN_FILES) {
		incore[i].ref_count = 0;
		i++;
	}
}

void read_inode(struct inode *in, int inode_num)
{
	unsigned char block[BLOCK_SIZE];
	int block_num = inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
	int block_offset = (inode_num % INODES_PER_BLOCK) * INODE_SIZE;
	int i = 0;

	bread(block_num, block);

	in->size        = read_u32(block + block_offset + 0);
	in->owner_id    = read_u16(block + block_offset + 4);
	in->permissions = read_u8 (block + block_offset + 6);
	in->flags       = read_u8 (block + block_offset + 7);
	in->link_count  = read_u8 (block + block_offset + 8);

	while (i < INODE_PTR_COUNT) {
		in->block_ptr[i] = read_u16(block + block_offset + 9 + i * 2);
		i++;
	}
}

void write_inode(struct inode *in)
{
	unsigned char block[BLOCK_SIZE];
	int block_num = in->inode_num / INODES_PER_BLOCK + INODE_FIRST_BLOCK;
	int block_offset = (in->inode_num % INODES_PER_BLOCK) * INODE_SIZE;
	int i = 0;

	bread(block_num, block);

	write_u32(block + block_offset + 0, in->size);
	write_u16(block + block_offset + 4, in->owner_id);
	write_u8 (block + block_offset + 6, in->permissions);
	write_u8 (block + block_offset + 7, in->flags);
	write_u8 (block + block_offset + 8, in->link_count);

	while (i < INODE_PTR_COUNT) {
		write_u16(block + block_offset + 9 + i * 2, in->block_ptr[i]);
		i++;
	}

	bwrite(block_num, block);
}

struct inode *iget(int inode_num)
{
	struct inode *in = incore_find(inode_num);
	if (in != NULL) {
		in->ref_count++;
		return in;
	}

	in = incore_find_free();
	if (in == NULL)
		return NULL;

	read_inode(in, inode_num);
	in->ref_count = 1;
	in->inode_num = inode_num;
	return in;
}

void iput(struct inode *in)
{
	if (in->ref_count == 0)
		return;
	in->ref_count--;
	if (in->ref_count == 0)
		write_inode(in);
}

struct inode *ialloc(void)
{
	unsigned char block[BLOCK_SIZE];
	int inode_num;
	int i = 0;

	bread(1, block);
	inode_num = find_free(block);
	if (inode_num == -1)
		return NULL;

	struct inode *in = iget(inode_num);
	if (in == NULL)
		return NULL;

	set_free(block, inode_num, 1);
	bwrite(1, block);

	in->size = 0;
	in->owner_id = 0;
	in->permissions = 0;
	in->flags = 0;
	in->link_count = 0;
	while (i < INODE_PTR_COUNT) {
		in->block_ptr[i] = 0;
		i++;
	}
	in->inode_num = inode_num;

	write_inode(in);
	return in;
}

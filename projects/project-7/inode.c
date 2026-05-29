#include "block.h"
#include "free.h"
#include "inode.h"

int ialloc(void)
{
	unsigned char block[BLOCK_SIZE];
	int inode_num;

	bread(1, block);
	inode_num = find_free(block);
	if (inode_num == -1)
		return -1;
	set_free(block, inode_num, 1);
	bwrite(1, block);
	return inode_num;
}
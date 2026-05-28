#include <unistd.h>
#include "block.h"
#include "image.h"

unsigned char *bread(int block_num, unsigned char *block)
{
	off_t offset;
	offset = block_num * BLOCK_SIZE;
	lseek(image_fd, offset, SEEK_SET);
	read(image_fd, block, BLOCK_SIZE);
	return block;
}

void bwrite(int block_num, unsigned char *block)
{
	off_t offset;
	offset = block_num * BLOCK_SIZE;
	lseek(image_fd, offset, SEEK_SET);
	write(image_fd, block, BLOCK_SIZE);
}

int alloc(void)
{
	unsigned char block[BLOCK_SIZE];
	int block_num;
 
	bread(2, block);
	block_num = find_free(block);
	if (block_num == -1)
		return -1;
	set_free(block, block_num, 1);
	bwrite(2, block);
	return block_num;
}
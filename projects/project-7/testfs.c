#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ctest.h"
#include "image.h"
#include "block.h"
#include "free.h"
#include "inode.h"
#include "mkfs.h"

#define TEST_IMAGE "test_image.bin"

#ifdef CTEST_ENABLE

void test_image_open(void)
{
	unlink(TEST_IMAGE);
	int fd = image_open(TEST_IMAGE, 1);
	CTEST_ASSERT(fd >= 0, "open should work");
	image_close();
}

void test_image_open_truncate(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	image_close();
	int fd = image_open(TEST_IMAGE, 1);
	CTEST_ASSERT(fd >= 0, "truncate should work");
	image_close();
}

void test_image_close(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	int result = image_close();
	CTEST_ASSERT(result == 0, "close should work");
}

void test_bread(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	unsigned char block[BLOCK_SIZE];
	unsigned char *result = bread(0, block);
	CTEST_ASSERT(result == block, "bread should return pointer");
	image_close();
}

void test_bwrite(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	unsigned char block[BLOCK_SIZE];
	int i = 0;
	while (i < BLOCK_SIZE) {
		block[i] = 0xAA;
		i = i + 1;
	}
	bwrite(0, block);
	image_close();
	CTEST_ASSERT(1, "write should not crash");
}

void test_write_and_read(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	unsigned char block[BLOCK_SIZE];
	unsigned char block2[BLOCK_SIZE];
	int i = 0;
	while (i < BLOCK_SIZE) {
		block[i] = 0x55;
		i = i + 1;
	}
	bwrite(0, block);
	bread(0, block2);
	int match = 1;
	int j = 0;
	while (j < BLOCK_SIZE) {
		if (block2[j] != 0x55) {
			match = 0;
		}
		j = j + 1;
	}
	CTEST_ASSERT(match, "data should match");
	image_close();
}

void test_block_0(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	unsigned char block[BLOCK_SIZE];
	int i = 0;
	while (i < BLOCK_SIZE) {
		block[i] = 0x11;
		i = i + 1;
	}
	bwrite(0, block);
	image_close();
	CTEST_ASSERT(1, "block 0 write");
}

void test_block_1(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	unsigned char block[BLOCK_SIZE];
	int i = 0;
	while (i < BLOCK_SIZE) {
		block[i] = 0x22;
		i = i + 1;
	}
	bwrite(1, block);
	image_close();
	CTEST_ASSERT(1, "block 1 write");
}

void test_set_free(void)
{
	unsigned char block[BLOCK_SIZE];
	int i = 0;
	while (i < BLOCK_SIZE) {
		block[i] = 0;
		i = i + 1;
	}
	set_free(block, 5, 1);
	CTEST_ASSERT(block[0] == 32, "bit 5 set");
}

void test_find_free(void)
{
	unsigned char block[BLOCK_SIZE];
	int i = 0;
	while (i < BLOCK_SIZE) {
		block[i] = 0;
		i = i + 1;
	}
	int x = find_free(block);
	CTEST_ASSERT(x == 0, "find first free");
}

void test_ialloc_basic(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	int x = ialloc();
	CTEST_ASSERT(x >= 0, "ialloc works");
	image_close();
}

void test_alloc_basic(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	int x = alloc();
	CTEST_ASSERT(x >= 0, "alloc works");
	image_close();
}

void test_mkfs_basic(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	unsigned char b[BLOCK_SIZE];
	bread(2, b);
	CTEST_ASSERT(b[0] == 0x7F, "mkfs writes block map");
	image_close();
}

int main(void)
{
	CTEST_COLOR(1);
	CTEST_VERBOSE(1);
	test_image_open();
	test_image_open_truncate();
	test_image_close();
	test_bread();
	test_bwrite();
	test_write_and_read();
	test_block_0();
	test_block_1();
	test_set_free();
	test_find_free();
	test_ialloc_basic();
	test_alloc_basic();
	test_mkfs_basic();
	CTEST_RESULTS();
	CTEST_EXIT();
	return 0;
}

#endif
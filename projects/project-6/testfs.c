#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ctest.h"
#include "image.h"
#include "block.h"

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
	CTEST_RESULTS();
	CTEST_EXIT();
	return 0;
}

#endif
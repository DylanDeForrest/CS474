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
#include "dir.h"

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
		if (block2[j] != 0x55)
			match = 0;
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
	CTEST_ASSERT(b[0] == 0xFF, "mkfs writes block map");
	image_close();
}

void test_incore_find_free(void)
{
	incore_free_all();
	struct inode *in = incore_find_free();
	CTEST_ASSERT(in != NULL, "should find a free incore inode");
}

void test_incore_find_free_sets_ref(void)
{
	incore_free_all();
	struct inode *in = incore_find_free();
	in->ref_count = 1;
	in->inode_num = 42;
	struct inode *found = incore_find(42);
	CTEST_ASSERT(found == in, "incore_find returns same pointer");
}

void test_incore_find_not_found(void)
{
	incore_free_all();
	struct inode *found = incore_find(99);
	CTEST_ASSERT(found == NULL, "incore_find returns NULL when not present");
}

void test_incore_free_all(void)
{
	incore_free_all();
	struct inode *in = incore_find_free();
	in->ref_count = 5;
	incore_free_all();
	struct inode *in2 = incore_find_free();
	CTEST_ASSERT(in2 != NULL, "after free_all a slot is available");
}

void test_read_write_inode(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();

	struct inode out;
	out.size = 1234;
	out.owner_id = 7;
	out.permissions = 0x6;
	out.flags = 0x1;
	out.link_count = 2;
	out.inode_num = 0;
	int i = 0;
	while (i < INODE_PTR_COUNT) {
		out.block_ptr[i] = i + 10;
		i = i + 1;
	}
	write_inode(&out);

	struct inode in;
	read_inode(&in, 0);
	CTEST_ASSERT(in.size == 1234, "size round-trips");
	CTEST_ASSERT(in.owner_id == 7, "owner_id round-trips");
	CTEST_ASSERT(in.permissions == 0x6, "permissions round-trips");
	CTEST_ASSERT(in.flags == 0x1, "flags round-trips");
	CTEST_ASSERT(in.link_count == 2, "link_count round-trips");
	CTEST_ASSERT(in.block_ptr[0] == 10, "block_ptr[0] round-trips");
	CTEST_ASSERT(in.block_ptr[15] == 25, "block_ptr[15] round-trips");
	image_close();
}

void test_iget_loads_inode(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();

	struct inode out;
	out.size = 500;
	out.owner_id = 0;
	out.permissions = 0;
	out.flags = 0;
	out.link_count = 0;
	out.inode_num = 1;
	int i = 0;
	while (i < INODE_PTR_COUNT) {
		out.block_ptr[i] = 0;
		i = i + 1;
	}
	write_inode(&out);

	struct inode *in = iget(1);
	CTEST_ASSERT(in != NULL, "iget returns non-null");
	CTEST_ASSERT(in->size == 500, "iget loads size");
	CTEST_ASSERT(in->ref_count == 1, "iget sets ref_count to 1");
	image_close();
}

void test_iget_increments_ref(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();

	struct inode out;
	out.size = 0;
	out.owner_id = 0;
	out.permissions = 0;
	out.flags = 0;
	out.link_count = 0;
	out.inode_num = 2;
	int i = 0;
	while (i < INODE_PTR_COUNT) {
		out.block_ptr[i] = 0;
		i = i + 1;
	}
	write_inode(&out);

	struct inode *a = iget(2);
	struct inode *b = iget(2);
	CTEST_ASSERT(a == b, "iget returns same pointer for same inode");
	CTEST_ASSERT(a->ref_count == 2, "ref_count incremented on second iget");
	image_close();
}

void test_iput_decrements_ref(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();

	struct inode out;
	out.size = 0;
	out.owner_id = 0;
	out.permissions = 0;
	out.flags = 0;
	out.link_count = 0;
	out.inode_num = 3;
	int i = 0;
	while (i < INODE_PTR_COUNT) {
		out.block_ptr[i] = 0;
		i = i + 1;
	}
	write_inode(&out);

	struct inode *in = iget(3);
	iget(3);
	iput(in);
	CTEST_ASSERT(in->ref_count == 1, "ref_count decremented by iput");
	image_close();
}

void test_iput_frees_at_zero(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();

	struct inode out;
	out.size = 0;
	out.owner_id = 0;
	out.permissions = 0;
	out.flags = 0;
	out.link_count = 0;
	out.inode_num = 4;
	int i = 0;
	while (i < INODE_PTR_COUNT) {
		out.block_ptr[i] = 0;
		i = i + 1;
	}
	write_inode(&out);

	struct inode *in = iget(4);
	iput(in);
	CTEST_ASSERT(in->ref_count == 0, "ref_count is 0 after final iput");
	image_close();
}

void test_ialloc_basic(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();
	struct inode *in = ialloc();
	CTEST_ASSERT(in != NULL, "ialloc returns non-null");
	CTEST_ASSERT(in->ref_count == 1, "ialloc sets ref_count to 1");
	CTEST_ASSERT(in->size == 0, "ialloc initializes size to 0");
	image_close();
}

void test_ialloc_multiple(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();
	struct inode *a = ialloc();
	struct inode *b = ialloc();
	CTEST_ASSERT(a != NULL && b != NULL, "two iallocs succeed");
	CTEST_ASSERT(a->inode_num != b->inode_num, "iallocs get different inode numbers");
	image_close();
}

void test_directory_open(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();
	struct directory *dir = directory_open(0);
	CTEST_ASSERT(dir != NULL, "directory_open returns non-null");
	directory_close(dir);
	image_close();
}

void test_directory_get(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();
	struct directory *dir = directory_open(0);
	struct directory_entry ent;
	int r = directory_get(dir, &ent);
	CTEST_ASSERT(r == 0, "directory_get first entry succeeds");
	CTEST_ASSERT(ent.inode_num == 0, "first entry inode_num is 0");
	CTEST_ASSERT(ent.name[0] == '.', "first entry is dot");
	r = directory_get(dir, &ent);
	CTEST_ASSERT(r == 0, "directory_get second entry succeeds");
	r = directory_get(dir, &ent);
	CTEST_ASSERT(r == -1, "directory_get returns -1 past end");
	directory_close(dir);
	image_close();
}

void test_directory_close(void)
{
	unlink(TEST_IMAGE);
	image_open(TEST_IMAGE, 1);
	mkfs();
	incore_free_all();
	struct directory *dir = directory_open(0);
	directory_close(dir);
	CTEST_ASSERT(1, "directory_close does not crash");
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
	test_alloc_basic();
	test_mkfs_basic();
	test_incore_find_free();
	test_incore_find_free_sets_ref();
	test_incore_find_not_found();
	test_incore_free_all();
	test_read_write_inode();
	test_iget_loads_inode();
	test_iget_increments_ref();
	test_iput_decrements_ref();
	test_iput_frees_at_zero();
	test_ialloc_basic();
	test_ialloc_multiple();
	test_directory_open();
	test_directory_get();
	test_directory_close();
	CTEST_RESULTS();
	CTEST_EXIT();
	return 0;
}

#endif
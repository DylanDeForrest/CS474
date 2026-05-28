CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar
ARFLAGS = rcs

# Source files
LIB_SRCS = image.c block.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_TARGET = libvvsfs.a

TEST_SRCS = testfs.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_TARGET = testfs

# Default target
all: $(LIB_TARGET) $(TEST_TARGET)

# Build static library
$(LIB_TARGET): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

# Build test executable
$(TEST_TARGET): $(TEST_OBJS) $(LIB_TARGET)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LIB_TARGET)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile testfs with CTEST_ENABLE
testfs.o: testfs.c
	$(CC) $(CFLAGS) -DCTEST_ENABLE -c $< -o $@

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean object files and static library
clean:
	rm -f $(LIB_OBJS) $(TEST_OBJS)

# Clean everything including executables and libraries
pristine: clean
	rm -f $(LIB_TARGET) $(TEST_TARGET)

.PHONY: all test clean pristine
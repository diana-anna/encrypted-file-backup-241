# not using standard Makefile template because this makefile creates shared
# objects and weird stuff
CC = clang
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter -Wmissing-declarations -Wmissing-variable-declarations
CFLAGS_DEBUG   = -O0 $(WARNINGS) -g -std=c99 -D_GNU_SOURCE -DDEBUG -lm
CFLAGS_RELEASE = -O3 $(WARNINGS) -std=c99 -D_GNU_SOURCE -lm

all: sbrk_alloc.so test

sbrk_alloc.so: sbrk_alloc.c
	$(CC) $^ $(CFLAGS_DEBUG) -o $@ -shared -fPIC -lm

test: test.c
	$(CC) $^ $(CFLAGS_RELEASE) -o $@ -ldl

run_test: test sbrk_alloc.so
	LD_PRELOAD=./sbrk_alloc.so ./test

.PHONY : clean
clean:
	-rm -rf sbrk_alloc.so test

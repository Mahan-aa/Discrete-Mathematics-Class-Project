CC      ?= gcc
CFLAGS  ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Isrc
LIB_SRC := $(filter-out src/main.c,$(wildcard src/*.c))
LIB_OBJ := $(LIB_SRC:src/%.c=build/%.o)
TESTS   := $(wildcard tests/test_*.c)
TEST_BIN:= $(TESTS:tests/%.c=build/%)

.PHONY: all test clean
all: $(if $(wildcard src/main.c),build/rsa)

build/%.o: src/%.c $(wildcard src/*.h) | build
	$(CC) $(CFLAGS) -c $< -o $@

build/rsa: src/main.c $(LIB_OBJ) | build
	$(CC) $(CFLAGS) $^ -o $@

build/test_%: tests/test_%.c $(LIB_OBJ) | build
	$(CC) $(CFLAGS) $^ -o $@

build:
	mkdir -p build

test: $(TEST_BIN)
	@for t in $(TEST_BIN); do echo "== $$t"; ./$$t || exit 1; done

clean:
	rm -rf build

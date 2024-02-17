CFLAGS := -std=gnu99 -fdiagnostics-color -Wall -Wextra
ifeq ($(debug),yes)
	CFLAGS += -O0 -g -DDEBUG
else
	CFLAGS += -Os -DNDEBUG
endif

all: _all

CLEAN_TARGETS :=

MINI64_DIR := .
include make.mk

TEST_DIR := tests
include tests/make.mk

.PHONY: all _all clean format

_all: $(MINI64_SHARED_LIBRARY) $(MINI64_STATIC_LIBRARY)

format:
	@find . -name '*.[hc]' -exec clang-format -i {} \;

clean: $(CLEAN_TARGETS)

TEST_BINARY := $(TEST_DIR)/tests

$(TEST_BINARY): $(TEST_DIR)/tests.c $(MINI64_STATIC_LIBRARY)
	$(CC) $(CFLAGS) $(MINI64_INCLUDE_FLAGS) $^ -lscrutiny -o $@

tests: $(TEST_BINARY)
	@$<

tests_clean:
	@rm -f $(TEST_BINARY)

.PHONY: tests_clean

CLEAN_TARGETS += tests_clean

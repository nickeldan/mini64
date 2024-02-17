ifndef MINI64_MK
MINI64_MK :=

MINI64_LIB_DIR ?= $(MINI64_DIR)
MINI64_OBJ_DIR ?= $(MINI64_DIR)/src

MINI64_SHARED_LIBRARY := $(MINI64_LIB_DIR)/libmini64.so
MINI64_STATIC_LIBRARY := $(MINI64_LIB_DIR)/libmini64.a

MINI64_INCLUDE_FLAGS := -I$(MINI64_DIR)/include

$(MINI64_SHARED_LIBRARY): $(MINI64_OBJ_DIR)/mini64.o
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -shared -o $@ $<

$(MINI64_STATIC_LIBRARY): $(MINI64_OBJ_DIR)/mini64.o
	@mkdir -p $(@D)
	$(AR) rcs $@ $<

$(MINI64_OBJ_DIR)/mini64.o: $(MINI64_DIR)/src/mini64.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(MINI64_INCLUDE_FLAGS) -c $< -o $@

mini64_clean:
	@rm -f $(MINI64_SHARED_LIBRARY) $(MINI64_STATIC_LIBRARY) $(MINI64_OBJ_DIR)/mini64.o

.PHONY: mini64_clean
CLEAN_TARGETS += mini64_clean

endif
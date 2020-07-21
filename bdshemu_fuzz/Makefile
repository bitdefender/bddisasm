# Look for afl-fuzz. If it is in path we assume that afl-gcc/afl-clang/afl-clang-fast is present as well.
ifeq (, $(shell which afl-fuzz))
  $(error "It looks like AFL is not in your path. AFL is available on GitHub: https://github.com/google/AFL")
endif

# Try to figure out which compiler we should use, afl-clang-fast being the preferred choice.
ifeq (, $(AFL_COMPILER))
  # If AFL_COMPILER is not set, check if afl-clang-fast is available.
  ifeq (, $(shell which afl-clang-fast))
    # If it is not, try to figure out if we should use afl-gcc or afl-clang.
    ifeq "$(findstring clang, $(shell $(CC) --version 2>/dev/null))" ""
      AFL_COMPILER = afl-gcc
    else
      AFL_COMPILER = afl-clang
    endif
  else
    # If it is available, use it.
    AFL_COMPILER = afl-clang-fast
  endif
endif

ROOT_DIR   := $(realpath ..)
DISASM_SRC := $(ROOT_DIR)/bddisasm
DISASM_INC := $(DISASM_SRC)/include
SHEMU_SRC  := $(ROOT_DIR)/bdshemu
PUBLIC_INC := $(ROOT_DIR)/inc
SHEMU_INC  := $(PUBLIC_INC)/bdshemu
FUZZER_SRC := $(ROOT_DIR)/bdshemu_fuzz

ALL_SRC    := $(shell find $(DISASM_SRC)/*.c -type f)
ALL_SRC    += $(shell find $(SHEMU_SRC)/*.c -type f)
ALL_SRC    += $(shell find $(FUZZER_SRC)/*.c -type f)

ALL_INC    := $(shell find $(DISASM_INC)/*.h -type f)
ALL_INC    += $(shell find $(PUBLIC_INC)/*.h -type f)
ALL_INC    += $(shell find $(SHEMU_INC)/*.h -type f)
 
# Enable qui ck and dirty mode, if needed.
ifeq ($(DIRTY),y)
  AFL_ARGS := -d
endif

ifdef AFL_MEMORY
  AFL_ARGS := -m $(AFL_MEMORY)
endif
 
CFLAGS     := -std=c11 -fpic -fno-strict-aliasing \
           -D_REENTRANT -fstack-protector -ffunction-sections -fdata-sections \
           -DNDEBUG -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=1 -Ofast -g3

ifeq ($(LOG),y)
  PREDEFS := -DENABLE_LOGGING
endif 

# Build shfuzz with AFL instrumentation.
shfuzz: $(ALL_SRC) $(ALL_INC)
	@echo "Using $(AFL_COMPILER) as a compiler"
	@$(AFL_COMPILER) $(PREDEFS) $(CFLAGS) -I$(DISASM_INC) -I$(PUBLIC_INC) -I$(SHEMU_INC) $(ALL_SRC) -o shfuzz
	@echo "Done!"

# Run the tests in 32-bit mode.
.PHONY: fuzz32
fuzz32: shfuzz
	@afl-fuzz -i ./in-32 -o ./out-32 $(AFL_ARGS) ./shfuzz @@ 32

# Run the tests in 64-bit mode.
.PHONY: fuzz64
fuzz64: shfuzz
	@afl-fuzz -i ./in-64 -o ./out-64 $(AFL_ARGS) ./shfuzz @@ 64

.PHONY: clean
clean:
	@rm -f ./shfuzz

.PHONY: clean

SRC_FILES := crt.c bddisasm.c bdformat.c bdhelpers.c

OBJECTS := $(SRC_FILES:.c=.o)

INCLUDES := -Iinclude -I../inc -I../../inc

ifeq ($(PLATFORM),)
	PLATFORM := x64
endif

ifeq ($(RELEASE),y)
	CONFIGURATION := Release
else
	CONFIGURATION := Debug
endif

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

DEFINES := -D_LIB -DAMD64

INT_DIR := ../_intdir/bddisasm/$(PLATFORM)/$(CONFIGURATION)

DEP_DIR := $(INT_DIR)/.d

OUT_DIR := ../bin/$(PLATFORM)/$(CONFIGURATION)

OBJECTS_PATH := $(addprefix $(INT_DIR)/, $(OBJECTS))

LIB_NAME := libbddisasm.a

WARNINGS_ENABLE := -Wall -Wextra -Wshadow -Wstrict-overflow \
	-Wframe-larger-than=3072 -Wstack-usage=3072

WARNINGS_DISABLE := -Wno-missing-field-initializers \
	-Wno-missing-braces \
	-Wno-unused-function \
	-Wno-unused-variable \
	-Wno-unknown-pragmas \
	-Wno-format \
	-Wno-multichar

# Some warnings are compiler-specific, some version-specific
ifeq (,$(findstring clang,$(CC)))
	CC_VERSION_MAJOR = $(shell $(CC) -dumpversion | cut -d '.' -f1)
	CC_VERSION_GT_4 = $(shell [ $(CC_VERSION_MAJOR) -gt 4 ] && echo true)
	CC_VERSION_GT_5 = $(shell [ $(CC_VERSION_MAJOR) -gt 5 ] && echo true)

	ifeq (true,$(CC_VERSION_GT_4))
		WARNINGS_DISABLE += -Wno-incompatible-pointer-types
	endif

	ifeq (true,$(CC_VERSION_GT_5))
		WARNINGS_ENABLE += -Wshift-overflow=2 \
			-Wnull-dereference \
			-Wduplicated-cond
      endif
else
	WARNINGS_DISABLE += -Wno-typedef-redefinition \
		-Wno-missing-braces \
		-Wno-incompatible-pointer-types \
		-Wno-missing-field-initializers
endif

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td

CFLAGS = -pipe -c -std=c11 -fpic -fno-strict-aliasing \
	-D_REENTRANT -fstack-protector -ffunction-sections -fdata-sections \
	$(WARNINGS_ENABLE) $(WARNINGS_DISABLE) $(DEFINES) $(INCLUDES)

ifeq ($(RELEASE),y)
	CFLAGS += -Ofast -g3 -DNDEBUG -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=1
else
	CFLAGS += -O0 -g3 -D_DEBUG -DDEBUG
endif

all: $(LIB_NAME)

# Here it will link the objects created below in the final .a
$(LIB_NAME): $(OBJECTS_PATH)
	@mkdir -p $(OUT_DIR)
	@ar rcs $(OUT_DIR)/$(LIB_NAME) $(OBJECTS_PATH)
	@echo Disasm library in $(OUT_DIR)/$(LIB_NAME)

# We need to mkdir inside the rule, so each subfolder gets created,
# and avoiding conflicts:
# ./linux/example.c => $(INT_DIR)/linux/example.o
# ./example.c => $(INT_DIR)/example.o
$(INT_DIR)/%.o: %.c $(DEP_DIR)/%.d
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$<) >/dev/null
	@echo $<
	@$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@
	@mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d

# So 'make' won't fail if the dependecy file doesn't exist
$(DEP_DIR)/%.d: ;

# Don't delete the dependecy files
.PRECIOUS: $(DEP_DIR)/%.d

# Include all the dependecy files (this will trigger a rebuild if a header is changed)
-include $(patsubst %,$(DEP_DIR)/%.d,$(basename $(SRC_FILES)))

clean_lib_file:
	@rm $(OUT_DIR)/$(LIB_NAME) 2>/dev/null ||:

clean_int_dir:
	@rm -r $(INT_DIR) 2>/dev/null ||:

clean_dep_dir:
	@rm -r $(DEP_DIR) 2>/dev/null ||:

clean: clean_lib_file clean_int_dir clean_dep_dir

install: all
	install -d $(DESTDIR)$(PREFIX)/lib/
	install -m 644 $(OUT_DIR)/$(LIB_NAME) $(DESTDIR)$(PREFIX)/lib/
	install -d $(DESTDIR)$(PREFIX)/include/bddisasm
	cp -r ../inc/* $(DESTDIR)$(PREFIX)/include/bddisasm/
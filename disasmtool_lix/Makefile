.PHONY: all last_build debug release

CMAKE_BUILD_DIR = build

CPUCOUNT := $(shell grep -c "^processor" /proc/cpuinfo)

ifneq (,$(CPUCOUNT))
EXTRA_MAKE_ARGS := -j$(CPUCOUNT)
endif

# NINJA := $(shell which ninja 2>/dev/null)

ifneq (,$(NINJA))
GENERATOR := "Ninja"
else
GENERATOR := "Unix Makefiles"
endif

$(shell if [ ! -d $(CMAKE_BUILD_DIR) ]; then mkdir $(CMAKE_BUILD_DIR); fi)

all: last_build

last_build:
	@cmake -H. -G$(GENERATOR) -B$(CMAKE_BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@cmake --build $(CMAKE_BUILD_DIR) -- $(EXTRA_MAKE_ARGS)

debug: CMAKE_BUILD_TYPE := Debug
debug: build

release: CMAKE_BUILD_TYPE := Release
release: build

clean:
	@if [ -d $(CMAKE_BUILD_DIR) ]; then cmake --build $(CMAKE_BUILD_DIR) --target clean; fi

reset:
	@if [ -d $(CMAKE_BUILD_DIR) ]; then rm -r $(CMAKE_BUILD_DIR); fi



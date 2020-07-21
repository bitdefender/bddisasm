.PHONY: all bddisasm bdshemu

all: bddisasm bdshemu

bddisasm:
	@$(MAKE) --no-print-directory -C bddisasm
	@$(MAKE) --no-print-directory RELEASE=y -C bddisasm

bdshemu:
	@$(MAKE) --no-print-directory -C bdshemu
	@$(MAKE) --no-print-directory RELEASE=y -C bdshemu

clean:
	@$(MAKE) --no-print-directory -C bddisasm clean
	@$(MAKE) --no-print-directory RELEASE=y -C bddisasm clean
	@$(MAKE) --no-print-directory -C bdshemu clean
	@$(MAKE) --no-print-directory RELEASE=y -C bdshemu clean
    
install:
	@$(MAKE) --no-print-directory RELEASE=y -C bddisasm install
	@$(MAKE) --no-print-directory RELEASE=y -C bdshemu install
	
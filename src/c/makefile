SUBDIRS := $(shell find . -mindepth 3 -maxdepth 4 -type f -name makefile -exec dirname {} \;)
all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir || exit 1; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean || exit 1; \
	done

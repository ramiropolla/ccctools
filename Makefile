include config.mak

COMPONENTS=cctools/ar cctools/as cctools/ld cctools/libmacho cctools/misc \
	cctools/otool ld64

.PHONY: $(COMPONENTS) cctools/libstuff

default: $(COMPONENTS)

$(COMPONENTS): cctools/libstuff

$(COMPONENTS) cctools/libstuff:
	cd $@ && $(MAKE) -f Makefile.ccc

clean distclean install:
	for c in $(COMPONENTS) cctools/libstuff; do           \
	    (cd $$c && $(MAKE) -f Makefile.ccc $@) || exit 1; \
	done
	[ "$@" = "distclean" ] && rm -f config.mak config.h || echo


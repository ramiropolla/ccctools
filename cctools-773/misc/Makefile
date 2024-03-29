export USE_APPLE_PB_SUPPORT = all
OFLAG = -O
LTO = -DLTO_SUPPORT
TRIE = -DTRIE_SUPPORT
RC_OS = macos
X_CFLAGS = -fno-builtin-round -fno-builtin-trunc

ifneq "" "$(SDKROOT)"
  SDK = -isysroot $(SDKROOT)
  CC = $(shell xcrun -find -sdk $(SDKROOT) cc)
endif
ifeq "clang" "$(notdir $(CC))"
  CFLAGS = $(OFLAG) $(LTO) $(TRIE) -g -Wall -Wno-format-nonliteral \
	 -I$(SRCROOT) -I$(SRCROOT)/../include -I$(OFILE_DIR) $(SDK)
else
  CFLAGS = $(OFLAG) $(LTO) $(TRIE) -g -Wall $(X_CFLAGS) \
	 -I$(SRCROOT) -I$(SRCROOT)/../include -I$(OFILE_DIR) $(SDK)
endif

USE_DEPENDENCY_FILE := $(shell if [ "$(notdir $(CC))" != "mwccppc" ] && \
				  [ "$(RC_XBS)" != "YES" ]; then \
	     echo YES ; else echo NO ; \
	     fi; )

LIBSTUFF = -L$(SYMROOT)/../libstuff -lstuff

ifeq "" "$(TRIE)"
  LIB_PRUNETRIE =
  else
  LIB_PRUNETRIE = /usr/local/lib/libprunetrie.a
endif

ifneq "" "$(wildcard /bin/mkdirs)"
  MKDIRS = /bin/mkdirs
else
  MKDIRS = /bin/mkdir -p
endif

SRCROOT = .
OBJROOT = .
OFILE_DIR = $(OBJROOT)
VPATH = $(OFILE_DIR)
SYMROOT = .

BINDIR = /bin
USRBINDIR = /usr/bin
LOCBINDIR = /usr/local/bin
LOCLIBDIR = /usr/local/lib

CFILES1 = libtool.c
CFILES2 = lipo.c size.c strings.c nm.c checksyms.c inout.c \
	 indr.c strip.c segedit.c cmpdylib.c \
	 pagestuff.c redo_prebinding.c seg_addr_table.c \
	 check_dylib.c seg_hack.c install_name_tool.c \
	 codesign_allocate.c
INSTALL_FILES = $(CFILES1) $(CFILES2) Makefile notes Info.plist

PROGS = lipo.NEW size.NEW strings.NEW nm.NEW libtool.NEW checksyms.NEW \
	indr.NEW strip.NEW nmedit.NEW segedit.NEW cmpdylib.NEW \
	pagestuff.NEW redo_prebinding.NEW seg_addr_table.NEW check_dylib.NEW \
	seg_hack.NEW install_name_tool.NEW codesign_allocate.NEW

teflon_all macos_all: $(PROGS)

nextstep_all: $(PROGS)

all:	$(OFILE_DIR) $(SYMROOT) $(RC_OS)_all lib_ofiles

lib_ofiles: $(OFILE_DIR) $(SYMROOT) libredo_prebinding.a

vers.c:
	vers_string -c $(VERS_STRING_FLAGS) cctools_misc > $(OFILE_DIR)/$@
	echo "const char apple_version[]= " \"`vers_string $(VERS_STRING_FLAGS) -f cctools`\" ";" >> \
	$(OFILE_DIR)/$@

ifeq "NO" "$(USE_DEPENDENCY_FILE)"
.c.o:
	$(CC) $(CFLAGS) $(RC_CFLAGS) -c -o $(OFILE_DIR)/$*.o $<
else
.c.o:
	$(CC) $(CFLAGS) $(RC_CFLAGS) -c -MD -dependency-file \
		$(OFILE_DIR)/$*.d -o $(OFILE_DIR)/$*.o $<
	md -d -u $(OFILE_DIR)/Makedep $(OFILE_DIR)/$*.d
endif

libredo_prebinding.o: redo_prebinding.c
ifeq "NO" "$(USE_DEPENDENCY_FILE)"
	$(CC) $(CFLAGS) $(RC_CFLAGS) -c \
		-o $(OFILE_DIR)/$*.o $(SRCROOT)/redo_prebinding.c -DLIBRARY_API
else
	$(CC) $(CFLAGS) $(RC_CFLAGS) -c -MD -dependency-file $(OFILE_DIR)/$*.d \
		-o $(OFILE_DIR)/$*.o $(SRCROOT)/redo_prebinding.c -DLIBRARY_API
	md -d -u $(OFILE_DIR)/Makedep $(OFILE_DIR)/$*.d
endif

checksyms.NEW: checksyms.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/checksyms.private.o \
		$(OFILE_DIR)/checksyms.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/checksyms.NEW \
		$(OFILE_DIR)/checksyms.private.o

cmpdylib.NEW: cmpdylib.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/cmpdylib.private.o \
		$(OFILE_DIR)/cmpdylib.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/cmpdylib.NEW \
		$(OFILE_DIR)/cmpdylib.private.o

lipo.NEW: lipo.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/lipo.private.o \
		$(OFILE_DIR)/lipo.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/lipo.NEW \
		$(OFILE_DIR)/lipo.private.o

size.NEW: size.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/size.private.o \
		$(OFILE_DIR)/size.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/size.NEW \
		$(OFILE_DIR)/size.private.o

strings.NEW: strings.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/strings.private.o \
		$(OFILE_DIR)/strings.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/strings.NEW \
		$(OFILE_DIR)/strings.private.o

nm.NEW: nm.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/nm.private.o \
		$(OFILE_DIR)/nm.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/nm.NEW \
		$(OFILE_DIR)/nm.private.o

libtool.NEW: libtool.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/libtool.private.o \
		$(OFILE_DIR)/libtool.o $(OFILE_DIR)/vers.o \
		$(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/libtool.NEW \
		$(OFILE_DIR)/libtool.private.o

redo_prebinding.NEW: redo_prebinding.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/redo_prebinding.private.o \
		$(OFILE_DIR)/redo_prebinding.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/redo_prebinding.NEW \
		$(OFILE_DIR)/redo_prebinding.private.o

libredo_prebinding.a: libredo_prebinding.o
	$(CC) $(RC_CFLAGS) -nostdlib -r -o $(OBJROOT)/libredo.o \
		libredo_prebinding.o $(LIBSTUFF)
	libtool -static -o $(SYMROOT)/libredo_prebinding.a $(OBJROOT)/libredo.o

seg_addr_table.NEW: seg_addr_table.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/seg_addr_table.private.o \
		$(OFILE_DIR)/seg_addr_table.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/seg_addr_table.NEW \
		$(OFILE_DIR)/seg_addr_table.private.o

check_dylib.NEW: check_dylib.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/check_dylib.private.o \
		$(OFILE_DIR)/check_dylib.o $(OFILE_DIR)/vers.o \
		$(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/check_dylib.NEW \
		$(OFILE_DIR)/check_dylib.private.o

inout.NEW: inout.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/inout.private.o \
		$(OFILE_DIR)/inout.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/inout.NEW \
		$(OFILE_DIR)/inout.private.o

codesign_allocate.NEW: codesign_allocate.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/codesign_allocate.private.o \
		$(OFILE_DIR)/codesign_allocate.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/codesign_allocate.NEW \
		-sectcreate __TEXT __info_plist Info.plist \
		$(OFILE_DIR)/codesign_allocate.private.o

seg_hack.NEW: seg_hack.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/seg_hack.private.o \
		$(OFILE_DIR)/seg_hack.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/seg_hack.NEW \
		$(OFILE_DIR)/seg_hack.private.o

install_name_tool.NEW: install_name_tool.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/install_name_tool.private.o \
		$(OFILE_DIR)/install_name_tool.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/install_name_tool.NEW \
		$(OFILE_DIR)/install_name_tool.private.o

indr.NEW: indr.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/indr.private.o \
		$(OFILE_DIR)/indr.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/indr.NEW \
		$(OFILE_DIR)/indr.private.o

strip.NEW: strip.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/strip.private.o \
		$(OFILE_DIR)/strip.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CXX) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/strip.NEW \
		$(OFILE_DIR)/strip.private.o $(LIB_PRUNETRIE)

nmedit.NEW: nmedit.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/nmedit.private.o \
		$(OFILE_DIR)/nmedit.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/nmedit.NEW \
		$(OFILE_DIR)/nmedit.private.o

nmedit.o: strip.c
ifeq "NO" "$(USE_DEPENDENCY_FILE)"
	$(CC) $(CFLAGS) $(RC_CFLAGS) -c \
		-o $(OFILE_DIR)/nmedit.o $(SRCROOT)/strip.c -DNMEDIT
else
	$(CC) $(CFLAGS) $(RC_CFLAGS) -c -MD -dependency-file $(OFILE_DIR)/$*.d \
		-o $(OFILE_DIR)/nmedit.o $(SRCROOT)/strip.c -DNMEDIT
	md -d -u $(OFILE_DIR)/Makedep $(OFILE_DIR)/$*.d
endif

segedit.NEW: segedit.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/segedit.private.o \
		$(OFILE_DIR)/segedit.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/segedit.NEW \
		$(OFILE_DIR)/segedit.private.o

pagestuff.NEW: pagestuff.o vers.o
	$(CC) $(RC_CFLAGS) -nostdlib -r \
		-o $(OBJROOT)/pagestuff.private.o \
		$(OFILE_DIR)/pagestuff.o $(OFILE_DIR)/vers.o $(LIBSTUFF)
	$(CC) $(RC_CFLAGS) $(SDK) -o $(SYMROOT)/pagestuff.NEW \
		$(OFILE_DIR)/pagestuff.private.o


clean: shlib_clean
	-rm -f \
	$(SYMROOT)/lipo.NEW \
	$(SYMROOT)/size.NEW \
	$(SYMROOT)/strings.NEW \
	$(SYMROOT)/nm.NEW \
	$(SYMROOT)/libtool.NEW \
	$(SYMROOT)/checksyms.NEW \
	$(SYMROOT)/seg_addr_table.NEW \
	$(SYMROOT)/check_dylib.NEW \
	$(SYMROOT)/inout.NEW \
	$(SYMROOT)/codesign_allocate.NEW \
	$(SYMROOT)/seg_hack.NEW \
	$(SYMROOT)/install_name_tool.NEW \
	$(SYMROOT)/codesign_allocate.NEW \
	$(SYMROOT)/indr.NEW \
	$(SYMROOT)/strip.NEW \
	$(SYMROOT)/nmedit.NEW \
	$(SYMROOT)/segedit.NEW \
	$(SYMROOT)/pagestuff.NEW \
	$(SYMROOT)/redo_prebinding.NEW \
	$(SYMROOT)/libredo_prebinding.a \
	$(SYMROOT)/cmpdylib.NEW

shlib_clean:
	-rm -f \
	$(OFILE_DIR)/vers.c \
	$(OFILE_DIR)/vers.o \
	$(OFILE_DIR)/lipo.o \
	$(OFILE_DIR)/size.o \
	$(OFILE_DIR)/strings.o \
	$(OFILE_DIR)/nm.o \
	$(OFILE_DIR)/libtool.o \
	$(OFILE_DIR)/make.h \
	$(OFILE_DIR)/checksyms.o \
	$(OFILE_DIR)/inout.o \
	$(OFILE_DIR)/codesign_allocate.o \
	$(OFILE_DIR)/seg_hack.o \
	$(OFILE_DIR)/install_name_tool.o \
	$(OFILE_DIR)/indr.o \
	$(OFILE_DIR)/segedit.o \
	$(OFILE_DIR)/pagestuff.o \
	$(OFILE_DIR)/redo_prebinding.o \
	$(OFILE_DIR)/libredo_prebinding.o \
	$(OFILE_DIR)/libredo.o \
	$(OFILE_DIR)/seg_addr_table.o \
	$(OFILE_DIR)/check_dylib.o \
	$(OFILE_DIR)/cmpdylib.o \
	$(OFILE_DIR)/nmedit.o \
	$(OFILE_DIR)/strip.o \
	$(OFILE_DIR)/lipo.private.o \
	$(OFILE_DIR)/size.private.o \
	$(OFILE_DIR)/strings.private.o \
	$(OFILE_DIR)/nm.private.o \
	$(OFILE_DIR)/libtool.private.o \
	$(OFILE_DIR)/checksyms.private.o \
	$(OFILE_DIR)/inout.private.o \
	$(OFILE_DIR)/codesign_allocate.private.o \
	$(OFILE_DIR)/seg_hack.private.o \
	$(OFILE_DIR)/install_name_tool.private.o \
	$(OFILE_DIR)/indr.private.o \
	$(OFILE_DIR)/segedit.private.o \
	$(OFILE_DIR)/pagestuff.private.o \
	$(OFILE_DIR)/redo_prebinding.private.o \
	$(OFILE_DIR)/seg_addr_table.private.o \
	$(OFILE_DIR)/check_dylib.private.o \
	$(OFILE_DIR)/cmpdylib.private.o \
	$(OFILE_DIR)/nmedit.private.o \
	$(OFILE_DIR)/strip.private.o \
	-rm -f $(OFILE_DIR)/Makedep make.out

install: all $(RC_OS)_install

teflon_install macos_install: common_install
	$(MKDIRS) $(DSTROOT)$(USRBINDIR)
	install -c -s -m 555 $(SYMROOT)/strip.NEW $(DSTROOT)$(USRBINDIR)/strip
	install -c -s -m 555 $(SYMROOT)/strings.NEW \
		$(DSTROOT)$(USRBINDIR)/strings
	install -c -s -m 555 $(SYMROOT)/size.NEW $(DSTROOT)$(USRBINDIR)/size
	install -c -s -m 555 $(SYMROOT)/nm.NEW $(DSTROOT)$(USRBINDIR)/nm
	install -c -s -m 555 $(SYMROOT)/libtool.NEW \
		$(DSTROOT)$(USRBINDIR)/libtool
	(cd $(DSTROOT)$(USRBINDIR); rm -f ranlib; ln -s libtool ranlib)
	install -c -s -m 555 $(SYMROOT)/lipo.NEW $(DSTROOT)$(USRBINDIR)/lipo
	install -c -s -m 555 $(SYMROOT)/segedit.NEW \
		$(DSTROOT)$(USRBINDIR)/segedit
	install -c -s -m 555 $(SYMROOT)/cmpdylib.NEW \
			  $(DSTROOT)$(USRBINDIR)/cmpdylib
	install -c -s -m 555 $(SYMROOT)/pagestuff.NEW \
			  $(DSTROOT)$(USRBINDIR)/pagestuff
	install -c -s -m 555 $(SYMROOT)/redo_prebinding.NEW \
			  $(DSTROOT)$(USRBINDIR)/redo_prebinding
	install -c -s -m 555 $(SYMROOT)/nmedit.NEW $(DSTROOT)$(USRBINDIR)/nmedit
	(cd $(DSTROOT)$(LOCBINDIR); rm -f nmedit; \
	 ln -s $(USRBINDIR)/nmedit nmedit)
	install -c -s -m 555 $(SYMROOT)/install_name_tool.NEW \
			  $(DSTROOT)$(USRBINDIR)/install_name_tool
	install -c -s -m 555 $(SYMROOT)/codesign_allocate.NEW \
			  $(DSTROOT)$(USRBINDIR)/codesign_allocate

nextstep_install: common_install
	$(MKDIRS) $(DSTROOT)$(BINDIR)
	$(MKDIRS) $(DSTROOT)$(LOCBINDIR)
	install -c -s -m 555 $(SYMROOT)/strip.NEW $(DSTROOT)$(BINDIR)/strip
	install -c -s -m 555 $(SYMROOT)/strings.NEW $(DSTROOT)$(BINDIR)/strings
	install -c -s -m 555 $(SYMROOT)/size.NEW $(DSTROOT)$(BINDIR)/size
	install -c -s -m 555 $(SYMROOT)/nm.NEW $(DSTROOT)$(BINDIR)/nm
	install -c -s -m 555 $(SYMROOT)/libtool.NEW $(DSTROOT)$(BINDIR)/libtool
	(cd $(DSTROOT)$(BINDIR); rm -f ranlib; ln -s libtool ranlib)
	install -c -s -m 555 $(SYMROOT)/lipo.NEW $(DSTROOT)$(BINDIR)/lipo
	install -c -s -m 555 $(SYMROOT)/segedit.NEW $(DSTROOT)$(BINDIR)/segedit
	install -c -s -m 555 $(SYMROOT)/cmpdylib.NEW \
		$(DSTROOT)$(BINDIR)/cmpdylib
	install -c -s -m 555 $(SYMROOT)/pagestuff.NEW \
		$(DSTROOT)$(BINDIR)/pagestuff
	install -c -s -m 555 $(SYMROOT)/redo_prebinding.NEW \
			  $(DSTROOT)$(BINDIR)/redo_prebinding
	install -c -s -m 555 $(SYMROOT)/nmedit.NEW $(DSTROOT)$(LOCBINDIR)/nmedit

common_install:
	$(MKDIRS) $(DSTROOT)$(LOCBINDIR)
	install -c -s -m 555 $(SYMROOT)/checksyms.NEW \
			  $(DSTROOT)$(LOCBINDIR)/checksyms
	install -c -s -m 555 $(SYMROOT)/seg_addr_table.NEW \
			  $(DSTROOT)$(LOCBINDIR)/seg_addr_table
	install -c -s -m 555 $(SYMROOT)/check_dylib.NEW \
			  $(DSTROOT)$(LOCBINDIR)/check_dylib
	install -c -s -m 555 $(SYMROOT)/indr.NEW $(DSTROOT)$(LOCBINDIR)/indr
	install -c -s -m 555 $(SYMROOT)/seg_hack.NEW \
			  $(DSTROOT)$(LOCBINDIR)/seg_hack

lib_ofiles_install: lib_ofiles
	$(MKDIRS) $(DSTROOT)$(LOCLIBDIR)
	install -p -c -m 444 $(SYMROOT)/libredo_prebinding.a \
		 $(DSTROOT)$(LOCLIBDIR)/libredo_prebinding.a

installsrc:
	$(MKDIRS) $(SRCROOT)
	chmod 755 $(SRCROOT)
	gnutar cf - $(INSTALL_FILES) | (cd $(SRCROOT); gnutar xf -)
	chmod 444 $(SRCROOT)/*

$(OFILE_DIR) $(SYMROOT):
	$(MKDIRS) $@

-include $(OFILE_DIR)/Makedep

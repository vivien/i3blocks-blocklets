PREFIX = $(HOME)/.local

_INSTALLDIR = $(PREFIX)/libexec/i3blocks

_MAKEBLOCKS := $(dir $(wildcard */Makefile))
all: build
build: $(_MAKEBLOCKS)
$(_MAKEBLOCKS):
		$(MAKE) -C $@

install: all do-install

installdirs:
		install -d $(_INSTALLDIR)

_BLOCKS := $(shell find . -type f -executable -not -path './.*')

do-install: installdirs $(_BLOCKS)
$(_BLOCKS):
		install -m755 $@ $(_INSTALLDIR)

uninstall:
		rm -f $(foreach block,$(_BLOCKS),$(_INSTALLDIR)$(notdir $(block)))

.PHONY: $(_MAKEBLOCKS) $(_BLOCKS) build do-install all install uninstall

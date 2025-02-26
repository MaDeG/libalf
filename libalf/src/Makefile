# $Id$
# libalf Makefile

.PHONY: all clean install uninstall

include ../config.mk

VERSIONTAG:=$(shell cat "../version" 2>/dev/null)
ifeq (${VERSIONTAG},)
GITHASH := $(shell git --no-pager rev-parse --short HEAD)
DIRTYFLAG := $(shell git --no-pager status -s | wc -c | sed -e 's/^[1-9][0-9]*$$/-DIRTY/' -e 's/^0$$//')
VERSION := git-${GITHASH}${DIRTYFLAG}
LIBVERSIONTAG := .git
else
# release branch
VERSION := v${VERSIONTAG}
LIBVERSIONTAG := .${VERSIONTAG}
endif

CPPFLAGS+=-Wall -I../include/ -I${INCLUDEDIR} -shared -fpic
CPPFLAGS+=-Wextra
CPPFLAGS+=-DVERSION="\"${VERSION}\""
# for ubuntu, try disabling stack-smashing due to linker failures:
#CPPFLAGS+=-fno-stack-protector
LDFLAGS+=-shared -L${LIBDIR}

MINISAT_OBJECTS=minisat/Solver.o
OBJECTS=alf.o alphabet.o conjecture.o basic_string.o learning_algorithm.o logger.o normalizer_msc.o statistics.o ${MINISAT_OBJECTS}

# Choose OS
ifeq (${OS}, Windows_NT)
  LDFLAGS += -lws2_32
  TARGET=libalf.dll
else
  TARGET=libalf.so
endif

INSTALL_SHARED_NAME=${TARGET}${LIBVERSIONTAG}
INSTALL_STATIC_NAME=libalf.a${LIBVERSIONTAG}

all:	${TARGET}

libalf.so: ${OBJECTS}
	${CXX} $(OBJECTS) $(LDFLAGS) -o $@ 

libalf.dll: ${OBJECTS}
	${CXX} $(OBJECTS) $(LDFLAGS) -o $@ 

libalf.a: ${OBJECTS}
	${AR} rcs $@ $(OBJECTS)

clean:
	-rm -f *.o minisat/*.o alf.dll libalf.so libalf.a 2>&1

install: ${TARGET} libalf.a
	@echo
	@echo installing libalf library to ${DESTDIR}${LIBDIR} ...
	@echo
	-install -v -m 755 -d ${DESTDIR}${LIBDIR}
	install -T -v -m 755 ${TARGET} ${DESTDIR}${LIBDIR}/${INSTALL_SHARED_NAME}
	install -T -v -m 755 libalf.a  ${DESTDIR}${LIBDIR}/${INSTALL_STATIC_NAME}
	# symlinks
	-rm -f ${DESTDIR}${LIBDIR}/${TARGET} 2>&1
	ln -s ${LIBDIR}/${INSTALL_SHARED_NAME} ${DESTDIR}${LIBDIR}/${TARGET}
	-rm -f ${DESTDIR}${LIBDIR}/libalf.a 2>&1
	ln -s ${LIBDIR}/${INSTALL_STATIC_NAME} ${DESTDIR}${LIBDIR}/libalf.a
	-test `id -u` -eq 0 && /sbin/ldconfig; true

uninstall:
	@echo
	@echo removing libalf library from ${DESTDIR}${LIBDIR} ...
	@echo
	rm -f ${DESTDIR}${LIBDIR}/${INSTALL_SHARED_NAME}
	rm -f ${DESTDIR}${LIBDIR}/${INSTALL_STATIC_NAME}
	rm -f ${DESTDIR}${LIBDIR}/${TARGET}
	rm -f ${DESTDIR}${LIBDIR}/libalf.a
	-test `id -u` -eq 0 && /sbin/ldconfig; true

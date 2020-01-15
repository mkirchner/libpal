# $Id: Makefile,v 1.2 2002/05/06 15:43:15 mkirchner Exp $

LIBPAL_VERSION=0.1.3
LIBPAL_SHLIB=libpal.so.$(LIBPAL_VERSION)
LIBPAL_LIB=libpal.a

INSTALL=/usr/bin/install
INSTALL_PREFIX=/tmp

CC=gcc
CCFLAGS=-Wall
CC_SHFLAGS=-fpic
OBJS=checksum.o packet.o socket.o tcp.o udp.o ip.o icmp.o misc.o
LIBS=
DEFS=
LD=gcc
LD_SHFLAGS=-shared
AR=/usr/bin/ar
RANLIB=/usr/bin/ranlib

all: shared static objclean

#shared libs
$(LIBPAL_SHLIB): $(OBJS)
	$(LD) $(LD_SHFLAGS) -o $@ $(OBJS) $(LIBS)
	ln -s $(LIBPAL_SHLIB) libpal.so

#static libs will unset CC_SHFLAGS
$(LIBPAL_LIB): CC_SHFLAGS=
$(LIBPAL_LIB): $(OBJS)
	$(AR) rc $@ $(OBJS)
	$(RANLIB) $@

%.o: %.c
	$(CC) -c -o $@ $(CCFLAGS) $(CC_SHFLAGS) $(DEFS) $<

static: objclean
	make $(LIBPAL_LIB)

shared: objclean
	make $(LIBPAL_SHLIB)

install:
	$(INSTALL) -C -D -m 0644 libpal.h $(INSTALL_PREFIX)/include/libpal.h
	@if [ -f $(LIBPAL_SHLIB) ] ; then \
		 echo "$(INSTALL) -C -D -m 0755 $(LIBPAL_SHLIB) \
			$(INSTALL_PREFIX)/lib/$(LIBPAL_SHLIB)" ;\
		 $(INSTALL) -C -D -m 0755 $(LIBPAL_SHLIB) \
			$(INSTALL_PREFIX)/lib/$(LIBPAL_SHLIB) ;\
		 echo "cp -a libpal.so $(INSTALL_PREFIX)/lib/" ; \
		 cp -a libpal.so $(INSTALL_PREFIX)/lib/ ; \
	else \
		echo "***Warning: $(LIBPAL_SHLIB) not found."; \
		echo "***Warning: won't install shared libs."; \
	fi
	@if [ -f $(LIBPAL_LIB) ] ; then \
		 echo "$(INSTALL) -C -p -D -m 0644 $(LIBPAL_LIB) \
			$(INSTALL_PREFIX)/lib/$(LIBPAL_LIB)" ;\
		 $(INSTALL) -C -p -D -m 0644 $(LIBPAL_LIB) \
			$(INSTALL_PREFIX)/lib/$(LIBPAL_LIB) ;\
	else \
		echo "***Warning: $(LIBPAL_LIB) not found."; \
		echo "***Warning: won't install static libs."; \
	fi

objclean:
	rm -f *.o

clean:
	rm -f *.o $(LIBPAL_LIB) $(LIBPAL_SHLIB) libpal.so


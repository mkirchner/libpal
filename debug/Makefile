CC=gcc
CCFLAGS=-Wall #-fPIC
OBJS=error.o
LIBS= -lpal -lnsl
LDFLAGS= -L..
DEFS=
LD=ld

test: test.c $(OBJS)
	$(CC) $(CCFLAGS) $(DEFS) -o test test.c $(OBJS) $(LIBS)

udptest: udptest.c $(OBJS)
	$(CC) $(CCFLAGS) $(DEFS) -o udptest udptest.c $(OBJS) $(LIBS)

icmptest: icmptest.c $(OBJS)
	$(CC) $(CCFLAGS) $(DEFS) -o icmptest.o -c icmptest.c
	$(LD) $(LDFLAGS) -o icmptest icmptest.o error.o $(LIBS)

testlib: test.c $(OBJS)
	$(CC) $(CCFLAGS) $(DEFS) -o testlib test.c $(LIBS)

error.o: error.c
	$(CC) $(CCFLAGS) $(DEFS) -o error.o -c error.c

clean:
	rm -f *.o test udptest icmptest

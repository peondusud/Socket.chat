VERSION=1.1

LIBS=libsplitscreen.a
TESTS=test1 test2

LDLIBS=-lncurses

DISTDIR=splitscreen-$(VERSION)
DISTFILES= splitscreen.c splitscreen.h test1.c test2.c README Makefile
DISTARCHIVE=splitscreen-$(VERSION).zip

all: $(LIBS) $(TESTS)

%: %.c
	$(CC) $(CFLAGS) -o $* $*.c $(OBJS) $(LIBS) $(LDLIBS)

clean:
	rm -r $(LIBS) $(TESTS) *.o

# Dependencies
testwin: $(LIBS)

libsplitscreen.a: splitscreen.o
	$(AR) $(ARFLAGS) $@ $?

splitscreen.o: splitscreen.h

dist:
	rm -rf $(DISTDIR) $(DISTARCHIVE)
	mkdir $(DISTDIR)
	for f in $(DISTFILES); do cp $$f $(DISTDIR); done
	zip -r $(DISTARCHIVE) $(DISTDIR)
	rm -r $(DISTDIR)


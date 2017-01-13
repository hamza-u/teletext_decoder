CC ?= gcc
CCFLAGS += -std=c99 -pedantic -O3 -Wall -Werror
LDFLAGS += -Ldeps/libtelxcc -ltelxcc
DEST := /usr/local

all : m2ts telxcc
	@-strip --strip-unneeded $^

man : telxcc.1.gz

.PHONY : clean
clean :
	-rm -f telxcc m2ts *.o *.1.gz

m2ts : m2ts.o
	@printf "%-16s%s\n" LD $@
	@$(CC) -o $@ $^

telxcc : main.o
	@printf "%-16s%s\n" LD $@
	@$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	@printf "%-16s%s\n" CC $@
	@$(CC) -c $(CCFLAGS) -o $@ $<

%.1.gz : %.1
	@printf "%-16s%s\n" GZIP $@
	@gzip -c -9 $< > $@

install : all man
	sudo cp telxcc $(DEST)/bin
	sudo cp m2ts $(DEST)/bin
	sudo mkdir -p $(DEST)/share/man/man1
	sudo cp telxcc.1.gz $(DEST)/share/man/man1

uninstall :
	sudo rm $(DEST)/bin/telxcc
	sudo rm $(DEST)/bin/m2ts
	sudo rm $(DEST)/share/man/man1/telxcc.1.gz

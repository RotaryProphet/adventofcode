DIRS := $(dir $(wildcard */))
TGTS := $(patsubst %, %code, $(DIRS))
CFLAGS := -Wall -ggdb

ALL : $(TGTS)

clean :
	rm -f $(TGTS)

4/code : 4/code.c
	gcc $(CFLAGS) -o $@ $< -lssl -lcrypto

%/code : %/code.c
	gcc $(CFLAGS) -o $@ $<

% : %/code FORCE
	$< < $@/input.txt

FORCE :

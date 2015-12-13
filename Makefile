DIRS := $(dir $(wildcard */))
TGTS := $(patsubst %, %code, $(DIRS))
CFLAGS := -Wall -ggdb -std=c99

ALL : $(TGTS)

clean :
	rm -f $(TGTS)

12/code : 12/code.c
	gcc $(CFLAGS) $(shell pkg-config --cflags json-c) -o $@ $< $(shell pkg-config --libs json-c)
4/code : 4/code.c
	gcc $(CFLAGS) -o $@ $< -lssl -lcrypto

%/code : %/code.c
	gcc $(CFLAGS) -o $@ $<

% : %/code FORCE
	$< < $@/input.txt

FORCE :

DIRS := $(dir $(wildcard */))
TGTS := $(patsubst %, %code, $(DIRS))
CFLAGS := -Wall -ggdb -std=gnu99

ALL : $(TGTS)

clean :
	rm -f $(TGTS)

5/code : 5/code.c
	gcc $(CFLAGS) -o $@ $< -lssl -lcrypto

14/code : 14/code.c
	gcc $(CFLAGS) -o $@ $< -lssl -lcrypto

%/code : %/code.c
	gcc $(CFLAGS) -o $@ $<

% : %/code FORCE
	$< < $@/input.txt

FORCE :

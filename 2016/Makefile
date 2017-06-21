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

17/code : 17/code.c
	gcc $(CFLAGS) -o $@ $< -lssl -lcrypto

25/code : 25/code.c
	gcc -ggdb -std=gnu99 -o $@ $<

%/code : %/code.c
	gcc $(CFLAGS) -o $@ $<

% : %/code FORCE
	$< < $@/input.txt

FORCE :

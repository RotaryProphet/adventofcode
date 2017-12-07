DIRS := $(dir $(wildcard */))
TGTS := $(patsubst %, %code, $(DIRS))
CFLAGS := -Wall -ggdb -std=gnu99

ALL : $(TGTS)

clean :
	rm -f $(TGTS)

%/code : %/code.c
	gcc -o $@ -Wall $<

% : %/code FORCE
	$< < $@/input.txt

FORCE :

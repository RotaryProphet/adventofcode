DIRS := $(dir $(wildcard */))
TGTS := $(patsubst %, %code, $(DIRS))

ALL : $(TGTS)

clean :
	rm -f $(TGTS)

4/code : 4/code.c
	gcc -Wall -o $@ $< -lssl -lcrypto

%/code : %/code.c
	gcc -Wall -o $@ $<

% : %/code FORCE
	$< < $@/input.txt

FORCE :

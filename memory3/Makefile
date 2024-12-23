P=memory3
OBJECTS=
CFLAGS=-Wall -Werror -O2 -std=c99
LDLIBS=

$(P): $(OBJECTS)

test:
	$(eval TMP_FILE=$(shell mktemp))
	cc -DTEST -DPROC_MEMINFO=\"$(TMP_FILE)\" $(CFLAGS) $(P).c -o $(P).test
	./$(P).test
	$(shell rm "$(TMP_FILE)")
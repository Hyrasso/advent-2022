CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -g

ODIR=obj
UTIL=utils

day%: $(ODIR)/day%.o $(ODIR)/vector.o
	$(CC) -o bin/$@ $^

test_%: $(ODIR)/test_%.o
	$(CC) -o bin/$@ $^

$(ODIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(ODIR)/%.o: $(UTIL)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
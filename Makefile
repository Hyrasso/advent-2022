CC=gcc
CFLAGS=-Wall -pedantic -std=c11

ODIR=obj
UTIL=utils

day%: $(ODIR)/day%.o $(ODIR)/vector.o
	$(CC) -o bin/$@ $^

$(ODIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(ODIR)/%.o: $(UTIL)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
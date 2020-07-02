CC=gcc
CFLAGS=-g
test:main.o memstat.o
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	rm test *.o -rf

CFLAGS := -fPIC -shared
LDFLAGS := -ldl
CC=gcc

all:hook.c ldpreload.c
	$(CC) $(CFLAGS) hook.c -o hook.so $(LDFLAGS)
	$(CC) -o test ldpreload.c

.PHONY:clean

clean:
	rm -rf hook.so test


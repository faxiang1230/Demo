CC=gcc
CFLAGS=-c -fno-builtin -nostdlib -fno-stack-protector
OBJ= hello minicrt entry
all: $(OBJ)

minicrt:malloc.c mini_crt_syscall.c stdio.c
	$(CC) $^ $(CFLAGS)
	ar -rs minicrt.a malloc.o mini_crt_syscall.o stdio.o

entry:entry.c
	$(CC) $^ $(CFLAGS)

hello:minicrt entry hello.o
	$(CC) hello.c $(CFLAGS) -ggdb
	ld -static -e mini_crt_entry entry.o hello.o minicrt.a -o hello


.phony:clean

clean:
	@rm -rf *.o minicrt.a hello

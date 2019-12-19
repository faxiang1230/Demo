CC=gcc
CFLAGS=-g
OBJ= rebuildimage inject payload
all: $(OBJ)

rebuildimage:rebuild.o ptrace_interface.o
	$(CC) $^ -o $@ $(CFLAGS)

inject:inject.o ptrace_interface.o
	$(CC) $^ -o $@ $(CFLAGS)

payload:payload.o
	 $(CC) -fpic -pie -nostdlib $^ -o $@

.phony:clean

clean:
	rm *.o $(OBJ)

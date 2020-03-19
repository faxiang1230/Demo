CC=gcc
CFLAGS+=-g -lpthread -lrt
timer_test:main.o timer.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm *.o timer_test

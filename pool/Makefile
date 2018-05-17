all:mempool threadpool
mempool:main.o mempool.o
CFLAGS += -g
LDFLAGS += -lpthread
threadpool:threadpool.o threadtest.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
clean:
	rm -rf *.o threadpool mempool

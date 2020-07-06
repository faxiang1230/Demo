all: main libmalloc_hook_library.so

main:main.c libmalloc_hook_library.so
	$(CC) $^ -o $@ -Wl,-rpath=./ -L./ -lmalloc_hook_library
	#$(CC) $^ -o $@ -Wl,-rpath=./ -Wl,libmalloc_hook_library.so

libmalloc_hook_library.so:malloc_hook_library.c
	$(CC) $^ -o $@ --shared -fPIC

clean:
	rm -rf main libmalloc_hook_library.so

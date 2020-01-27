all:do_execve show_info

do_execve:do_execve.o

show_info:show_info.o


test:all
	./do_execve	./show_info
	./do_execve ./show_info.sh
	./do_execve ./wrapper_args

clean:
	rm *.o do_execve show_info

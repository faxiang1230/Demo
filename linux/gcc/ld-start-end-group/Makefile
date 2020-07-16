all:liba.a libb.a test

liba.a:libtest1.o libtest2.o
	@ar -rs $@ $^

libb.a:libtest3.o libtest4.o
	@ar -rs $@ $^
test:main.o
	@gcc -o $@ $^ -Wl,--start-group liba.a libb.a -Wl,--end-group
	#gcc -o $@ $^ liba.a libb.a		//circul dependency
	sh -c ./$@

clean:
	@rm -rf *.a *.o test

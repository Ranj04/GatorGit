gatorgit: main.c gatorgit.c gatorgit.h util.h
	gcc -std=c99 main.c gatorgit.c -o gatorgit

clean:
	rm -rf gatorgit autotest test

test-gatorgit:
	sh init_test.sh

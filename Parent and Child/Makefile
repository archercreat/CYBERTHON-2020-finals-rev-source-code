
all:
	gcc -m32 -fno-pic -no-pie src/main.c src/func.s -o parent_and_child
	strip parent_and_child
	rm -rf bin/
	mkdir bin/
	mv parent_and_child bin/
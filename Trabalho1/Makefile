all:
	gcc -o programaTrab *.c

run:
	./programaTrab

clean:
	rm -f programaTrab

test: clean all run

zip:
	zip runcodes.zip Makefile *.c *.h
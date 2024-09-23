CFLAGS=-Wall -g

clean:
	rm -f ex1 ex3

clean_ex3:
	rm -f ex3

#all:
#	make ex1
all:ex1 ex3

ex1:
	cc -Wall -g ex1.c -o ex1

ex3:
	cc $(CFLAGS) ex3.c -o ex3


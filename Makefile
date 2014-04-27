compactor : compactor.o libcircle.o
	gcc -Wall -o compactor compactor.o libcircle.o -lm

compactor.o : compactor.c
	gcc -Wall -c -o compactor.o compactor.c

libcircle.o : libcircle.c libcircle.h
	gcc -Wall -c -o libcircle.o libcircle.c

clean:
	rm -f *.o

mrproper: clean
	rm compactor

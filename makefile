#makefile

build: clean testprog.c
	mpicc testprog.c -lm -o test

clean:
	rm -f *.o *~ core




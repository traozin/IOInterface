# makefile to create biggerProg

# link object files and system libraries to create executable
biggerProg: biggerProg.o sub1.o sub2.o
      gcc -o biggerProg biggerProg.o sub1.o sub2.o

# compile/assemble source files to create object files
biggerProg.o: biggerProg.c sub1.h sub2.h
      gcc -g -c biggerProg.c

sub1.o: sub1.c sub1.h
      gcc -g -c sub1.c

sub2.o: sub2.s
      as --gstabs sub2.s -o sub2.o

# remove object files and backup files
clean:
      rm -i *.o *~
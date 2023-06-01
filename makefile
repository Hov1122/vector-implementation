# the compiler: gcc for C program, define as g++ for C++
CC = g++
 
# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall
 
# The build target 
main: main.o
	$(CC) $(CFLAGS) -o main main.o

main.o: main.cpp vector.hpp vector_impl.hpp array.hpp array_impl.hpp
	$(CC) $(CFLAGS) -c main.cpp
 
clean:
	rm -f *.o

.PHONY: clean

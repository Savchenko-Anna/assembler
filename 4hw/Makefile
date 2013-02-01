#Makefile for task assembler 4

solver: main.o vector_operations.o Matrix.o 
	g++ -o solver main.o vector_operations.o Matrix.o 
main.o: main.cpp Matrix.h Linear_vector.h
	g++ -c -g main.cpp
Matrix.o: Matrix.cpp Matrix.h vector_operations.h
	g++ -c -g Matrix.cpp
vector_operations.o: vector_operations.h vector_operations.S
	gcc -c -g vector_operations.S -o vector_operations.o 
clean: 
	rm -f *.o solver

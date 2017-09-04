all: main.o copy_functions.o 
	g++ -o rapcp main.o copy_functions.o


main.o: main.cc 
	g++ -c main.cc


copy_functions.o: copy_functions.cc copy_functions.h verbose.h
	g++ -c copy_functions.cc



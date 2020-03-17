main: human.o student.o lab.o main.o
	g++ -Wall main.o human.o student.o lab.o -o main 

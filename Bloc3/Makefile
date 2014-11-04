#makefile 

all: main everything

main:
	g++ -c main.cpp model/model.cpp

everything:
	g++ -o bloc3 main.o model.o -lglut -lGL -lGLU

clean:
	rm *.o bloc3

default: all

all: client.o client_main.o
	gcc -Wall -o client client.c client_main.c

client.o:
	gcc -c client.c

client_main.o:
	gcc -c client_main.c

clean:
	rm -rf *~ client *.o

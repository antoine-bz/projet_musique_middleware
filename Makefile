
all: serveur client

serveur: libInet.a 
	gcc  main.c -DSERVEUR -o serveur -lInet -lProjet -L./
	

client: libInet.a libProjet.a
	gcc  main.c -DCLIENT -o client -lInet -lProjet -L./

libProjet.a: proto.o reqRep.o
	ar -crs libProjet.a proto.o reqRep.o
	rm -f *.o

libInet.a : session.o data.o
	ar -crs libInet.a session.o data.o
	rm -f *.o

session.o: session.c session.h
	gcc -c session.c

data.o: data.c data.h
	gcc -c data.c

clean:
	rm -f *.o
	rm -f serveur
	rm -f client
	rm -f libInet.a
	rm -f libProjet.a
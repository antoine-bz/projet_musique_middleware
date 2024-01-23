
all: serveur client
serveur: libInet.a
	gcc  serveur.c -o serveur -lInet -L./
	

client: libInet.a
	gcc  client.c -o client -lInet -L./
	
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

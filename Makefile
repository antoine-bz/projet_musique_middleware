serveur: libServer.a 
	gcc  main.c -DSERVEUR -o serveur.exe -lServer -L./
	rm -f *.a

client: libClient.a
	gcc main.c -I. -L. -L/usr/lib/x86_64-linux-gnu/ -o client.exe -DCLIENT -lClient -lmpg123 -lao -lm -L./
	rm -f *.a

libServer.a : session.o data.o reqRep.o protoServer.o 
	ar -crs libServer.a session.o data.o reqRep.o protoServer.o 
	rm -f *.o

libClient.a : session.o data.o reqRep.o protoClient.o
	ar -crs libClient.a session.o data.o reqRep.o protoClient.o
	rm -f *.o

reqRep.o: reqRep.c reqRep.h
	gcc -c reqRep.c

protoServer.o: protoServer.c protoServer.h
	gcc -c protoServer.c

protoClient.o: protoClient.c protoClient.h
	gcc -c protoClient.c

session.o: session.c session.h
	gcc -c session.c

data.o: data.c data.h
	gcc -c data.c

clean:
	rm -f *.o
	rm -f *.exe
	rm -f current_*.mp3
	rm -f *.a
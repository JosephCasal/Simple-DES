all: mycipher

mycipher: mycipher.c
	gcc mycipher.c -o mycipher

clean:
	rm -f mycipher *.o *~ core


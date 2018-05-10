# Makefile
CC = gcc
IFLAGS = -I.
LIBS = -lcurl
CFLAGS = -c -g -o
OBJ = json.o main.o baseBuild.o buildStreams.o buildTypes.o buildData.o  buildObjects.o \
   deviceSimulator.o sendToServerC.o testTxMessages.o getValuesAndStreams.o  \
   bufferData.o sendCert.o

%.o : %.c
	$(CC) $(CFLAGS) $@ $< $(IFLAGS)

omfDemo : $(OBJ)
	gcc -o $@ $^ $(LIBS)

clean :
	rm -f *.o *.exe omfDemo


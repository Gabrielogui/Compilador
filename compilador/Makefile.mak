CC     = gcc
RM     = rm -f
OBJS   = main.o \
         anasint.o \
         analex.o

LIBS   =
CFLAGS =

.PHONY: bin/Release/compilador.exe clean clean-after

all: bin/Release/compilador.exe

clean:
	$(RM) $(OBJS) bin/Release/compilador.exe

clean-after:
	$(RM) $(OBJS)

bin/Release/compilador.exe: $(OBJS)
	$(CC) -O2 -s -Wall -o $@ $(OBJS) $(LIBS)

main.o: main.c analex.h anasint.h
	$(CC) -O2 -s -Wall -c $< -o $@ $(CFLAGS)

anasint.o: anasint.c analex.h anasint.h
	$(CC) -O2 -s -Wall -c $< -o $@ $(CFLAGS)

analex.o: analex.c analex.h
	$(CC) -O2 -s -Wall -c $< -o $@ $(CFLAGS)


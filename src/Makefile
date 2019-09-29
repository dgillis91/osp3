IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=../obj

_DEPS = parse.h semutil.h pclock.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o parse.o semutil.o pclock.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

oss: $(OBJ)
	$(CC) -o ../bin/$@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

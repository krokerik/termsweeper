IDIR=headers
CC=gcc
CFLAGS=-I$(IDIR) -std=c99

SRCDIR=src

ODIR=obj
LDIR =../lib

LIBS=-lm -lncurses

_DEPS = main.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

termsweeper: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean readme

clean:
	rm -f $(ODIR)/*.o *~ core termsweeper
readme: termsweeper
	rm -f README.md
	echo \`\`\` > README.md
	./termsweeper --version >> README.md
	./termsweeper --help >> README.md
	echo \`\`\` >> README.md
	echo -n "README.md automatically generated on " >> README.md
	date >> README.md
	echo "last 5 commits:\n" >> README.md
	git log --pretty=format:"%ad - %an: %s  " -5 >> README.md
fresh: | clean termsweeper readme

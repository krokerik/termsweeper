CC=gcc

termsweep: src/main.c
	${CC} src/main.c

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

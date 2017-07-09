objects=a.o b.o c.o d.o e.o
all:$(objects)
		gcc -o all a.o b.o c.o \
			d.o e.o

a.o:a.c all.h
#	@echo compiling a.c 
	@gcc -c a.c -I ./ 

b.o:b.c all.h
	gcc -c b.c -I ./
	
c.o:c.c all.h
	gcc -c c.c -I ./
	
d.o:d.c all.h
	gcc -c d.c -I ./
	
e.o:e.c all.h
	gcc -c e.c -I ./

.PHONY clear:
	rm -rf *.o all 

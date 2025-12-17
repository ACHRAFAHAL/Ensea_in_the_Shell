CC	=	gcc

# wall = warning All , Wextra = Encore plus d'alertes )
CFLAGS	=	-Wall	-Wextra	-std=c99

# le nom du programme final
TARGET	=	enseash

#LE FICHIER CODE SOURCE 
SRC	=	q1_enseash.c

all:	$(SRC)
	$(CC)	$(CFLAGS)	-o	$(TARGET)	$(SRC)

clean:
	rm	-f	$(TARGET) 

.PHONY:	all	clean

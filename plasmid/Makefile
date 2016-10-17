TARGET = ../libplasmid.so
OBJS = plasmid.o signature.o
SRCS = $(OBJS:o=cpp)
INCS = -I./ -I /usr/java/jdk1.6.0_41/include  -I /usr/java/jdk1.6.0_41/include/linux
LIBS = -lpthread
CC = -c -fPIC -g

$(TARGET): $(OBJS)
	gcc  -shared -o $@ $^ $(LIBS)	

.c.o :	
	gcc $(CC) $< $(INCS)	

clean :	
	rm -f *.o	
	
dep :
	gccmakedep $(SRCS) $(INCS)	
# DO NOT DELETE

# Use the GNU C/C++ compiler:
CC = gcc
CPP = g++

#OBJECT FILES
OBJS = memman.o

#TARGET FILES
TARGET = memman

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

memman.o: memman.c 
	$(CC) -c memman.c

clean:
	$(RM) *.o *~ $(TARGET)

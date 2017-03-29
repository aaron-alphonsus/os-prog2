# INCOMPLETE

# Use the GNU C/C++ compiler:
CC = gcc
CPP = g++

#OBJECT FILES
OBJS = main.o student.o ta.o hangout.o help_student.o

#TARGET FILES
TARGET = bankers

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CC) -lpthread -o $(TARGET) $(OBJS)

bankers.o: main.c ta.h
    $(CC) -lpthread -c main.c

student.o: student.c ta.h
    $(CC) -lpthread -c student.c

ta.o: ta.c ta.h
    $(CC) -lpthread -c ta.c

hangout.o: hangout.c
    $(CC) -c hangout.c

help_student.o: help_student.c
    $(CC) -c help_student.c

clean:
    $(RM) *.o *~ $(TARGET)

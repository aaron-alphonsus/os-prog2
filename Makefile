# -w flag to hide warnings

# Use the GNU C/C++ compiler:
CC = gcc
CPP = g++

#OBJECT FILES
OBJS = bankers.o customer.o 

#TARGET FILES
TARGET = bankers

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -lpthread -pthread -o $(TARGET) $(OBJS)

bankers.o: bankers.c customer.h bank.h
	$(CC) -lpthread -pthread -c bankers.c

customer.o: customer.c customer.h
	$(CC) -lpthread -pthread -c customer.c

clean:
	$(RM) *.o *~ $(TARGET)

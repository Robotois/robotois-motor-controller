CC=g++

COMPILE_FLAGS = -g -std=c++11 -Wall -c
LINKER_FLAGS = -g
LIBS = -l bcm2835 -l rt

OBJECTS = \
	test.o \
	Motors.o \

#OBJECTS = $(subst .cpp,.o,$(SOURCES))

all: motors.o main.o
	$(CC) $(LINKER_FLAGS) -o runner $(OBJECTS) $(LIBS)

main.o: test.cpp
	$(CC) $(COMPILE_FLAGS) test.cpp
motors.o: Motors.cpp Motors.h
	$(CC) $(COMPILE_FLAGS) Motors.cpp

clean:
	$(RM) $(OBJECTS) runner


# This make only works on debian linux distribution. For mac and windows users, see ya.
CC = g++ -std=c++17
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LIBS = -lglut -lGLU -lGL -lGLEW

EXECUTABLE = mav
OBJS = main.o ObjParser.o

$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) -o $(EXECUTABLE)

main.o: main.cpp ObjParser.o
	$(CC) $(CFLAGS) $(LIBS) main.cpp

ObjParser.o: ObjParser.cpp ObjParser.h
	$(CC) $(CFLAGS) $(LIBS) ObjParser.cpp

clean:
	rm -f $(EXECUTABLE) $(OBJS)
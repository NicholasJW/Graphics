
# This make only works on debian linux distribution. For mac and windows users, see ya.
# To compile the program, just run make from terminal. To run the program, run make start, change the config file in CONF as you want.

CC = g++ -std=c++17
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
LIBS = 
# -lglut -lGLU -lGL -lGLEW -lfreeimage

EXECUTABLE = mav
OBJS = RayTracer.o SceneParser.o
CONF = Scene.config

$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) $(LIBS) -o $(EXECUTABLE)

RayTracer.o: RayTracer.cpp 
	$(CC) $(CFLAGS) $(LIBS) RayTracer.cpp

SceneParser.o: SceneParser.cpp
	$(CC) $(CFLAGS) $(LIBS) SceneParser.cpp

clean:
	rm -f $(EXECUTABLE) $(OBJS)

start:
	./mav $(CONF)

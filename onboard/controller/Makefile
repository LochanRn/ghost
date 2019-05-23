CC=sudo g++ -std=c++11
FLAGS=-lgps
MAIN=index.cpp
SOURCES=compass/compass.cpp udp/udp.cpp helper/helper.cpp autobot/autobotObstacle.cpp `pkg-config --cflags --libs opencv`
EXECUTABLE=index
RM=rm -rf

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(MAIN) $(SOURCES) $(FLAGS) -o $@

clean:
	$(RM) *.o $(EXECUTABLE)



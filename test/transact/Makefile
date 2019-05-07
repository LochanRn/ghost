CC=sudo g++ -std=c++11
MAIN=arm.cpp
SOURCES = udp/udp.cpp helper/helper.cpp motor/motor.cpp
EXECUTABLE=arm
RM=rm -rf

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(MAIN) $(SOURCES) $(FLAGS) -o $@

clean:
	$(RM) *.o $(EXECUTABLE)



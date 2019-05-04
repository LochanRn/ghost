CC = sudo g++ -std=c++11
MAIN = start.cpp
SOURCES = Fork/Fork.cpp udp/udp.cpp
EXECUTABLE = start
RM = rm -rf
$(EXECUTABLE): $(OBJECTS)
		$(CC) $(MAIN) $(SOURCES) -o $@

clean:
		$(RM) *.o $(EXECUTABLE)

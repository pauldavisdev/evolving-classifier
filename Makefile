CC = g++
CPPLAGS = -Wall 
DEPS = population_operations.h
OBJ = main.o population_operations.o

%.o: %.cpp $(DEPS)
	$(CC) $(CPPFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CPPFLAGS) -o $@ $^

clean:
	rm -f *.o main population_operations

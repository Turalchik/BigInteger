CC=g++
CFLAGS=-pedantic-errors -g
LDFLAGS=-lgtest -lgtest_main -lpthread
SOURCES=tests.cc biginteger.cpp
EXECUTABLE=biginttest

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE):
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
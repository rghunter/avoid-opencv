CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags`

OBJS =		SURF.o main.o 

CFLAGS  = -O3 -g -Wall `pkg-config --cflags opencv` -D LINUX -fPIC

LIBS =		`pkg-config opencv --libs`

TARGET = 	SURF_test

all:		SURF_test

SURF_test: $(OBJS)
	$(CXX) -o SURF_Test $(CXXFLAGS) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o  $@

clean:
	rm -f *.o $(TARGET)

CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags`

OBJS =		OpticalFlow.o

LIBS =		`pkg-config opencv --libs`

TARGET =	OpticalFlow

CXX = 		arm-angstrom-linux-gnueabi-g++

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

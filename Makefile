CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags`

OBJS =		OpticalFlow.o

LIBS =		`pkg-config opencv --libs`

TARGET =	OpticalFlow

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

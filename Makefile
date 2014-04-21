
TARGET =	COLA
OBJS =		main.o FeatureTracker.o Tau.o

LIBS =		`pkg-config opencv --libs`
CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags`
CFLAGS  = -O3 -g -Wall `pkg-config --cflags opencv` -D LINUX -fPIC

all	: $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)
	
%.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o  $@

clean:
	rm -f $(OBJS) $(TARGET)

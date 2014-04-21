
TARGET =	COLA
OBJS = main.o Tau.o FeatureTracker.o FlowField.o

LIBS =		`pkg-config opencv --libs`
CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags`
CFLAGS  = -O3 -g -Wall `pkg-config --cflags opencv` -D LINUX -fPIC

all	: $(TARGET)

$(TARGET) : $(OBJS)
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)
	
$(OBJS) : 
	$(CXX) -c $(CXXFLAGS) $< -o  $@.o

clean:
	rm -f $(OBJS) $(TARGET)

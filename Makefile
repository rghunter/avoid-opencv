CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags`

OBJS =		OpticalFlow.o ObstacleDetector.o Tau.o 

CFLAGS  = -O3 -g -Wall `pkg-config --cflags opencv` -D LINUX -fPIC

LIBS =		`pkg-config opencv --libs`

TARGET =	OpticalFlow

#$(TARGET):	$(OBJS)
#	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS) -pg
all :	OpticalFlow

OpticalFlow: $(OBJS)
	$(CXX) -o OpticalFlow $(CXXFLAGS) $(OBJS) $(LIBS)
	
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o  $@

clean:
	rm -f $(OBJS) $(TARGET)

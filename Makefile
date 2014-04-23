
#MAIN APPLICATION
TARGET =	COLA
DEPENDS = main FeatureTracker FlowField FrameDescriptor GlobalFlow

#Build Environment Variables
SRC_DIR = src/
BUILD_DIR = build/

OBJS = $(addprefix $(BUILD_DIR), $(addsuffix .o, $(DEPENDS)))
TEST_OBJS = $(addprefix $(BUILD_DIR), $(TESTS))

LIBS =		`pkg-config opencv --libs`
CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags`
CFLAGS  = -O3 -g -Wall `pkg-config --cflags opencv` -D LINUX -fPIC

all	: $(TARGET) 

$(TARGET) : $(OBJS)
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)
	
$(OBJS) : $(BUILD_DIR)%.o : $(SRC_DIR)%.cpp | builddir
	$(CXX) -c $(CXXFLAGS) $< -o  $@
	
builddir:
	mkdir -p build
	
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

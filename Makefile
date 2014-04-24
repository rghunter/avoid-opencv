
#MAIN APPLICATION
TARGET =	COLA_Test
EXECUTABLE_SRC = main.cpp
DEPENDS = FeatureTracker FlowField FrameDescriptor GlobalFlow Time DrawTools

#Build Environment Variables
SRC_DIR = src/
BUILD_DIR = build/

OBJS = $(addprefix $(BUILD_DIR), $(addsuffix .o, $(DEPENDS)))
TEST_OBJS = $(addprefix $(BUILD_DIR), $(TESTS))

LIBS =		`pkg-config opencv --libs`
CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags` -Iinclude/

all	: $(TARGET) 

$(TARGET) : $(EXECUTABLE_SRC) $(OBJS)
	$(CXX) $(CXXFLAGS) $(EXECUTABLE_SRC) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)
	
FeatureTracker : FlowField TimeDelay FrameDescriptor

GlobalFlow : FlowField
	
$(OBJS) : $(BUILD_DIR)%.o : $(SRC_DIR)%.cpp | builddir
	$(CXX) -c $(CXXFLAGS) $< -o  $@
	
builddir:
	mkdir -p build
	
.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

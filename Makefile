
#MAIN APPLICATION
TARGET =	COLA_Test
EXECUTABLE_SRC = main.cpp
DEPENDS = FeatureTracker FlowField FrameDescriptor GlobalFlow Time DrawTools Tau

INCLUDE_DIR = include

#UNIT TESTS
TESTS = FlowField_TEST
TEST_DIR = src/tests/

GTEST_LIBS=libgtest.la libgtest_main.la
GTEST_LIB_PATH=gtest_framework/gtest-1.7.0/lib

#Build Environment Variables
SRC_DIR = src

OBJS = $(addsuffix .o, $(DEPENDS))
TEST_OBJS = $(addprefix $(BUILD_DIR), $(TESTS))

LIBS =		`pkg-config opencv --libs`
CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags` -Iinclude/

all	: $(TARGET) 

$(TARGET) : $(EXECUTABLE_SRC) COLA.a
	$(CXX) $(CXXFLAGS) $(EXECUTABLE_SRC) -o $(TARGET) COLA.a $(LIBS)
	
COLA.a : $(INCLUDE_DIR)/COLA/COLA.h $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	
%.o : $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/COLA/%.h
	$(CXX) -c $(CXXFLAGS) $< -o  $@
	
builddir:
	mkdir -p build
	
.PHONY: clean
clean:
	rm -rf *.o *.a $(TARGET)
	
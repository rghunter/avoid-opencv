
#MAIN APPLICATION
TARGET =	COLA_Test
EXECUTABLE_SRC = main.cpp
DEPENDS = FeatureTracker FlowField FrameDescriptor GlobalFlow Time DrawTools Tau

INCLUDE_DIR = include

BUILD_DIR=build

#UNIT TESTS
TESTS = FlowField_TEST
TEST_DIR = tests

GTEST_LIB_PATH=gtest_framework/gtest-1.7.0/lib
GTEST_LIBS=$(addprefix $(GTEST_LIB_PATH)/, ( libgtest.la libgtest_main.la))

GTEST_DIR=gtest_framework/gtest-1.7.0

#Build Environment Variables
SRC_DIR = src

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(DEPENDS)))
TEST_OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(TESTS)))

LIBS =		`pkg-config opencv --libs`
CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags` -I$(INCLUDE_DIR) 

all	: $(TARGET) 

tests : $(GTEST_LIBS) $(TEST_OBJS) $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o COLA_Test


$(TARGET) : $(EXECUTABLE_SRC) $(OBJS)
	$(CXX) $(CXXFLAGS) $(EXECUTABLE_SRC) -o $(TARGET) COLA.a $(LIBS)
	
build/%.o : $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/COLA/%.h
	$(CXX) -c $(CXXFLAGS) $< -o  $@
	
build/%.o : $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include -c $< -o  $@

$(GTEST_LIBS) : 
	$(MAKE) -C gtest_framework/

.PHONY: clean
clean:
	rm -rf build/*.o build/*.a $(TARGET)
	
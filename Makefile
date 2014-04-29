
#MAIN APPLICATION
TARGET =	COLA
EXECUTABLE_SRC = main.cpp
DEPENDS = FeatureTracker FlowField FrameDescriptor GlobalFlow Time DrawTools Tau

INCLUDE_DIR = include

BUILD_DIR=build

#UNIT TESTS
TEST_TARGET= COLA_Test
TESTS = FlowField_TEST Tau_TEST
TEST_DIR = tests

GTEST_LIB_PATH=gtest_framework
GTEST_LIBS=$(addprefix $(GTEST_LIB_PATH)/, ( gtest_main.a gtest.a))

GTEST_DIR=gtest_framework/gtest-1.7.0

#Build Environment Variables
SRC_DIR = src

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(DEPENDS)))
TEST_OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(TESTS)))

LIBS =		`pkg-config opencv --libs`
CXXFLAGS =	-g -O0 -fno-inline -Wall `pkg-config opencv --cflags` -I$(INCLUDE_DIR) 

all	: $(TEST_TARGET) $(TARGET)

run : $(TARGET)

manual : include/COLA/*.h #We should update the manual if we update headers
	doxygen docs/Doxyfile; \
	cd docs/latex; \
	$(MAKE); \
	cp refman.pdf ../../COLA_Manual.pdf

tests : $(TEST_TARGET)

$(TEST_TARGET) : $(GTEST_LIBS) $(TEST_OBJS) $(OBJS)
	$(CXX) $(CXXFLAGS) -lpthread $^ -o $(TEST_TARGET) $(LIBS)

$(TARGET) : $(EXECUTABLE_SRC) $(OBJS)
	$(CXX) $(CXXFLAGS) $(EXECUTABLE_SRC) -o $(TARGET) $(OBJS) $(LIBS)
	
build/%.o : $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/COLA/%.h
	$(CXX) -c $(CXXFLAGS) $< -o  $@
	
build/%.o : $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(GTEST_DIR)/include -c $< -o  $@

$(GTEST_LIBS) : 
	$(MAKE) -C gtest_framework/

.PHONY: clean
clean:
	rm -rf build/*.o build/*.a $(TARGET) $(TEST_TARGET); \
	$(MAKE) -C gtest_framework/ clean
	

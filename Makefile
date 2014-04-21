
#MAIN APPLICATION
TARGET =	COLA
DEPENDS = main Tau FeatureTracker FlowField

#UNIT TESTS
TESTS = FlowField_TEST
TEST_DIR = src/tests/

#Build Environment Variables
SRC_DIR = src/
BUILD_DIR = build/

GTEST_DIR = gtest_framework

CPPFLAGS += -isystem $(GTEST_DIR)/include

OBJS = $(addprefix $(BUILD_DIR), $(addsuffix .o, $(DEPENDS)))
TEST_OBJS = $(addprefix $(BUILD_DIR), $(TESTS))

LIBS =		`pkg-config opencv --libs`
CXXFLAGS =	-g -O0 -Wall `pkg-config opencv --cflags`
G_CXXFLAGS += -g -Wall -Wextra -pthread

all	: $(TARGET) 

$(TARGET) : $(OBJS)
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS) $(LIBS)
	
$(OBJS) : $(BUILD_DIR)%.o : $(SRC_DIR)%.cpp | builddir
	$(CXX) -c $(CXXFLAGS) $< -o  $@
	
builddir:
	mkdir -p build
	
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
	
# Builds gtest.a and gtest_main.a.

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
	
# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(BUILD_DIR)gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(G_CXXFLAGS) -c $(GTEST_DIR)/src/gtest-all.cc -o $@

$(BUILD_DIR)gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(G_CXXFLAGS) -c $(GTEST_DIR)/src/gtest_main.cc -o $@

$(BUILD_DIR)gtest.a : $(BUILD_DIR)gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : $(BUILD_DIR)gtest-all.o $(BUILD_DIR)gtest_main.o
	$(AR) $(ARFLAGS) $(BUILD_DIR)$@ $^

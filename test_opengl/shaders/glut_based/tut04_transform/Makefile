#==================
# compile flags
#==================

PARENT = ..
LIB_SRC_PATH = .
INCLUDE_PATH = .
LIB_PATH = $(EXTERN_LIB_PATH)
SRC_PATH = .
BUILD_PATH = build
BIN_PATH = bin
BIN_STEMS = triangle
BINARIES = $(patsubst %, $(BIN_PATH)/%, $(BIN_STEMS))

COMMON = $(PARENT)/common
INCLUDE_PATH_COMMON = $(COMMON)/include
SRC_PATH_COMMON = $(COMMON)/src
BUILD_PATH_COMMON = $(BUILD_PATH)/common

INCLUDE_PATHS = $(INCLUDE_PATH) $(COMMON_INCLUDE_PATH) $(EXTERN_INCLUDE_PATH)
INCLUDE_PATH_FLAGS = $(patsubst %, -I%, $(INCLUDE_PATHS))

LIB_PATHS = $(LIB_PATH)
LIB_PATH_FLAGS = $(patsubst %, -L%, $(LIB_PATHS))

LIB_STEMS = glut GLEW GL
LIBS = $(patsubst %, $(LIB_PATH)/lib%.a, $(LIB_STEMS))
LIB_FLAGS = $(patsubst %, -l%, $(LIB_STEMS))

CXX = g++
DEBUG = -g
CXXFLAGS = -Wall $(DEBUG) $(INCLUDE_PATH_FLAGS)
LDFLAGS = -Wall $(DEBUG) $(LIB_PATH_FLAGS) $(LIB_FLAGS)

SCRIPT_PATH = test
TEST_PATH = test

#==================
# all
#==================

.DEFAULT_GOAL : all
all : $(BINARIES)

#==================
# objects (common)
#==================

CPP_STEMS_COMMON = \
		shader_utils
OBJECTS_COMMON = $(patsubst %, $(BUILD_PATH_COMMON)/%.o, $(CPP_STEMS_COMMON))
LINT_FILES_COMMON = $(patsubst %, $(BUILD_PATH_COMMON)/%.lint, $(CPP_STEMS_COMMON))

$(BUILD_PATH_COMMON)/%.o : $(SRC_PATH_COMMON)/%.cpp
	mkdir -p $(BUILD_PATH_COMMON)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY : clean_objects_common
clean_objects_common :
	-rm $(OBJECTS_COMMON)
	-rmdir $(BUILD_PATH_COMMON)

#==================
# objects
#==================

$(BUILD_PATH)/%.o : $(SRC_PATH)/%.cpp
	mkdir -p $(BUILD_PATH)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY : clean_objects
clean_objects : clean_objects_common
	-rm $(OBJECTS)

#==================
# binaries
#==================

CPP_STEMS = triangle
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(CPP_STEMS))

$(BIN_PATH)/triangle : $(BUILD_PATH)/triangle.o $(OBJECTS_COMMON)
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY : clean_binaries
clean_binaries :
	-rm $(BINARIES)

#==================
# test
#==================

TEST_PATH = test
TEST_SH = $(SCRIPT_PATH)/test.sh
TEST_FILE_STEMS = triangle
TEST_FILES = $(patsubst %, $(BUILD_PATH)/%.test, $(TEST_FILE_STEMS))
TEST_PASS_FILES = $(patsubst %, %.pass, $(TEST_FILES))
TEST_FAIL_FILES = $(patsubst %, %.fail, $(TEST_FILES))

$(BUILD_PATH)/triangle.test.pass : $(BIN_PATH)/triangle $(TEST_PATH)/triangle.test
	-$(TEST_SH) $(BIN_PATH)/triangle arg $(TEST_PATH)/triangle.test $(TEST_PATH)/triangle.gold \
			$(BUILD_PATH)/triangle.test

.PHONY : test
test : $(TEST_PASS_FILES)

.PHONY : clean_tests
clean_tests :
	-rm $(TEST_PASS_FILES) $(TEST_FAIL_FILES)

#==================
# clean
#==================

.PHONY : clean
clean : clean_binaries clean_objects clean_tests
	-rmdir $(BUILD_PATH) $(BIN_PATH)

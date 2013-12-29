#==================
# compile flags
#==================

LIB_SRC_PATH = .
INCLUDE_PATH = .
LIB_PATH = $(EXTERN_LIB_PATH)
SRC_PATH = .
BUILD_PATH = build
BIN_PATH = bin
BIN_STEMS = basic matrix_transform triangle_normal vector_types
BINARIES = $(patsubst %, $(BIN_PATH)/%, $(BIN_STEMS))

INCLUDE_PATHS = $(INCLUDE_PATH) $(COMMON_INCLUDE_PATH) $(EXTERN_INCLUDE_PATH)
INCLUDE_PATH_FLAGS = $(patsubst %, -I%, $(INCLUDE_PATHS))

LIB_PATHS = $(LIB_PATH)
LIB_PATH_FLAGS = $(patsubst %, -L%, $(LIB_PATHS))

LIB_STEMS = glut
LIBS = $(patsubst %, $(LIB_PATH)/lib%.a, $(LIB_STEMS))
LIB_FLAGS = $(patsubst %, -l%, $(LIB_STEMS))

CXX = g++
DEBUG = -g
CXXFLAGS = -Wall $(DEBUG) $(INCLUDE_PATH_FLAGS)
LDFLAGS = -Wall $(DEBUG) $(LIB_PATH_FLAGS) $(LIB_FLAGS)

SCRIPT_PATH = test
TEST_PATH = test

ASM_SUFFIX = asm

#==================
# all
#==================

.DEFAULT_GOAL : all
all : $(BINARIES)

#==================
# objects
#==================

$(BUILD_PATH)/%.o : $(SRC_PATH)/%.cpp
	mkdir -p $(BUILD_PATH)
	$(CXX) -c -o $@ $< $(CXXFLAGS)
	$(CXX) -S -o $(BUILD_PATH)/$<.$(ASM_SUFFIX) $< $(CXXFLAGS)

.PHONY : clean_objects
clean_objects :
	-rm $(OBJECTS) $(ASM_OBJECTS)

#==================
# binaries
#==================

CPP_STEMS = basic matrix_transform triangle_normal vector_types
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(CPP_STEMS))
ASM_OBJECTS = $(patsubst %, $(BUILD_PATH)/%.cpp.$(ASM_SUFFIX), $(CPP_STEMS))

$(BIN_PATH)/basic : $(BUILD_PATH)/basic.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/matrix_transform : $(BUILD_PATH)/matrix_transform.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/triangle_normal : $(BUILD_PATH)/triangle_normal.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/vector_types : $(BUILD_PATH)/vector_types.o
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
TEST_FILE_STEMS = basic matrix_transform triangle_normal vector_types
TEST_FILES = $(patsubst %, $(BUILD_PATH)/%.test, $(TEST_FILE_STEMS))
TEST_PASS_FILES = $(patsubst %, %.pass, $(TEST_FILES))
TEST_FAIL_FILES = $(patsubst %, %.fail, $(TEST_FILES))

$(BUILD_PATH)/basic.test.pass : $(BIN_PATH)/basic $(TEST_PATH)/basic.test
	-$(TEST_SH) $(BIN_PATH)/basic arg $(TEST_PATH)/basic.test $(TEST_PATH)/basic.gold \
			$(BUILD_PATH)/basic.test

$(BUILD_PATH)/matrix_transform.test.pass : $(BIN_PATH)/matrix_transform $(TEST_PATH)/matrix_transform.test
	-$(TEST_SH) $(BIN_PATH)/matrix_transform arg $(TEST_PATH)/matrix_transform.test $(TEST_PATH)/matrix_transform.gold \
			$(BUILD_PATH)/matrix_transform.test

$(BUILD_PATH)/triangle_normal.test.pass : $(BIN_PATH)/triangle_normal $(TEST_PATH)/triangle_normal.test
	-$(TEST_SH) $(BIN_PATH)/triangle_normal arg $(TEST_PATH)/triangle_normal.test $(TEST_PATH)/triangle_normal.gold \
			$(BUILD_PATH)/triangle_normal.test

$(BUILD_PATH)/vector_types.test.pass : $(BIN_PATH)/vector_types $(TEST_PATH)/vector_types.test
	-$(TEST_SH) $(BIN_PATH)/vector_types arg $(TEST_PATH)/vector_types.test $(TEST_PATH)/vector_types.gold \
			$(BUILD_PATH)/vector_types.test

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

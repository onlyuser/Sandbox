#==================
# compile flags
#==================

PARENT = ..
LIB_SRC_PATH = .
INCLUDE_PATH = include
LIB_PATH = $(EXTERN_LIB_PATH)
SRC_PATH = src
BUILD_PATH = build
BIN_PATH = bin
BIN_STEMS = main
BINARIES = $(patsubst %, $(BIN_PATH)/%, $(BIN_STEMS))

COMMON = $(PARENT)/common
INCLUDE_PATH_COMMON = $(COMMON)/include
SRC_PATH_COMMON = $(COMMON)/src
BUILD_PATH_COMMON = $(BUILD_PATH)/common

INCLUDE_PATHS = $(INCLUDE_PATH) $(COMMON_INCLUDE_PATH) $(EXTERN_INCLUDE_PATH)
INCLUDE_PATH_FLAGS = $(patsubst %, -I%, $(INCLUDE_PATHS))

LIB_PATHS = $(LIB_PATH)
LIB_PATH_FLAGS = $(patsubst %, -L%, $(LIB_PATHS))

LIB_STEMS = glut GLEW GL png
LIBS = $(patsubst %, $(LIB_PATH)/lib%.a, $(LIB_STEMS))
LIB_FLAGS = $(patsubst %, -l%, $(LIB_STEMS))

CXX = g++
DEBUG = -g
CXXFLAGS = -Wall $(DEBUG) $(INCLUDE_PATH_FLAGS) -std=c++0x
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

$(BUILD_PATH)/%.o : $(SRC_PATH)/%.c
	mkdir -p $(BUILD_PATH)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY : clean_objects
clean_objects : clean_objects_common
	-rm $(OBJECTS)

#==================
# binaries
#==================

CPP_STEMS = ShaderContext Buffer Camera IdentObject Light main Material Mesh PrimitiveFactory Program res_texture res_texture2 Scene Shader Texture Util VarAttribute VarUniform XformObject
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(CPP_STEMS))

$(BIN_PATH)/main : $(OBJECTS) $(OBJECTS_COMMON)
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
TEST_FILE_STEMS = main
TEST_FILES = $(patsubst %, $(BUILD_PATH)/%.test, $(TEST_FILE_STEMS))
TEST_PASS_FILES = $(patsubst %, %.pass, $(TEST_FILES))
TEST_FAIL_FILES = $(patsubst %, %.fail, $(TEST_FILES))

$(BUILD_PATH)/main.test.pass : $(BIN_PATH)/main $(TEST_PATH)/main.test
	-$(TEST_SH) $(BIN_PATH)/main arg $(TEST_PATH)/main.test $(TEST_PATH)/main.gold \
			$(BUILD_PATH)/main.test

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

#==================
# compile flags
#==================

LIB_SRC_PATH = .
INCLUDE_PATH = .
LIB_PATH = $(EXTERN_LIB_PATH)
SRC_PATH = .
BUILD_PATH = build
BIN_PATH = bin
BIN_STEMS = main main2 main3 main4
BINARIES = $(patsubst %, $(BIN_PATH)/%, $(BIN_STEMS))

INCLUDE_PATHS = $(INCLUDE_PATH) $(COMMON_INCLUDE_PATH) $(EXTERN_INCLUDE_PATH)
INCLUDE_PATH_FLAGS = $(patsubst %, -I%, $(INCLUDE_PATHS))

LIB_PATHS = $(LIB_PATH)
LIB_PATH_FLAGS = $(patsubst %, -L%, $(LIB_PATHS))

LIB_STEMS =
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
# objects
#==================

$(BUILD_PATH)/%.o : $(SRC_PATH)/%.cpp
	mkdir -p $(BUILD_PATH)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY : clean_objects
clean_objects :
	-rm $(OBJECTS)

#==================
# binaries
#==================

CPP_STEMS = main main2 main3 main4
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(CPP_STEMS))

$(BIN_PATH)/main : $(BUILD_PATH)/main.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/main2 : $(BUILD_PATH)/main2.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/main3 : $(BUILD_PATH)/main3.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/main4 : $(BUILD_PATH)/main4.o
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
TEST_FILE_STEMS = main main2 main3 main4
TEST_FILES = $(patsubst %, $(BUILD_PATH)/%.test, $(TEST_FILE_STEMS))
TEST_PASS_FILES = $(patsubst %, %.pass, $(TEST_FILES))
TEST_FAIL_FILES = $(patsubst %, %.fail, $(TEST_FILES))

$(BUILD_PATH)/main.test.pass : $(BIN_PATH)/main $(TEST_PATH)/main.test
	-$(TEST_SH) $(BIN_PATH)/main arg $(TEST_PATH)/main.test $(TEST_PATH)/main.gold \
			$(BUILD_PATH)/main.test

$(BUILD_PATH)/main2.test.pass : $(BIN_PATH)/main2 $(TEST_PATH)/main2.test
	-$(TEST_SH) $(BIN_PATH)/main2 stdin $(TEST_PATH)/main2.test $(TEST_PATH)/main2.gold \
			$(BUILD_PATH)/main2.test

$(BUILD_PATH)/main3.test.pass : $(BIN_PATH)/main3 $(TEST_PATH)/main3.test
	-$(TEST_SH) $(BIN_PATH)/main3 arg $(TEST_PATH)/main3.test $(TEST_PATH)/main3.gold \
			$(BUILD_PATH)/main3.test

$(BUILD_PATH)/main4.test.pass : $(BIN_PATH)/main4 $(TEST_PATH)/main4.test
	-$(TEST_SH) $(BIN_PATH)/main4 arg $(TEST_PATH)/main4.test $(TEST_PATH)/main4.gold \
			$(BUILD_PATH)/main4.test

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

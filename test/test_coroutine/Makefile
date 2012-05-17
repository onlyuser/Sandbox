CXX = g++

#==================
# compile flags
#==================

LIB_SRC_PATH = .
INCLUDE_PATH = $(EXTERN_INCLUDE_PATH)
LIB_PATH = $(EXTERN_LIB_PATH)
SRC_PATH = .
BUILD_PATH = build
BIN_PATH = bin
BINARY = $(BIN_PATH)/test_coroutine.exe

INCLUDE_PATHS = $(INCLUDE_PATH) $(COMMON_INCLUDE_PATH)
INCLUDE_PATH_FLAGS = $(patsubst %, -I%, $(INCLUDE_PATHS))

LIB_PATHS = $(LIB_PATH)
LIB_PATH_FLAGS = $(patsubst %, -L%, $(LIB_PATHS))

LIB_STEMS =
LIBS = $(patsubst %, $(LIB_PATH)/lib%.a, $(LIB_STEMS))
LIB_FLAGS = $(patsubst %, -l%, $(LIB_STEMS))

#DEBUG = -g
CXXFLAGS = -Wall $(DEBUG) $(INCLUDE_PATH_FLAGS)
LDFLAGS = -Wall $(DEBUG) $(LIB_PATH_FLAGS) $(LIB_FLAGS)

SCRIPT_PATH = test
TEST_PATH = test

#==================
# all
#==================

.DEFAULT_GOAL : all
all : $(BINARY)

#==================
# lib
#==================

$(LIBS) :
	cd $(LIB_SRC_PATH); $(MAKE) -f Makefile

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
# binary
#==================

CPP_STEMS = main
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(CPP_STEMS))

$(BINARY) : $(OBJECTS) $(LIBS)
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY : clean_binary
clean_binary :
	-rm $(BINARY)

#==================
# test
#==================

TEST_PATH = test
TEST_SH = $(SCRIPT_PATH)/test.sh
TEST_FILE_STEMS = coroutine_input
TEST_FILES = $(patsubst %, $(BUILD_PATH)/%.test, $(TEST_FILE_STEMS))
TEST_PASS_FILES = $(patsubst %, %.pass, $(TEST_FILES))
TEST_FAIL_FILES = $(patsubst %, %.fail, $(TEST_FILES))

$(BUILD_PATH)/%.test.pass : $(BINARY) $(TEST_PATH)/%.test
	-$(TEST_SH) $(BINARY) $(TEST_PATH)/$*.test $(TEST_PATH)/$*.gold \
			$(BUILD_PATH)/$*.test

.PHONY : test
test : $(TEST_PASS_FILES)

.PHONY : clean_tests
clean_tests :
	-rm $(TEST_PASS_FILES) $(TEST_FAIL_FILES)

#==================
# clean
#==================

.PHONY : clean
clean : clean_binary clean_objects clean_tests
	-rmdir $(BUILD_PATH) $(BIN_PATH)

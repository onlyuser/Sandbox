CXX = g++
LLVM_CONFIG = /usr/lib/llvm-2.8/bin/llvm-config

BIN_PATH = bin
BUILD_PATH = build
BINARY = $(BIN_PATH)/toy

SCRIPT_PATH = test
TEST_PATH = test

#==================
# all
#==================

.DEFAULT_GOAL : all
all : $(BINARY)

#==================
# binary
#==================

$(BINARY) : toy.cpp
	mkdir -p $(BIN_PATH)
	$(CXX) -g toy.cpp `$(LLVM_CONFIG) --cppflags --ldflags --libs core` -O3 -o $(BINARY)

.PHONY : clean_binary
clean_binary :
	-rm $(BINARY)

#==================
# test
#==================

TEST_PATH = test
TEST_SH = $(SCRIPT_PATH)/test.sh
TEST_FILE_STEMS = toy1 toy2 toy3 toy4
TEST_FILES = $(patsubst %, $(BUILD_PATH)/%.test, $(TEST_FILE_STEMS))
TEST_PASS_FILES = $(patsubst %, %.pass, $(TEST_FILES))
TEST_FAIL_FILES = $(patsubst %, %.fail, $(TEST_FILES))

$(BUILD_PATH)/%.test.pass : $(BINARY) $(TEST_PATH)/%.test
	mkdir -p $(BUILD_PATH)
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
clean : clean_binary clean_tests
	-rmdir $(BUILD_PATH) $(BIN_PATH)

#==================
# compile flags
#==================

LIB_SRC_PATH = .
INCLUDE_PATH = .
LIB_PATH = $(EXTERN_LIB_PATH)
SRC_PATH = .
BUILD_PATH = build
BIN_PATH = bin
BIN_STEMS = calc1 calc2 calc3a calc3b calc3g
BINARIES = $(patsubst %, $(BIN_PATH)/%, $(BIN_STEMS))

INCLUDE_PATHS = $(INCLUDE_PATH) $(COMMON_INCLUDE_PATH) $(EXTERN_INCLUDE_PATH)
INCLUDE_PATH_FLAGS = $(patsubst %, -I%, $(INCLUDE_PATHS))

LIB_PATHS = $(LIB_PATH)
LIB_PATH_FLAGS = $(patsubst %, -L%, $(LIB_PATHS))

LIB_STEMS =
LIBS = $(patsubst %, $(LIB_PATH)/lib%.a, $(LIB_STEMS))
LIB_FLAGS = $(patsubst %, -l%, $(LIB_STEMS))

CXX = gcc
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
# yacc-gen
#==================

YACC_STEMS = calc1.tab calc2.tab calc3.tab
YACC_GEN = \
		$(patsubst %, $(SRC_PATH)/%.c, $(YACC_STEMS)) \
		$(patsubst %, $(INCLUDE_PATH)/%.h, $(YACC_STEMS))
.SECONDARY : $(YACC_GEN)

$(SRC_PATH)/%.tab.c $(INCLUDE_PATH)/%.tab.h : $(SRC_PATH)/%.y
	bison -d $<

.PHONY : clean_yacc
clean_yacc :
	-rm $(YACC_GEN)

#==================
# lex-gen
#==================

LEX_STEMS = lex.calc1 lex.calc2 lex.calc3
LEX_GEN = $(patsubst %, $(SRC_PATH)/%.c, $(LEX_STEMS))
.SECONDARY : $(LEX_GEN)

$(SRC_PATH)/lex.%.c : $(SRC_PATH)/%.l $(YACC_GEN)
	flex $<

.PHONY : clean_lex
clean_lex :
	-rm $(LEX_GEN)

#==================
# objects
#==================

$(BUILD_PATH)/%.o : $(SRC_PATH)/%.c
	mkdir -p $(BUILD_PATH)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY : clean_objects
clean_objects : clean_yacc clean_lex
	-rm $(OBJECTS)

#==================
# binaries
#==================

CPP_STEMS = $(YACC_STEMS) $(LEX_STEMS) calc3a calc3b calc3g
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(CPP_STEMS))

$(BIN_PATH)/calc1 : $(BUILD_PATH)/calc1.tab.o $(BUILD_PATH)/lex.calc1.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/calc2 : $(BUILD_PATH)/calc2.tab.o $(BUILD_PATH)/lex.calc2.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/calc3a : $(BUILD_PATH)/calc3.tab.o $(BUILD_PATH)/lex.calc3.o $(BUILD_PATH)/calc3a.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/calc3b : $(BUILD_PATH)/calc3.tab.o $(BUILD_PATH)/lex.calc3.o $(BUILD_PATH)/calc3b.o
	mkdir -p $(BIN_PATH)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_PATH)/calc3g : $(BUILD_PATH)/calc3.tab.o $(BUILD_PATH)/lex.calc3.o $(BUILD_PATH)/calc3g.o
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
TEST_FILE_STEMS = calc1 calc2 calc3a calc3b #calc3g
TEST_FILES = $(patsubst %, $(BUILD_PATH)/%.test, $(TEST_FILE_STEMS))
TEST_PASS_FILES = $(patsubst %, %.pass, $(TEST_FILES))
TEST_FAIL_FILES = $(patsubst %, %.fail, $(TEST_FILES))

$(BUILD_PATH)/%.test.pass : $(BIN_PATH)/% $(TEST_PATH)/%.test
	-$(TEST_SH) $(BIN_PATH)/$* $(TEST_PATH)/$*.test $(TEST_PATH)/$*.gold \
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
clean : clean_binaries clean_objects clean_tests
	-rmdir $(BUILD_PATH) $(BIN_PATH)

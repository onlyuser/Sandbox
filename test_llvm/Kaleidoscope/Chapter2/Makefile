CXX = g++

BIN_PATH = bin
BINARY = $(BIN_PATH)/toy

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
	$(CXX) -g toy.cpp -O3 -o $(BINARY)

.PHONY : clean_binary
clean_binary :
	-rm $(BINARY)

#==================
# test
#==================

.PHONY : test
test : $(BINARY)

#==================
# clean
#==================

.PHONY : clean
clean : clean_binary
	-rmdir $(BIN_PATH)

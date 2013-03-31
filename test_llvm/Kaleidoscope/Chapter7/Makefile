CXX = g++
LLVM_CONFIG = /usr/lib/llvm-2.8/bin/llvm-config

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
	$(CXX) -g toy.cpp `$(LLVM_CONFIG) --cppflags --ldflags --libs core jit native` -O3 -o $(BINARY)

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

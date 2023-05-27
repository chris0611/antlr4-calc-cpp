CXX := g++

ANTLR4_DIR := /usr/local
INC_DIRS := -I$(ANTLR4_DIR)/include/antlr4-runtime -Igenerated
CXXFLAGS := -std=c++17 -O2 -march=native $(INC_DIRS) -Wall -Wpedantic -Wno-attributes

TARGET_EXEC := main	# name of final executable

B_DIR := build
GEN_DIR := generated

GEN_OBJS := $(B_DIR)/calcBaseListener.o $(B_DIR)/calcLexer.o $(B_DIR)/calcParser.o $(B_DIR)/calcListener.o
GEN_HDRS := $(GEN_DIR)/calcBaseListener.h $(GEN_DIR)/calcLexer.h $(GEN_DIR)/calcParser.h $(GEN_DIR)/calcListener.h

all: $(B_DIR)/$(TARGET_EXEC)

$(B_DIR)/$(TARGET_EXEC): $(B_DIR)/main.o $(GEN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L$(ANTLR4_DIR)/lib -lantlr4-runtime -Wl,-rpath=$(ANTLR4_DIR)/lib

$(B_DIR)/main.o: $(GEN_HDRS)

# only main.cpp
$(B_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(B_DIR)/%.o: $(GEN_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(GEN_HDRS): calc.g4
	antlr4 -Dlanguage=Cpp $< -o $(GEN_DIR)/

clean:
	rm -f $(B_DIR)/*.o $(B_DIR)/main

realclean: clean
	rm -f $(GEN_DIR)/*

.PHONY: all clean realclean

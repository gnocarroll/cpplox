TARGET  := cpplox

SRC     := src
INCLUDE := include
OBJ     := obj
BIN     := bin

CXX := clang++
CC  := clang

CXXFLAGS := -std=c++17 -Wall -I$(INCLUDE)

CPP_SRC := $(wildcard $(SRC)/*.cpp)
CPP_OBJ := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(CPP_SRC))

all: $(TARGET)

$(OBJ)/%.o : $(SRC)/%.cpp
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET): $(CPP_OBJ)
	@mkdir -p $(BIN)
	$(CXX) -o $(BIN)/$@ $(CPP_OBJ)

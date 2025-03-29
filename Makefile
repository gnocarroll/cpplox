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

TARGET_PATH := $(BIN)/$(TARGET)

all: $(TARGET_PATH)

$(OBJ)/%.o : $(SRC)/%.cpp
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(TARGET_PATH): $(CPP_OBJ)
	@mkdir -p $(BIN)
	$(CXX) -o $@ $(CPP_OBJ)

.PHONY: clean
clean:
	-rm $(OBJ)/*.o $(TARGET_PATH)

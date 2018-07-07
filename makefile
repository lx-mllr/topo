LOCAL_DIR = C:\Users\lx-pc\Documents\C++\topo

SRC_DIR = $(LOCAL_DIR)\src
OBJ_DIR = $(LOCAL_DIR)\obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

OBJECTS = glTest.o
LFLAGS = -LC:/Users/lx-pc/Documents/C++/topo/lib/ -lglfw3 -lopengl32 -lgdi32 -lglew32
CFLAGS = -IC:/Users/lx-pc/Documents/C++/topo/include
EXEC = a.exe
FLAGS = -Wall
CC = g++

all: build

build: $(OBJ_FILES) 
	$(CC) $(FLAGS) -o $(EXEC) $^ $(LFLAGS)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp  | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJ_DIR):
	mkdir $@
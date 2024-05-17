# CC:=arm-linux-gnueabihf-g++
CC:=g++
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin
TAR=$(BIN_DIR)/server
LDFLAGS := $(shell pkg-config --libs opencv) -lpthread

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(TAR)

# 生成目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

# 链接目标文件生成可执行文件
$(TAR):$(OBJ_FILES)
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR)/*.o $(TAR)




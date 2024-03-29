CC:=gcc
SRC_DIR=./src
OBJ_DIR=./obj
BIN_DIR=./bin
TAR=$(BIN_DIR)/server

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(TAR)

# 生成目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

# 链接目标文件生成可执行文件
$(TAR):$(OBJ_FILES)
	$(CC) $^ -o $@

# $(TAR_SERVER):$(OBJ_FILES)
# 	$(CC) $^ -o $@

# clean:
#     rm -rf $(OBJ_DIR)
# all:client,server

# .PHONY:all
# client:client.o
# 	gcc -o client client.o
# client.o:./src/client.c
# 	gcc -c ./src/client.c -o client.o 

# server:server.o
# 	gcc -o server server.o
# server.o:./src/server.c
# 	gcc -c ./src/server.c -o server.o

# clean:
# 	rm -f *.o


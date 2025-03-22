CC = gcc
CFLAGS = -Wall
LDFLAGS = -lssl -lcrypto
CSTD = -std=c11
TARGET = imap
SRC_DIR = src
OBJ_DIR = objs

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(CSTD) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CSTD) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

clean_objs:
	rm -f $(OBJS)

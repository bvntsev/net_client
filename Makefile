# === config ===
include config.mk

# get the files
SRCS := $(wildcard $(SRC_DIR)/*.c) 
# replace the .c names to .o
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRCS))

all: $(BIN)

$(BIN): make_dir $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir p $@

clean:
	rm -rf $(BIN_DIR) $(BIN)

run: 
	./$(BIN)

run_debug:
	gdb $(BIN)

make_dir:
	mkdir -p build/ temp/


.PHONY: all clean run run_debug

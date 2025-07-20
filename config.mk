CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -Iinclude -O0 -ggdb -g3

LDFLAGS =

SRC_DIR = src
INCLUDE_DIR = include
BIN_DIR = build
NAME_BIN = net_client
BIN = $(BIN_DIR)/$(NAME_BIN)

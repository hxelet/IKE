CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIRS = utils
BUILD_DIR = .build
INCLUDE_DIRS = $(addprefix -I, $(SRC_DIRS))

SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS))) main.c
OBJS = $(SRCS:.c=.o)
OBJS := $(patsubst %.o, $(BUILD_DIR)/%.o, $(OBJS))

TARGET = $(BUILD_DIR)/ike

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCUDE_DIRS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

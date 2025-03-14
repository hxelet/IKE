CC = gcc
CFLAGS = -Wall -Wextra -Wno-unused-command-line-argument -g
LDFLAGS = -L/opt/homebrew/lib -lconfig

SRC_DIRS = utils core network sa
BUILD_DIR = .build
INCLUDE_DIRS = $(addprefix -I, $(SRC_DIRS))
INCLUDE_DIRS += -I/opt/homebrew/include

SRCS = $(wildcard $(addsuffix /*.c, $(SRC_DIRS))) main.c
OBJS = $(SRCS:.c=.o)
OBJS := $(patsubst %.o, $(BUILD_DIR)/%.o, $(OBJS))

USER = $(shell whoami)
ifeq ($(USER),root)
	BEAR =
else
	BEAR = bear --
endif

TARGET = $(BUILD_DIR)/ike

all: build

build:
	$(BEAR) make $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run build

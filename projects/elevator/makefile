SOURCES := main.c modes.c interface.c modeSelector.c

SOURCE_DIR := source/
BUILD_DIR := build/
DOCS_DIR := docs/
EXECUTABLE_NAME := elevator

OBJ := $(patsubst %.c,$(BUILD_DIR)%.o,$(SOURCES))

DRIVER_ARCHIVE := $(BUILD_DIR)libdriver.a
DRIVER_SOURCE := hardware.c io.c

CC := gcc
CFLAGS := -O0 -g3 -Wall -Wextra -std=c11 -I$(SOURCE_DIR)
LDFLAGS := -L$(BUILD_DIR) -ldriver -lcomedi

.DEFAULT_GOAL := elevator

$(EXECUTABLE_NAME) : $(OBJ) | $(DRIVER_ARCHIVE)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR) :
	mkdir -p $@driver

$(BUILD_DIR)%.o : $(SOURCE_DIR)%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)driver/%.o : $(SOURCE_DIR)driver/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(DRIVER_ARCHIVE) : $(DRIVER_SOURCE:%.c=$(BUILD_DIR)driver/%.o)
	ar rcs $@ $^

.PHONY: clean
clean :
	rm -rf $(BUILD_DIR) $(DOCS_DIR)  elevator
docs:
	doxygen

LAB_USERNAME := student
LAB_HOST := 127.0.0.1

remote:
	scp $(EXECUTABLE_NAME) $(LAB_USERNAME)@$(LAB_HOST):/tmp/$(EXECUTABLE_NAME)
	export EXECUTABLE=$(EXECUTABLE_NAME)
	ssh $(LAB_USERNAME)@$(LAB_HOST) "bash -s" < buildRemote.sh

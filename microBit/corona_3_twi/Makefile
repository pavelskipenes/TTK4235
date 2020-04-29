# You only need to edit this line
SOURCES := main.c uart.c twi.c accelerometer.c print.c

# You can safely ignore everything south of this line if it intimidates you
SOURCE_DIR := source
BUILD_DIR := build

SYSTEM_BASE := $(SOURCE_DIR)/build_system
SYSTEM_FILES := system_nrf51.c gcc_startup_nrf51.S
SYSTEM_FILES := $(SYSTEM_FILES:%=$(SYSTEM_BASE)/%)
LINKER_SCRIPT := $(SYSTEM_BASE)/linker_script.ld

# Set this to "@", without the quotes, to suppress output text;
# leave it empty to see commands that are called
QUIET := @

GNU_PREFIX := arm-none-eabi
CC := $(GNU_PREFIX)-gcc
OBJCOPY := $(GNU_PREFIX)-objcopy

# General compiler flags
# CFLAGS := -Wall -Werror -std=c11
CFLAGS := -Wall -std=c11
# CPU Specific compiler flags
CFLAGS += -mcpu=cortex-m0 -mthumb -mabi=aapcs -mfloat-abi=soft
# Free linker optimizations
CFLAGS += -ffunction-sections -fdata-sections --short-enums
CFLAGS += -fno-strict-aliasing -fno-builtin

# Linker flags
LDFLAGS := --specs=nosys.specs -Wl,--gc-sections -T $(LINKER_SCRIPT)

$(BUILD_DIR)/main.hex : $(SOURCES:%.c=$(BUILD_DIR)/%.o) $(SYSTEM_FILES)
	$(QUIET)$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $(BUILD_DIR)/main.elf
	$(QUIET)$(OBJCOPY) -O ihex $(BUILD_DIR)/main.elf $@
	@echo "Files compiled"

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(QUIET)$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) :
	$(QUIET)mkdir -p $@

$(SYSTEM_BASE)/%.o : $(SYSTEM_BASE)/%.c
	$(QUIET)$(CC) $(CFLAGS) -c $< -o $@

.PHONY: flash
flash : $(BUILD_DIR)/main.hex
	$(QUIET)nrfjprog -f nrf51 --chiperase --program $<
	$(QUIET)nrfjprog -f nrf51 --reset

.PHONY: erase
erase :
	$(QUIET)nrfjprog -f nrf51 --eraseall

.PHONY: clean
clean :
	$(QUIET)rm -rf $(BUILD_DIR) $(SYSTEM_BASE)/*.o

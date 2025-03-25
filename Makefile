# Directories
STMGCC_ROOT_DIR = $(STM32_CUBE_CLT)/GNU-tools-for-STM32
STMGCC_BIN_DIR = $(STMGCC_ROOT_DIR)/bin
STMFLASH_ROOT_DIR = $(STM32_CUBE_CLT)/STM32CubeProgrammer
STMFLASH_BIN_DIR = $(STMFLASH_ROOT_DIR)/bin

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

INCLUDE_DIRS = \
				$(SRC_DIR)

# Toolchain
CC = $(STMGCC_BIN_DIR)/arm-none-eabi-gcc
OBJCOPY = $(STMGCC_BIN_DIR)/arm-none-eabi-objcopy
SIZE = $(STMGCC_BIN_DIR)/arm-none-eabi-size

FLASH = $(STMFLASH_BIN_DIR)/STM32_Programmer_CLI

CPPCHECK = cppcheck
FORMAT = clang-format

ifeq ($(OS), Windows_NT)
	RM = cmd /C del /Q /F
else
	RM = rm -rf
endif

# Files
TARGET = $(BIN_DIR)/firmware

MAIN_FILE = $(SRC_DIR)/main.c

LINKER_SCRIPT = $(SRC_DIR)/link.ld

SOURCES_WITH_HEADERS = \
						$(SRC_DIR)/hal.c

SOURCES = \
			$(MAIN_FILE) \
			$(SOURCES_WITH_HEADERS)

HEADERS = \
			$(SOURCES_WITH_HEADERS:.c=.h)

OBJECT_NAMES = $(patsubst %.s, %.o, $(patsubst %.c, %.o, $(SOURCES)))
OBJECTS = $(patsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))

# Static Analysis
CPPCHECK_FLAGS = \
					--quiet \
					--enable=all \
					--error-exitcode=1 \
					--inline-suppr \
					--suppress=missingIncludeSystem \
					--suppress=unmatchedSuppression \
					--suppress=unusedFunction \
					--suppress=checkersReport

# Flags
CORE = cortex-m4
WFLAGS = \
			-Wall -Wextra -Werror -Wshadow -Wundef \
			-Wdouble-promotion -Wformat-truncation -Wconversion
CFLAGS = \
			$(WFLAGS) \
			-mcpu=$(CORE) \
			-mthumb \
			-mfloat-abi=hard \
			-mfpu=fpv4-sp-d16 \
			-fno-common \
			-ffunction-sections \
			-fdata-sections \
			-g3 \
			-Os \
			$(addprefix -I , $(INCLUDE_DIRS))

LDFLAGS = \
			-mcpu=$(CORE) \
			-mthumb \
			-mfloat-abi=hard \
			-mfpu=fpv4-sp-d16 \
			-g3 \
			-nostartfiles \
			-nostdlib \
			--specs=nano.specs \
			-lc \
			-lgcc \
			-Wl,--gc-sections \
			-Wl,-Map=$@.map

# Build
# Compiling
$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

$(OBJ_DIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# Linking
$(TARGET): $(OBJECTS)
	mkdir -p $(dir $@)
	$(CC) \
			-T $(LINKER_SCRIPT) \
			$^ \
			$(LDFLAGS) \
			-o $@
	$(OBJCOPY) -O binary $(TARGET) $(TARGET).bin
	$(SIZE) --format=berkeley $@

# Phonies
.PHONY: all clean cppcheck flash format size

all: $(TARGET)

clean:
	$(RM) $(BUILD_DIR)

cppcheck:
	$(CPPCHECK) \
					$(CPPCHECK_FLAGS) \
					$(addprefix -I, $(INCLUDE_DIRS)) \
					$(SOURCES)

flash: $(TARGET)
	$(FLASH) -c port=SWD -e all
	$(FLASH) -c port=SWD -w $(TARGET).bin 0x08000000
	$(FLASH) -c port=SWD -hardRst
	$(FLASH) -c port=SWD disconnect

format:
	$(FORMAT) -i $(SOURCES) $(HEADERS)

size: $(TARGET)
	$(SIZE) --format=berkeley $^

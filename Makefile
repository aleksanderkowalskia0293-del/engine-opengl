CC       ?= gcc
SRC_DIR  := src
BUILD_DIR:= build
BIN_DIR  := bin
TARGET   := engine

COMMON_SRCS := $(SRC_DIR)/gl_loader.c $(SRC_DIR)/shader.c $(SRC_DIR)/texture.c \
               $(SRC_DIR)/mesh.c $(SRC_DIR)/camera.c $(SRC_DIR)/wav.c $(SRC_DIR)/main.c

ifeq ($(OS),Windows_NT)
    PLATFORM    := windows
    SRCS        := $(COMMON_SRCS) $(SRC_DIR)/platform_win32.c $(SRC_DIR)/audio_winmm.c
    TARGET_FILE := $(BIN_DIR)/$(TARGET).exe
    PNG_CFLAGS  := $(shell pkg-config --cflags libpng 2>/dev/null)
    PNG_LIBS    := $(shell pkg-config --libs libpng 2>/dev/null)
    ifeq ($(strip $(PNG_LIBS)),)
        PNG_LIBS := -lpng16 -lz
    endif
    LDLIBS := -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm $(PNG_LIBS)
else
    PLATFORM    := linux
    SRCS        := $(COMMON_SRCS) $(SRC_DIR)/platform_x11.c $(SRC_DIR)/audio_alsa.c
    TARGET_FILE := $(BIN_DIR)/$(TARGET)
    PNG_CFLAGS  := $(shell pkg-config --cflags libpng 2>/dev/null)
    PNG_LIBS    := $(shell pkg-config --libs libpng 2>/dev/null)
    ifeq ($(strip $(PNG_LIBS)),)
        PNG_LIBS := -lpng16 -lz
    endif
    LDLIBS := -lX11 -lGL -lm -lpthread -lasound $(PNG_LIBS)
endif

CFLAGS  ?= -std=c11 -Wall -Wextra -O2 -I$(SRC_DIR) $(PNG_CFLAGS)
OBJS    := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean run assets
all: $(TARGET_FILE)

$(TARGET_FILE): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

tools/gen_assets$(if $(filter windows,$(PLATFORM)),.exe,): tools/gen_assets.c
	$(CC) -std=c11 -O2 $(PNG_CFLAGS) $< -o $@ $(PNG_LIBS) -lm

assets: tools/gen_assets$(if $(filter windows,$(PLATFORM)),.exe,)
	@mkdir -p assets
	./$<

run: all assets
	./$(TARGET_FILE)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) tools/gen_assets tools/gen_assets.exe

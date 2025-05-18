# Makefile for SDL2 project with cross-platform support

# OS detection
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)
ifeq ($(UNAME_S),Darwin)
    OS = macos
    INSTALL_DIR = /usr/local/bin
    APP_NAME = "RaycastingDemo"
    EXTENSION = 
    RM = rm -f
    MKDIR = mkdir -p
    CP = cp
    RMDIR = rm -rf
else ifeq ($(UNAME_S),Linux)
    OS = linux
    INSTALL_DIR = /usr/local/bin
    APP_NAME = "RaycastingDemo"
    EXTENSION = 
    RM = rm -f
    MKDIR = mkdir -p
    CP = cp
    RMDIR = rm -rf
else
    # Windows detection (both native and MinGW/MSYS)
    OS = windows
    INSTALL_DIR = "$(PROGRAMFILES)/$(APP_NAME)"
    APP_NAME = "RaycastingDemo"
    EXTENSION = .exe
    RM = del /F /Q
    MKDIR = mkdir
    CP = copy
    RMDIR = rmdir /S /Q
endif

# Handle Windows-specific needs
ifeq ($(OS),windows)
    # Check if using MSYS/MinGW
    ifneq ($(findstring MINGW,$(UNAME_S)),)
        # MinGW environment
        SHELL = sh.exe
        MKDIR = mkdir -p
        RM = rm -f
        CP = cp
        RMDIR = rm -rf
    endif
    
    # Handle spaces in paths
    INSTALL_DIR_SAFE = $(subst /,\,$(INSTALL_DIR))
    DEBUG_DIR_SAFE = $(subst /,\,$(DEBUG_DIR))
    RELEASE_DIR_SAFE = $(subst /,\,$(RELEASE_DIR))
endif

# Compiler and flags based on platform
ifeq ($(OS),windows)
    # Prefer clang if available, fall back to gcc
    ifneq ($(shell where clang 2>nul),)
        CC = clang
    else
        CC = gcc
    endif
    CFLAGS = -Wall -std=c11
    
    # Try SDL2 config if available (MSYS2/MinGW), otherwise use common paths
    SDL2_CONFIG := $(shell where sdl2-config 2>nul)
    ifdef SDL2_CONFIG
        SDL2_FLAGS = $(shell sdl2-config --cflags)
        SDL2_LIBS = $(shell sdl2-config --libs)
    else
        # Common SDL2 locations for Windows
        ifneq ($(wildcard C:/SDL2),)
            SDL2_PATH = C:/SDL2
        else ifneq ($(wildcard C:/Program Files/SDL2),)
            SDL2_PATH = "C:/Program Files/SDL2"
        else ifneq ($(wildcard C:/Program Files (x86)/SDL2),)
            SDL2_PATH = "C:/Program Files (x86)/SDL2"
        else
            SDL2_PATH = /mingw64
        endif
        
        SDL2_FLAGS = -I$(SDL2_PATH)/include/SDL2
        SDL2_LIBS = -L$(SDL2_PATH)/lib -lSDL2main -lSDL2
        
        # Add DLL path to PATH for running
        PATH := $(SDL2_PATH)/bin:$(PATH)
    endif
else
    # macOS and Linux prefer clang
    CC = clang
    
    # Fallback to gcc if no clang
    ifeq ($(shell which clang),)
        CC = gcc
    endif
    
    CFLAGS = -Wall -std=c11
    
    # Check if sdl2-config exists
    ifneq ($(shell which sdl2-config),)
        SDL2_FLAGS = $(shell sdl2-config --cflags)
        SDL2_LIBS = $(shell sdl2-config --libs)
    else
        $(warning WARNING: sdl2-config not found. Please install SDL2 development libraries.)
        SDL2_FLAGS = -I/usr/include/SDL2 -I/usr/local/include/SDL2
        SDL2_LIBS = -lSDL2
    endif
endif

# Source and executable
SRC = src/main.c
EXE = raycaster

# Build directories
DEBUG_DIR = bin/debug
RELEASE_DIR = bin/release

# Main targets with clearer naming
all: release

# Debug configuration
debug: CFLAGS += -g -O0 -DDEBUG
debug: $(DEBUG_DIR)/$(EXE)$(EXTENSION)

$(DEBUG_DIR)/$(EXE)$(EXTENSION): $(SRC)
	$(MKDIR) $(DEBUG_DIR)
	$(CC) $(CFLAGS) $(SDL2_FLAGS) $^ -o $@ $(SDL2_LIBS)

# Release configuration
release: CFLAGS += -O2
release: $(RELEASE_DIR)/$(EXE)$(EXTENSION)

$(RELEASE_DIR)/$(EXE)$(EXTENSION): $(SRC)
	$(MKDIR) $(RELEASE_DIR)
	$(CC) $(CFLAGS) $(SDL2_FLAGS) $^ -o $@ $(SDL2_LIBS)

# Build is alias for release (standard practice)
build: release

# Run targets
run-debug: debug
ifeq ($(OS),windows)
	$(DEBUG_DIR)\$(EXE)$(EXTENSION)
else
	$(DEBUG_DIR)/$(EXE)$(EXTENSION)
endif

run: release
ifeq ($(OS),windows)
	$(RELEASE_DIR)\$(EXE)$(EXTENSION)
else
	$(RELEASE_DIR)/$(EXE)$(EXTENSION)
endif

# Clean up
clean:
ifeq ($(OS),windows)
	if exist $(DEBUG_DIR_SAFE) $(RMDIR) $(DEBUG_DIR_SAFE)
	if exist $(RELEASE_DIR_SAFE) $(RMDIR) $(RELEASE_DIR_SAFE)
else
	$(RMDIR) $(DEBUG_DIR) $(RELEASE_DIR)
endif

# Install (optional)
install: release
ifeq ($(OS),macos)
	$(MKDIR) "$(INSTALL_DIR)/$(APP_NAME)"
	$(CP) $(RELEASE_DIR)/$(EXE)$(EXTENSION) "$(INSTALL_DIR)/$(APP_NAME)/"
	@echo "Installed $(EXE) to $(INSTALL_DIR)/$(APP_NAME)"
else ifeq ($(OS),linux)
	$(MKDIR) "$(INSTALL_DIR)/$(APP_NAME)"
	$(CP) $(RELEASE_DIR)/$(EXE)$(EXTENSION) "$(INSTALL_DIR)/$(APP_NAME)/"
	@echo "Installed $(EXE) to $(INSTALL_DIR)/$(APP_NAME)"
else
	if not exist $(INSTALL_DIR_SAFE) $(MKDIR) $(INSTALL_DIR_SAFE)
	$(CP) $(RELEASE_DIR_SAFE)\$(EXE)$(EXTENSION) $(INSTALL_DIR_SAFE)
	@echo "Installed $(EXE) to $(INSTALL_DIR)"
endif

# Uninstall
uninstall:
ifeq ($(OS),macos)
	$(RMDIR) "$(INSTALL_DIR)/$(APP_NAME)"
	@echo "Uninstalled $(APP_NAME) from $(INSTALL_DIR)"
else ifeq ($(OS),linux)
	$(RMDIR) "$(INSTALL_DIR)/$(APP_NAME)"
	@echo "Uninstalled $(APP_NAME) from $(INSTALL_DIR)"
else
	if exist "$(INSTALL_DIR_SAFE)\$(EXE)$(EXTENSION)" $(RM) "$(INSTALL_DIR_SAFE)\$(EXE)$(EXTENSION)"
	@echo "Uninstalled $(EXE) from $(INSTALL_DIR)"
endif

# Show system information
info:
	@echo "Operating System: $(OS)"
	@echo "Application Name: $(APP_NAME)"
	@echo "Install Directory: $(INSTALL_DIR)/$(APP_NAME)"
	@echo "C Compiler: $(CC)"
	@echo "SDL2 Flags: $(SDL2_FLAGS)"

# Force build target: cleans and builds the project
.PHONY: force-build
force-build: clean build
	@echo "Force build complete"

.PHONY: all clean run run-debug debug release build install uninstall info